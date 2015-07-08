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
	estrategia(int, int, int, robot, robot, robot, robot);
	~estrategia(void);

	void update(robot*);
	void Config(int, int);
	void ConfigAreas(robot, robot, robot, robot);
	void ConfigWeight(int*);

	// -------------------COMPORTAMENTO -----------------//
	
	void comportamento(void);					//Diz o comportamento de cada carrinho
	void comportamento2(void);                  //Diz o comportamento de cada carrinho 2ªversao(mais simples)
	void comportamento3(void);					//Diz o comportamento de cada carrinho 3ªversão(mais complexa que a 2ª e menos que a 3ª

	// -------------------OBJETIVOS --------------------//

	void goleiro2(void);						//Diz o objetivo atual do goleiro versao 2
	void goleiro(void);							//Diz o objetivo atual do goleiro versao 1
    void goleiro_gago(void);					//Diz o objetivo atual do goleiro versao do gago
	void atacante(void);						//Diz o objetivo atual do atacante
	void lateral(void);						    //Diz o objetivo atual do lateral
	float faz_gol(void);						//Algoritmo que diz como fazer o gol
	void zagueiro(void);						//Diz o objetivo do zagueiro
	void goleiro3(void);						//Diz o objetivo do goleiro versão 3
	void goleiro4(void);						//Diz o objetivo do goleiro versão 4
	void goleiro_definitivo(void);				//Diz o objetivo do goleiro versão definitiva
	void auxilia_goleiro(float);				//função que auxilia o goleiro definitivo
	void atacante2(void);
	void auxilia_atacante(float);

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
	
	bool bola_na_area(void);						//Diz se a bola está na nossa area ou não

	/*-------------------------------------------- Variaveis ------------------------------------------------------*/
	float x_area_gol1;			//x que define a area dos goleiros mais proxima de x = 0
	float x_area_gol2;			//x que define a area dos goleiros mais distante de x = 0
	float y_area_gol_min;		//y minimo das areas dos goleiros
	float y_area_gol_max;		//y minimo das areas dos goleiros
	float x_gol_min;			//x do gol mais próximo do x = 0
	float x_gol_max;			//x do gol mais distante do x = 0
	float y_gol_min;			//y maínimo dos gols
	float y_gol_max;			//y máximo dos gols
	float x_max;				//x máximo do campo
	float x_min;				//x mínimo do campo
	float y_max;				//y máximo do campo
	float y_min;
	bool side;
	float x_limite1;
	float x_limite2;
	bool flag_bola_area;

	float reduce;

	float weight_data;
	float weight_smooth;
	int raio;

	int ball_h;
	int ball_w;
	int ball_weight;
	int ball_radius;
	int player_radius;
	int adv_weight;
	int adv_radius;
	int comportamento1_old;
	int comportamento2_old;

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