#pragma once

#include<cv.h>

#ifndef ESTRATEGIA_H
#define ESTRATEGIA_H

class estrategia
{
public:
	estrategia(void);
	~estrategia(void);

	IplImage *field;

    robot ponto_bola;
    robot ponto_f;
};

#endif
