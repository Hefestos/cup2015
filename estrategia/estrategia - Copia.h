#pragma once


#include<opencv\cv.h>
#include"..\robot\robot.h"
#include<iostream>
#include <thread>

#define M_PI 3.14159265358979323846

using namespace cv;
using namespace std;

#ifndef ESTRATEGIA_H
#define ESTRATEGIA_H

class estrategia
{
public:
	estrategia(int, int);
	~estrategia(void);

	void update(robot*);
	void Config(int, int);

	// -------------------COMPORTAMENTO -----------------//
	
	void comportamento(void);					//Diz o comportamento de cada carrinho
	void comportamento2(void);                  //Diz o comportamento de cada carrinho 2ªversao(mais simples)
	void comportamento3(void);

	// -------------------OBJETIVOS --------------------//

	void goleiro2(void);						//Diz o objetivo atual do goleiro versao 2
	void goleiro(void);							//Diz o objetivo atual do goleiro versao 1
    void goleiro_gago(void);					//Diz o objetivo atual do goleiro versao do gago
	void atacante(void);						//Diz o objetivo atual do atacante
	void lateral(void);						    //Diz o objetivo atual do lateral
	float faz_gol(void);						//Algoritmo que diz como fazer o gol
	void zagueiro(void);
	void goleiro3(void);

	// --------------------OBSTACULOS--------------------//	

	void obstacles(void);
	void obstaclesI(int n);

	// ----------------------ROTAS----------------------//

	void routing(robot, robot);
	void aStar(robot, robot);
	void weightAStar(robot, robot);
	void weightAStarI(int);
	void minimizeRoute(void);
	void minimizeRouteI(int);
	void smoothPath(void);
	void smoothPathI(int);
	void nextGoal(void);
	void nextGoalI(int);

	//-----------RECOLHIMENTO DE INFORMAÇÕES------------//

	void Cria_distancia(void);					// Calcula as distancias de cada jogador a bola e ao gol
	void Posse(void);							// Detecta se cada carrinho possui ou nao posse de bola

	//--------------FUNÇÕES AUXILIARES ----------------//

	vector<robot> sort(vector<robot>);			// Organiza um vector de robots pelo valor do g de forma crescente
	vector<robot> sortA(vector<robot>);			// Organiza um vector de robots pelo valor do h de forma crescente
	vector<robot> reverseVector(vector<robot>);	// Inverte os valores de um vector
	float distance(robot, robot);				// Calcula a distancia entre dois pontos

	void generalRouting(void);

	bool bola_na_area();						//Diz se a bola está na nossa area ou não
	

	/*-------------------------------------------- Variaveis ------------------------------------------------------*/
	float x_area_gol1;
	float x_area_gol2;
	float y_area_gol_min;
	float y_area_gol_max;
	float x_gol_min;
	float x_gol_max;
	float y_gol_min;
	float y_gol_max;
	float x_max;
	float x_min;
	float y_max;
	float y_min;
	bool side;
	float x_limite1;
	float x_limite2;
	bool flag_bola_area;

	float reduce;

	robot ponto_gol_adv;
	robot meio_campo;
	robot ponto_gol;
    robot ponto_bola;
    robot ponto_f;

	robot players[7];
	vector<robot> way;
	vector<robot> path[3];
	Mat field;
	Mat closed;
	Mat expand;

	Mat fieldI[3];
	Mat closedI[3];
	Mat expandI[3];

	int height, width;

	robot circulo1, circulo2;
};


#endif