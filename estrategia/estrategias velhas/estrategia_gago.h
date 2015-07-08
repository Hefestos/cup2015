#pragma once

#include</opencv/include/opencv/cv.h>
#include"..\robot\robot.h"
#include<iostream>

using namespace std;

#ifndef ESTRATEGIA_H
#define ESTRATEGIA_H

class estrategia
{
public:
	estrategia(void);
	~estrategia(void);

    robot goleiro(robot, float);
    robot goleiro_gago(robot, float);
	void routing(robot, robot);
	vector<robot> sort(vector<robot>);
	vector<robot> reverse(vector<robot>);

	IplImage *field;

    robot ponto_bola;
    robot ponto_f;
};

#endif
