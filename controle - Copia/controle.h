#pragma once

#include"../robot/robot.h"
#include <math.h>

class controle{
public:
    controle(float, float, float, float, float, float); // constroi o objeto
	controle(void);
	controle(float*);
	controle(int*);
    void setParametros_pid(float, float, float, float, float, float); // seta os parametros para o pid de deslocamento
    void setParametros_pid_rotacao(float, float, float, float, float, float); // seta os parametros para o pid de rotacao


	void setParam(float*);
	void setParam(int*);
    void coeficientes(robot, robot); // calcula os coeficientes angulares e lineares de uma reta - nao usado
    void deltas(robot, robot); // calcula o deltaX e o deltaY de dois pontos passados
    void flag_Sentido(robot, robot); // calcula o sentido que o carro deve andar sobre uma reta passada - nao usado
    void flag_direcao(robot, robot, robot); // calcula se o carro deve ir de frente ou de costas
    void pid(robot, robot); // calcula as velocidades para o carro se locomover
    void pid_angulo(robot); // calcula as velocidades para o carro girar
    void error(robot); // calcula o erro para alcancar uma reta passada - nao usado
    void error2(robot); // calcula o erro para alcancar um ponto passado
    void error_angulo(robot); // calcula o erro para ficar no angulo passado
	void controla(robot, robot); // controla o carrinho levando em consideracao a precisao

    bool flag_precisao(robot, robot); // verifica se o carro chegou no ponto de destino

    float npi2pi(float); // converte o valor passado para um correspondente no intervalo de -pi até pi
    float arredonda(float); // arredonda o valor passado
    float constrain (float, float, float); // limita um valor ao intervalo passado
	

    float tolerancia; // tolerancia de erro do carro ao ponto
	float angleRef; // angulo que o carro deve permanecer enquanto estiver parado

    float dist; // erro do carro ao ponto
    float dist_angulo; // erro da orientacao do carro ao angulo

    // parametros do pid de deslocamento para frente
    float pp_f;
    float pd_f;
    float pi1_f;

    // parametros do pid de deslocamento para tras
    float pp_t;
    float pd_t;
    float pi1_t;

    // parametros do pid de rotacao para frente
    float pp_r_f;
    float pd_r_f;
    float pi1_r_f;

    // parametros do pid de rotacao para tras
    float pp_r_t;
    float pd_r_t;
    float pi1_r_t;

    // erros do pid de deslocamento
    float soma_e;
    float novo_e;
    float e_old;
    float dev_e;

    // erros do pid de rotacao
    float soma_e_r;
    float novo_e_r;
    float e_old_r;
    float dev_e_r;

    float flag_p; // fala se a reta passada eh paralela ao eixo Y - nao usado
    int flagsen; // fala se o carro deve ir para o sentido positivo ou negativo do eixo x - nao usado
    int flagdir; // fala se o carro deve ir de frente ou de costas
	int flagrot; //fala se o carro roda entre -90 e 90 ou fora desse intervalo

    float velocidade; // velocidade maxima do carro

    float ve_old; //- nao usado
    float vd_old; //- nao usado

    robot delta; // guarda os coeficientes angular e linear
    robot v; // guarda as velocidades a serem usadas pelo motor vx eh a direita, vy a esquerda
    robot ponto_i; // guarda o ponto inicial da reta passada - nao usado
    robot ponto_f; // guarda o ponto de objetivo do carro
    robot coef; // guarda os coeficientes da reta
};
