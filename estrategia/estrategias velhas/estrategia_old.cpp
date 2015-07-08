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

float estrategia::goleiro(robot ponto_bola, robot ponto_f.x)
{

    robot ponto_f.x = robot ponto_f.x;

    if (robot ponto_f.y => 148)
    {
         robot ponto_f.y = 148;
    }
    else if(robot ponto_f.y <= 64)
    {
         robot ponto_f.y = 64;
    }
    else
    {
         robot ponto_f.y = robot ponto_bola.y;
    }
}

