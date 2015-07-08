#pragma once

#include<cv.h>
#include"..\robot\robot.h"
#include<vector>

#ifndef ESTRATEGIA_H
#define ESTRATEGIA_H

using namespace std;

class estrategia
{
public:
	estrategia(void);
	~estrategia(void);

	robot goleiro(robot, float);

	void routing(robot, robot);

	IplImage *field;

    robot ponto_bola;
    robot ponto_f;
};

#endif
