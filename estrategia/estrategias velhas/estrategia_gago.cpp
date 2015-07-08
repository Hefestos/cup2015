#include "estrategia.h"


estrategia::estrategia()
{
	field = cvCreateImage(cvSize(270,102),IPL_DEPTH_8U,1);
	cvSet(field,cvScalar(0,0,0));
	cvCircle(field, cvPoint(270/2, 102/2), 10, cvScalar(128), -1);
	cvCircle(field, cvPoint(270/2 + 5, 102/2 + 5), 10, cvScalar(128), -1);
}

estrategia::~estrategia()
{


}

robot estrategia::goleiro(robot ponto_bola, float x_goleiro)
{
	ponto_f.x = x_goleiro;

    if (ponto_bola.y >= 148)
    {
         ponto_f.y = 148;
    }
    else if(ponto_bola.y <= 64)
    {
         ponto_f.y = 64;
    }
    else
    {
         ponto_f.y = ponto_bola.y;
    }
	return ponto_f;
}

robot estrategia::goleiro_gago(robot ponto_bola, float x_goleiro)
{
    float y_max = 148; // y maximo que o goleiro pode ir
    float y_min = 64; // y minimo que o goleiro pode ir
    float y_med  = (y_max + y_min)/2.0;

    float coeficiente = ponto_bola.x*1.0/x_goleiro*1.0; // constante de proporcionalidade da distancia da bola ao goleiro

	ponto_f.x = x_goleiro;

    if (ponto_bola.y > y_med){
        ponto_f.y = y_med + ((y_med - ponto_bola.y)/coeficiente);
    } else if (ponto_bola.y > y_med){
        ponto_f.y = y_med - ((y_med - ponto_bola.y)/coeficiente);
    } else {
        ponto_f.y = y_med;
    }
	return ponto_f;
}

void estrategia::routing(robot init, robot end)
{
	std::vector<robot> open;
	init.theta = 0;
	int step = 0;

	int delta[8][3] =	{{-1, 0 }, // go up
						{ -1, -1}, // go northwest
						{ 0, -1}, // go left
						{ 1, -1}, // go southwest
						{ 1, 0}, // go down
						{ 1, 1}, // go southeast
						{ 0, 1 }, // go right
						{ -1, 1 }}; // go norheast

	for(int k = 0; k < 8; k++)
		cout << "(" << delta[k][0] << ", " << delta[k][1] << ")" << endl;

	int height, width;
	CvScalar pixel;

	height = field ->height;
	width = field -> width;

	bool found = false;
	bool resign = false;

	CvMat* closed = cvCreateMat(height, width, CV_32SC1);
	CvMat* expand = cvCreateMat(height, width, CV_32SC1);

	pixel.val[0] = -1;
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			cvSet2D(expand, i, j, pixel);
		}
	}
	pixel.val[0] = 0;
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			cvSet2D(closed, i, j, pixel);
		}
	}
	
	open.push_back(init);
	init.theta = 2;
	open.push_back(init);
	init.theta = 3;
	open.push_back(init);
	init.theta = 4;
	open.push_back(init);

	
	open = reverse(open);

	while (!found && !resign)
	{
		if (open.size() == 0)
		{
			resign = true;
			break;
		}
		else
		{



			found = true;
		}
	}

	
	pixel = cvGet2D(closed, 102/2, 270/2);
	cout << "Intensidade: " << pixel.val[0] << endl;
}

vector<robot> estrategia::sort(vector<robot> open)
{
	robot aux;
	int k = open.size() - 1; 
	for (int i = 0; i < open.size(); i++)
	{
		int j = 0;
		while (j < k)
		{
			if (open[j].theta > open[j+1].theta)
			{
				aux = open[j];
				open[j] = open[j+1];
				open[j+1] = aux;
			}
			j += 1;
		}
		k -= 1;
	}
	return open;
}

vector<robot> estrategia::reverse1(vector<robot> open)
{
	reverse(open.begin(), open.end());
	return open;
}
