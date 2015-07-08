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

    //robot carro_1;       // posição e comportamento do carro 1
    //robot carro_2;       // posição e comportamento do carro 2
    //robot carro_3;       // posição e comportamento do carro 3
    //robot carro_I_1;     // posição do carro adversario 1
    //robot carro_I_2;     // posição do carro adversario 2
    //robot carro_I_3;     // posição do carro adversario 3


    robot ponto_g;       // ponto em que o goleiro deveria estar

    //robot ponto_bola;    // localização da bola
    robot ponto_f_g;     // localização do ponto que o carro deve ir
	
	robot;
};

#endif
