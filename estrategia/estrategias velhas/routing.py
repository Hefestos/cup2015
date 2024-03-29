# ----------
# User Instructions:
#
# Modify the the search function so that it returns
# a shortest path as follows:
# 
# [['>', 'v', ' ', ' ', ' ', ' '],
#  [' ', '>', '>', '>', '>', 'v'],
#  [' ', ' ', ' ', ' ', ' ', 'v'],
#  [' ', ' ', ' ', ' ', ' ', 'v'],
#  [' ', ' ', ' ', ' ', ' ', '*']]
#
# Where '>', '<', '^', and 'v' refer to right, left, 
# up, and down motions. NOTE: the 'v' should be 
# lowercase.
#
# Your function should be able to do this for any
# provided grid, not just the sample grid below.
# ----------
import time

# Sample Test case
grid = [[0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0]]

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]

delta = [[-1, 0 ], # go up
         [ 0, -1], # go left
         [ 1, 0 ], # go down
         [ 0, 1 ]] # go right

delta_name = ['v', '>', '^', '<']

cost = 1

# ----------------------------------------
# modify code below
# ----------------------------------------

def search():
    closed = [[0 for row in range(len(grid[0]))] for col in range(len(grid))]
    closed[init[0]][init[1]] = 1

    expand = [[-1 for i in range(len(grid[0]))] for j in range(len(grid))]
    step = 0

    x = init[0]
    y = init[1]
    g = 0

    open = [[g, x, y]]

    found = False  # flag that is set when search is complet
    resign = False # flag set if we can't find expand

    while not found and not resign:
        if len(open) == 0:
            resign = True
            return 'fail'
        else:
            open.sort()
            open.reverse()

            print open
            next = open.pop()
            x = next[1]
            y = next[2]
            g = next[0]
            
            expand[x][y] = step
            step += 1
            
            if x == goal[0] and y == goal[1]:
                found = True
            else:
                for i in range(len(delta)):
                    x2 = x + delta[i][0]
                    y2 = y + delta[i][1]
                    if x2 >= 0 and x2 < len(grid) and y2 >=0 and y2 < len(grid[0]):
                        if closed[x2][y2] == 0 and grid[x2][y2] == 0:
                            g2 = g + cost
                            open.append([g2, x2, y2])
                            closed[x2][y2] = 1
    for i in range(len(expand)):
        print expand[i]

    
    short = [[' ' for i in range(len(grid[0]))] for j in range(len(grid))]
    short[x][y] = '*'
    for i in range(g):

        n_step = expand[x][y]
        for j in range(4):
            x2 = x + delta[j][0]
            y2 = y + delta[j][1]
            if (0<= (x2) <= (len(grid)- 1)) and (0 <= (y2) <= (len(grid[0]) -1)):
                if (n_step > expand[x2][y2]) and (expand[x2][y2] != -1):
                    n_step = expand[x2][y2]
                    short[x2][y2] = delta_name[j]
                    x3 = x2
                    y3 = y2
        x = x3
        y = y3


    return short# make sure you return the shortest path.
counter = 0
start = time.clock()
end = time.clock

a = search()
end = time.clock()
counter += 1;
print "frame_rate: ", counter/(end-start)
for i in range(len(a)):
    print a[i]


