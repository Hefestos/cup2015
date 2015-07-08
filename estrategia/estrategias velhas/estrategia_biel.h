#pragma once

#include<cv.h>
#include"..\robot\robot.h"

#ifndef ESTRATEGIA_H
#define ESTRATEGIA_H

class estrategia
{
public:
////////////////////////////////----FUNCOES----//////////////////////////////////////////////////////////////////////////////////
	estrategia(void);                                                        // construtor
	~estrategia(void);                                                       // destrutor

	robot goleiro(float);                                                    // designa para onde o goleiro deve ir
	int comportamento(void);      // designa o comportamneto de cada carrinho
	float distancia(robot, robot);                                           // calcula as distancias do carro a bola e ao ponto em que o goleiro deveria estar

	IplImage *field;

////////////////////////////////////----Variaveis----////////////////////////////////////////////////////////////////////////////

    //robot carro_1;       // posi��o e comportamento do carro 1
    //robot carro_2;       // posi��o e comportamento do carro 2
    //robot carro_3;       // posi��o e comportamento do carro 3
    //robot carro_I_1;     // posi��o do carro adversario 1
    //robot carro_I_2;     // posi��o do carro adversario 2
    //robot carro_I_3;     // posi��o do carro adversario 3


    robot ponto_g;       // ponto em que o goleiro deveria estar

    //robot ponto_bola;    // localiza��o da bola
    robot ponto_f_g;     // localiza��o do ponto que o carro deve ir
	
	robot;
};

#endif
