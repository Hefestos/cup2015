#pragma once

#include"../robot/robot.h"
#include"../vision/vision.h"
#include <math.h>

class controle_bili{
public:
	void ajuste_distancia(float, float);

	void ajuste_theta(float,float,float); // ajusta theta para sempre crescer/decrescer

	void ajuste_referencia(float, float, float, float); //mantém a referência no padrão do theta

	void controle(int, int, int, int, float); // função para o controle angular e linear do carro

	void modelagem(float); //utiliza as velocidades linear e angular para calcular v1 e v2

	void rotina_controle(int, float, vision*, int, int, float);

	//--------------Parâmetros gerais------------------------------------------------//

	int inicio_voltas = 0; //define o início de voltas ao redor do próprio eixo do carro
	float thetad = 0.0; //inical do theta de referências pros carros
	float thetad_old = 0.0; // inicial do auxiliar do theta de referência para os carros 
	float ang_error; //erro do controle angular
	float alpha;    //angulo do carro para referencia do controle
	float h = 0.038; // relativo à taxa de frames (26 frames -> h=1/26)
	float v1 = 0.0 ; // velocidade inicial motor dir
	float v2 = 0.0;  // velocidade inicial motor esq
	float i; //flag de direção de v1
	float o; //flag de direção de v2
	float u ; // velocidade linear
	float w_teste ; // velocidade angular
	float theta = 0.0; // angulo do carro
	int x;
	int y;
	float refaux_old;
	float refaux_acumulated;
	float theta_old;
	float theta_acumulated;
	float KP_w;
	float KP_u;
	float refaux = 0.0;
	float KI_w;
	float KI_u;
	float KD_w;
	float KD_u;
	float x_old = 0.0;
	float y_old = 0.0;
	float dist = 0.0;
	float dist_old = 0.0;
	float ang_error_old = 0.0;
	float dif_ang ;
	float dif_ang_old;
	int flag_dir;
	float aux_x;
	float aux_old_x = 0;
	float deriv_x;
	float aux_y;
	float aux_old_y = 0;
	float deriv_y;
	float aux_theta;
	float aux_old_theta = 0;
	float deriv_theta;
	float n_xd;
	float n_yd;
	float vel;
	//--------------Parâmetros do controle angular frente-esquerda ------------------------//
	
	float w_p;
	float w_i = 0.0;
	float w_d = 0.0;
	

	//--------------Parâmetros do controle linear frente-esquerda ------------------------//
	float u_p;
	float u_i = 0.0;
	float u_d = 0.0;

	
};

