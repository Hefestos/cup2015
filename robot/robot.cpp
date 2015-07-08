#include "robot.h"

robot::robot(void)
{}

robot::robot(float copyX, float copyY)
{
	x = copyX;
	y = copyY;
}

robot::robot(float copyX, float copyY, int new_g)
{
	x = copyX;
	y = copyY;
	g_value = new_g;
}

robot::robot(float copyX, float copyY, int new_g, float new_h)
{
	x = copyX;
	y = copyY;
	g_value = new_g;
	h_value = new_h;
}

robot::robot(float copyX, float copyY, float copyTheta)
{
	x = copyX;
	y = copyY;
	theta = copyTheta;
}

robot& robot::operator=(const robot& copy)
{
	x = copy.x;
	y = copy.y;
	theta = copy.theta;
	comportamento = copy.comportamento;
	g_value = copy.g_value;
	h_value = copy.h_value;
	posse = copy.posse;
	distancia_bola = copy.distancia_bola;
	distancia_gol = copy.distancia_gol;
	objective_x = copy.objective_x;
	objective_y = copy.objective_y;
	sec1_x = copy.sec1_x;
	sec1_y = copy.sec1_y;
	sec2_x = copy.sec2_x;
	sec2_y = copy.sec2_y;
	stop = copy.stop;
	return *this;
}

void robot::setPos(robot nrobot)
{
	x = nrobot.x;
	y = nrobot.y;
}

void robot::setPos(float nx, float ny)
{
	x = nx;
	y = ny;
}

void robot::setSec1(int nx, int ny) {
	sec1_x = nx;
	sec1_y = ny;
}

void robot::setSec1(robot sec) {
	sec1_x = (int)sec.x;
	sec1_y = (int)sec.y;
}

void robot::setSec2(int nx, int ny) {
	sec2_x = nx;
	sec2_y = ny;
}

void robot::setSec2(robot sec) {
	sec2_x = (int)sec.x;
	sec2_y = (int)sec.y;
}

void robot::setGoal(robot goal)
{
	goal_x = goal.x;
	goal_y = goal.y;
}

void robot::setGoal(float new_x, float new_y)
{
	goal_x = new_x;
	goal_y = new_y;
}

void robot::setObjective(robot new_objective)
{
	objective_x = new_objective.x;
	objective_y = new_objective.y;
}

void robot::setObjective(float nx, float ny)
{
	objective_x = nx;
	objective_y = ny;
}

void robot::setComportamento(int comp)
{
	comportamento = comp;
}

void robot::setPosse(bool p, int flag)
{
	posse = p;
	flag_pos_bola = flag;
}

void robot::setDistancia_bola(float dist_b)
{
	distancia_bola = dist_b;
}

void robot::setDistancia_gol(float dist_g)
{
	distancia_gol = dist_g;
}