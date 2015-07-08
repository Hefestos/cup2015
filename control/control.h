#pragma once


#include <math.h>
#include "iostream"
#include <stdio.h>
#include <string>
#include"../serial/serial.h"
#include"../vision/vision.h"
#include"../robot/robot.h"
#include <time.h>
#include "Windows.h"

class controle_bili{
public:
	void ajuste_theta(float,float,float);				// ajusta theta para sempre crescer/decrescer

	void ajuste_referencia(float, float, float, float); //mantém a referência no padrão do theta

	void controle(int, int, int, int);					// função para o controle angular e linear do robotro

	void modelagem(float);								//utiliza as velocidades linear e angular para calcular v1 e v2

	void rotina_controle(int, float, vision*,int, int);
	

	// ADIÇÃO DO AJUSTE DOS GANHOS

	void limiter(float val, float up_lim, float down_lim);      //Limita um valor dentro de um intervalo
	/*
	up_lim   -> limite superior
	down_lim -> limite inferior
	*/
	float find_ts(char var, robot entry, int n_samples);           //encontra o tempo de assentamento
	/*
	var -> diz qual variável será analisada
	entry -> é um struct q contém as informações de posição, ângulo
	n_samples -> é o tamanho do vetor da struct que armazena todas as posições
	*/
	float find_overshoot(char var, robot entry, int n_samples);    //encontra o overshoot
	/*
	var -> diz qual variável será analisada
	entry -> é um struct q contém as informações de posição, ângulo
	n_samples -> é o tamanho do vetor da struct que armazena todas as posições
	*/
	float find_upertime(char var, robot entry, int n_samples);     //encontra o tempo de subida
	/*
	var -> diz qual variável será analisada
	entry -> é um struct q contém as informações de posição, ângulo
	n_samples -> é o tamanho do vetor da struct que armazena todas as posições
	*/
	float improve_ts(char var, robot entry);               //encontra os novos ganhos baseado na análise do tempo de assentamento
	/*
	var -> diz qual variável será analisada
	entry -> é um struct q contém as informações de posição, ângulo
	*/
	float improve_overshoot(char var, robot entry);        //encontra os novos ganhos baseado na análise do overshoot
	/*
	var -> diz qual variável será analisada
	entry -> é um struct q contém as informações de posição, ângulo
	*/
	float improve_upertime(char var, robot entry);         //encontra os novos ganhos baseado na análise do tempo de subida
	/*
	var -> diz qual variável será analisada
	entry -> é um struct q contém as informações de posição, ângulo
	*/
	float improve_PID(robot entry, char var);     //encontra os ganhos basado no tempo de assentamento, overshoot, tempo de subida
	/*
	var -> diz qual variável será analisada
	entry -> é um struct q contém as informações de posição, ângulo
	*/

	int controle_bili::improve_p_ts(robot entry, char var, float wanted_ts, int step_flag);
	int controle_bili::improve_i_ts(robot entry, char var, float wanted_ts, int step_flag);
	int controle_bili::improve_d_ts(robot entry, char var, float wanted_ts, int step_flag);

	int controle_bili::improve_p_overshoot(robot entry, char var, float wanted_overshoot, int step_flag);
	int controle_bili::improve_i_overshoot(robot entry, char var, float wanted_overshoot, int step_flag);
	int controle_bili::improve_d_overshoot(robot entry, char var, float wanted_overshoot, int step_flag);

	int controle_bili::improve_p_upertime(robot entry, char var, float wanted_upertime, int step_flag);
	int controle_bili::improve_i_upertime(robot entry, char var, float wanted_upertime, int step_flag);
	int controle_bili::improve_d_upertime(robot entry, char var, float wanted_upertime, int step_flag);
	


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
	float w ; // velocidade angular
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
	//--------------Parâmetros do controle angular frente-esquerda ------------------------//
	
	float w_p;
	float w_i = 0.0;
	float w_d = 0.0;
	

	//--------------Parâmetros do controle linear frente-esquerda ------------------------//
	float u_p;
	float u_i = 0.0;
	float u_d = 0.0;

	
};

