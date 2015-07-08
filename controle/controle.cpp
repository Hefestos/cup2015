#include "controle.h"
#include "iostream"


#define M_PI 3.14159265358979323846

using namespace std;

//!  1ª versão de construtor dos parâmetros para o PID do controle.
/*!
*    1. O ângulo de referência é inicializado com pi/2.
*    2. A tolerância de discrepânciada posição atual do carrinho e da posição que ele deveria estar em pixels é inicializada com 7.
*    3. A velocidade é inicializada com 10.
*    4. O erro e inicializado com 0(zero).
*    \param novo_pp_f float que representa o valor inicial do parâmetro proporcional para frente do PID.
*    \param novo_pi1_f float que representa o valor inicial do parâmetro integral para frente do PID.
*    \param novo_pd_f float que representa o valor inicial do parâmetro derivativo para frente do PID.
*    \param novo_pp_t float que representa o valor inicial do parâmetro proporcional para trás do PID.
*    \param novo_pi1_t float que representa o valor inicial do parâmetro integral para trás do PID.
*    \param novo_pd_t float que representa o valor inicial do parâmetro derivativo para trás do PID.
*/
controle::controle(float novo_pp_f, float novo_pi1_f, float novo_pd_f, float novo_pp_t, float novo_pi1_t, float novo_pd_t){

	cout << "*********** OBJETO CONTROLE CRIADO ***********"<< endl;

	angleRef = M_PI/2;
	tolerancia = 7;

    soma_e = 0.0;
    novo_e = 0.0;
    e_old = 0.0;
    dev_e = 0.0;

	soma_e_r = 0.0;
	novo_e_r = 0.0;
	e_old_r = 0.0;
	dev_e_r = 0.0;

	velocidade = 10.0;

    pp_f = novo_pp_f;
    pi1_f = novo_pi1_f;
    pd_f = novo_pd_f;

    pp_t = novo_pp_t;
    pi1_t = novo_pi1_t;
    pd_t = novo_pd_t;

}

//!  2ª versão de construtor dos parâmetros do controle.
/*!
*    Nesse construtor não inicializamos os parâmetros do PID.
*    1. O ângulo de referência é inicializado com pi/2.
*    2. A tolerância de discrepânciada posição atual do carrinho e da posição que ele deveria estar em pixels é inicializada com 7.
*    3. A velocidade é inicializada com 15.
*    4. O erro e inicializado com 0(zero).
*/
controle::controle(void)
{
	angleRef = M_PI / 2;
	tolerancia = 7;

	soma_e = 0.0;
	novo_e = 0.0;
	e_old = 0.0;
	dev_e = 0.0;

	soma_e_r = 0.0;
	novo_e_r = 0.0;
	e_old_r = 0.0;
	dev_e_r = 0.0;

	velocidade = 15.0;
}

//!  3ª versão de construtor dos parâmetros do controle.
/*!
*   Nesse construtor inicializamos os parâmetros do PID por meio de um array de float.
*   1. O ângulo de referência é inicializado com pi/2.
*   2. A tolerância de discrepânciada posição atual do carrinho e da posição que ele deveria estar em pixels é inicializada com 10.
*   3. A velocidade é inicializada com 15.
*   4. O erro e inicializado com 0(zero).
*
*    \param param um array de float onde cada índice representa um parâmetro do PID na ordem:
*      -  0.Proporcional para andar para frente.
*      -  1.Integrador para andar para frente.
*      -  2.Derivativo para andar para frente.
*      -  3.Proporcional para andar para trás.
*      -  4.Integrador para andar para trás.
*      -  5.Derivativo para andar para trás.
*      -  6.Proporcional para rotacionar para um lado.
*      -  7.Integrador para rotacionar para um lado.
*      -  8.Derivativo para rotacionar para um lado.
*      -  9.Proporcional para rotacionar para o outro lado.
*      - 10.Integrador para rotacionar para o outro lado.
*      - 11.Derivativo para rotacionar para o outro lado.
*/
controle::controle(float* param)
{
	angleRef = M_PI / 2;
	tolerancia = 10;

	soma_e = 0.0;
	novo_e = 0.0;
	e_old = 0.0;
	dev_e = 0.0;

	soma_e_r = 0.0;
	novo_e_r = 0.0;
	e_old_r = 0.0;
	dev_e_r = 0.0;

	velocidade = 15.0;

	pp_f = param[0];
	pi1_f = param[1];
	pd_f = param[2];

	pp_t = param[3];
	pi1_t = param[4];
	pd_t = param[5];

	pp_r_f = param[6];
	pi1_r_f = param[7];
	pd_r_f = param[8];

	pp_r_t = param[9];
	pi1_r_t = param[10];
	pd_r_t = param[11];
}

//!  4ª versão de construtor dos parâmetros do controle.
/*!
*   Nesse construtor inicializamos os parâmetros do PID por meio de um array de float.
*   1. O ângulo de referência é inicializado com pi/2.
*   2. A tolerância de discrepânciada posição atual do carrinho e da posição que ele deveria estar em pixels é inicializada com 7.
*   3. A velocidade é inicializada com 15.
*   4. O erro e inicializado com 0(zero).
*    \param param array de float onde cada índice representa um parâmetro, que será dividido por 100(cem), do PID na ordem:
*      -  0.Proporcional para andar para frente.
*      -  1.Integrador para andar para frente.
*      -  2.Derivativo para andar para frente.
*      -  3.Proporcional para andar para trás.
*      -  4.Integrador para andar para trás.
*      -  5.Derivativo para andar para trás.
*      -  6.Proporcional para rotacionar para um lado.
*      -  7.Integrador para rotacionar para um lado.
*      -  8.Derivativo para rotacionar para um lado.
*      -  9.Proporcional para rotacionar para o outro lado.
*      - 10.Integrador para rotacionar para o outro lado.
*      - 11.Derivativo para rotacionar para o outro lado.
*/
controle::controle(int* param)
{

	cout << "*********** OBJETO CONTROLE CRIADO asdas ***********" << endl;

	angleRef = M_PI / 2;
	tolerancia = 7;

	soma_e = 0.0;
	novo_e = 0.0;
	e_old = 0.0;
	dev_e = 0.0;

	soma_e_r = 0.0;
	novo_e_r = 0.0;
	e_old_r = 0.0;
	dev_e_r = 0.0;

	velocidade = 15.0;

	pp_f = param[0]/100;
	pi1_f = param[1] / 100;
	pd_f = param[2] / 100;

	pp_t = param[3] / 100;
	pi1_t = param[4] / 100;
	pd_t = param[5] / 100;


	pp_r_f = param[6] / 100;
	pi1_r_f = param[7] / 100;
	pd_r_f = param[8] / 100;

	pp_r_t = param[9] / 100;
	pi1_r_t = param[10] / 100;
	pd_r_t = param[11] / 100;
}
//! Essa função seta a que distância do objetivo o carrinho deve reduzir sua velocidade.
void controle::set_distance_lim(float newdistance)
{
	distance_lim = newdistance;
}

//!  Função para setar os valores dos parâmetros do PID de deslocamento.
/*!
*    \param novo_pp_f float que representa o valor do parâmetro proporcional para frente do PID.
*    \param novo_pi1_f float que representa o valor do parâmetro integral para frente do PID.
*    \param novo_pd_f float que representa o valor do parâmetro derivativo para frente do PID.
*    \param novo_pp_t float que representa o valor do parâmetro proporcional para trás do PID.
*    \param novo_pi1_t float que representa o valor do parâmetro integral para trás do PID.
*    \param novo_pd_t float que representa o valor do parâmetro derivativo para trás do PID.
*/

void controle::setParametros_pid(float novo_pp_f, float novo_pi1_f, float novo_pd_f, float novo_pp_t, float novo_pi1_t, float novo_pd_t){

    pp_f = novo_pp_f;
    pi1_f = novo_pi1_f;
    pd_f = novo_pd_f;

    pp_t = novo_pp_t;
    pi1_t = novo_pi1_t;
    pd_t = novo_pd_t;
}

//!  Função para setar os valores dos parâmetros do PID.
/*!
*    \param param array de float onde cada índice representa um parâmetro do PID na ordem:
*      -  param[0]->Proporcional para andar para frente.
*      -  param[1]->Integrador para andar para frente.
*      -  param[2]->Derivativo para andar para frente.
*      -  param[3]->Proporcional para andar para trás.
*      -  param[4]->Integrador para andar para trás.
*      -  param[5]->Derivativo para andar para trás.
*      -  param[6]->Proporcional para rotacionar para um lado.
*      -  param[7]->Integrador para rotacionar para um lado.
*      -  param[8]->Derivativo para rotacionar para um lado.
*      -  param[9]->Proporcional para rotacionar para o outro lado.
*      -  param[10]->Integrador para rotacionar para o outro lado.
*      -  param[11]->Derivativo para rotacionar para o outro lado.
*/

void controle::setParam(float* param)
{
	pp_f = param[0];
	pi1_f = param[1];
	pd_f = param[2];

	pp_t = param[3];
	pi1_t = param[4];
	pd_t = param[5];


	pp_r_f = param[6];
	pi1_r_f = param[7];
	pd_r_f = param[8];

	pp_r_t = param[9];
	pi1_r_t = param[10];
	pd_r_t = param[11];
}

//!  Segunda função para setar os valores dos parâmetros do PID.Nessa função os parâmetros serão dividos por 100(cem).
/*!
*    \param param array de float onde cada índice representa um parâmetro do PID na ordem:
*      -  param[0]->Proporcional para andar para frente.
*      -  param[1]->Integrador para andar para frente.
*      -  param[2]->Derivativo para andar para frente.
*      -  param[3]->Proporcional para andar para trás.
*      -  param[4]->Integrador para andar para trás.
*      -  param[5]->Derivativo para andar para trás.
*      -  param[6]->Proporcional para rotacionar para um lado.
*      -  param[7]->Integrador para rotacionar para um lado.
*      -  param[8]->Derivativo para rotacionar para um lado.
*      -  param[9]->Proporcional para rotacionar para o outro lado.
*      -  param[10]->Integrador para rotacionar para o outro lado.
*      -  param[11]->Derivativo para rotacionar para o outro lado.
*/

void controle::setParam(int* param)
{
	angleRef = M_PI / 2;
	tolerancia = 7;

	velocidade = 15.0;

	pp_f = param[0] / 100;
	pi1_f = param[1] / 100;
	pd_f = param[2] / 100;

	pp_t = param[3] / 100;
	pi1_t = param[4] / 100;
	pd_t = param[5] / 100;


	pp_r_f = param[6] / 100;
	pi1_r_f = param[7] / 100;
	pd_r_f = param[8] / 100;

	pp_r_t = param[9] / 100;
	pi1_r_t = param[10] / 100;
	pd_r_t = param[11] / 100;
}

//!  Verifica se o carro chegou no ponto de destino.
/*!
*    Função que dispara um flag dizendo se a distância entre dois pontos é menor ou não do que a tolerância aceita.
*    Retorna true se a distância for menor que a tolerância e false caso contrário.
*    \param ponto_c robot primeiro ponto.
*    \param ponto_f robot segundo ponto.
*    \return bool retorna true se a distância for menor que a tolerância e false caso contrário.
*    \sa robot()
*/

bool controle::flag_precisao(robot ponto_c, robot ponto_f){
    float distancia = sqrt(pow((ponto_c.x - ponto_f.x),2) + pow((ponto_c.y - ponto_f.y),2));
    //cout << "distancia = " << distancia << endl;
	if (distancia < tolerancia)
		return true;
	else
		return false;
}

//!  Função para setar os valores dos parâmetros do PID de rotação.
/*!
*    \param novo_pp_f float que representa o valor do parâmetro proporcional para um lado do PID.
*    \param novo_pi1_f float que representa o valor do parâmetro integral para um lado do PID.
*    \param novo_pd_f float que representa o valor do parâmetro derivativo para um lado do PID.
*    \param novo_pp_t float que representa o valor do parâmetro proporcional para o outro lado do PID.
*    \param novo_pi1_t float que representa o valor do parâmetro integral para o outro lado do PID.
*    \param novo_pd_t float que representa o valor do parâmetro derivativo para o outro lado do PID.
*/

void controle::setParametros_pid_rotacao(float novo_pp_f, float novo_pi1_f, float novo_pd_f, float novo_pp_t, float novo_pi1_t, float novo_pd_t){

    pp_r_f = novo_pp_f;
    pi1_r_f = novo_pi1_f;
    pd_r_f = novo_pd_f;

    pp_r_t = novo_pp_t;
    pi1_r_t = novo_pi1_t;
    pd_r_t = novo_pd_t;
}

//! Converte o ângulo dado para um valor, em radianos, correspondente entre -pi e pi.

/*! \param theta float é o ângulo que se quer converter, seu valor deve estar em radianos.
*   \return theta float retorna o ângulo em radianos com um valor entre -pi e pi.
*/

float controle::npi2pi(float theta){
    if (theta>0)
        theta = fmod(theta+M_PI,2.0*M_PI)-M_PI;
    else
        theta = fmod(theta-M_PI, 2.0*M_PI)+M_PI;
    return theta;
}

//! Calcula a diferença entre o x de dois pontos e entre o y dos mesmos dois pontos.
/*! \param ponto_i robot é o primeiro par cartesiano.
*   \param ponto_f robot é o segundo par cartesiano.
*   \return delta robot retorna os valores de delta.x e delta.y
*   \sa robot()
*/

void controle::deltas(robot ponto_i, robot ponto_f){
    delta.x = ponto_f.x - ponto_i.x;
    delta.y = ponto_f.y - ponto_i.y;
}

//! Calcula os coeficientes angulares e lineares de uma reta
/*! Além do cálculo dos coeficientes angular e linear de uma reta no caso da diferença do x entre os
*   dois pontos ser igual a zero a função seta o flag_p para 1 indicando assim que temos uma reta
*   paralela ao eixo Y.
*   \param ponto_i robot é o primeiro par cartesiano.
*   \param ponto_f robot é o segundo par cartesiano.
*   \return coef robot retorna os valores de coef.x e coef.y
*   \sa robot()
*   \sa flag_p
*/

void controle::coeficientes(robot ponto_i, robot ponto_f){
    if (delta.x == 0) {
        flag_p = 1;
    } else {
        coef.x = (ponto_f.y - ponto_i.y)/(ponto_f.x - ponto_i.x);
        coef.y = ponto_i.y - (coef.x*ponto_i.x);
    }
}

//! Calcula o sentido que o carro deve andar sobre uma reta passada.
/*! Decide se o carro deve "subir" ou "descer" uma determinada reta passada.Isso se baseando se o delta x ou delta y
*   são positivos ou negativos.Sendo assim, a ordem dos pares cartesianos faz diferença para sabermos em que direção a
*   reta "evolui".
*   \param ponto_i robot é o primeiro par cartesiano.
*   \param ponto_f robot é o segundo par cartesiano.
*   \return flagsen bool retorna true se for no sentido positivo- crescente - da reta e false caso contrário.
*   \sa robot()
*   \sa flagsen
*   \sa delta()
*/

void controle::flag_Sentido(robot ponto_i, robot ponto_f){
    if (delta.x < 0) {
        flagsen =  0;     // sentido negativo
    } else if (delta.x > 0) {
        flagsen =  1;     // sentido positivo
    } else {
        if (delta.y < 0){
            flagsen =  0; // sentido negativo
        } else {
            flagsen =  1; // sentido positivo
        }
    }
}

//! Calcula se o carro deve ir de frente ou de costas
/*! Baseado no ângulo atual do carro em relação ao seu objetivo, a função decide se o o mesmo deve andar de frente
*   ou de ré em uma determinada reta.
*   \param ponto_i robot é o primeiro par cartesiano.
*   \param ponto_f robot é o segundo par cartesiano.
*   \param ponto_c robot é o par cartesiano em que o carro se encontra.
*   \return flagdir bool retorna true se for para andar de frente e false se for para ir de ré.
*   \sa robot()
*   \sa delta()
*   \sa flagdir
*   \sa npi2pi()
*/

void controle::flag_direcao(robot ponto_c, robot ponto_i, robot ponto_f){

	float deltaX = ponto_f.x - ponto_c.x;
    float deltaY = ponto_f.y - ponto_c.y;

    float angulo = atan2(deltaY, deltaX);

    float limite_superior = npi2pi(angulo + M_PI/2);
    float limite_inferior = npi2pi(angulo - M_PI/2);

	//cout << "( " << limite_inferior * (180/M_PI) << ", " << limite_superior* (180/M_PI) << ")" << endl;

    if (limite_inferior < limite_superior){
        if ((limite_inferior < ponto_c.theta) && (ponto_c.theta < limite_superior)){
            flagdir = 1; // frente
			//cout << "caso1 = " << flagdir << endl;
        } else {
            flagdir = 0; // tras
			//cout << "caso2 = " << flagdir << endl;
        }

    } else {
        if ((limite_superior < ponto_c.theta) && (ponto_c.theta < limite_inferior)){
            flagdir = 0; //tras
			//cout << "caso3 = " << flagdir << endl;
        } else {
            flagdir = 1; //frente
			//cout << "caso4 = " << flagdir << endl;
        }
    }
}

//! Calcula o erro para alcançar uma reta passada
/*! Verifica a distância do carro em relação a uma reta passada fazendo a projeção da posição
*   atual do carro na reta.
*   \param ponto_c robot é o par cartesiano em que o carro se encontra.
*   \sa robot()
*   \sa delta()
*   \sa coeficientes()
*   \sa flag direcao()
*   \sa flagdir
*/

void controle::error(robot ponto_c)
{
    if (delta.x == 0){
        if (flagdir == 1){
            dist = (ponto_c.x - ponto_i.x);
        } else {
            dist = (-1)*(ponto_c.x - ponto_i.x);
        }
    } else {
        if (flagdir == 1){
            dist = (coef.x * ponto_c.x + (-1)*ponto_c.y + coef.y)/sqrt(pow(coef.x, 2) + 1);
        } else {
            dist = (-1)*(coef.x * ponto_c.x + (-1)*ponto_c.y + coef.y)/sqrt(pow(coef.x, 2) + 1);
        }
    }
}

//! Calcula o erro para alcançar um ponto passado.
/*! Verifica a distância do carro em relação a um ponto passado utilizando o delta.x
*   e delta.y, levando em consideração o flagdir.
*   \param ponto_c robot é o par cartesiano em que o carro se encontra.
*   \return dist float retorna o valor da distância.
*   \sa robot()
*   \sa flagdir
*   \sa npi2pi()
*/

void controle::error2(robot ponto_c)
{
	//cout << "Ponto Final: (" << ponto_f.x << ", " << ponto_f.y << ")" << endl;

	float deltaX = ponto_f.x - ponto_c.x;
	float deltaY = ponto_f.y - ponto_c.y;

	float ref = atan2(deltaY, deltaX);
	float theta;

	//cout << "ref: " << ref * (180/M_PI) << endl;
	//cout << "flag de direcao: " << flagdir << endl;

	if (flagdir == 1){
		dist = npi2pi(ref - ponto_c.theta);
	}
	else {
		theta = npi2pi(ponto_c.theta + M_PI);
		dist = npi2pi(ref - theta);
	}

}

//! 2ª versão para calcula o erro para alcançar um ponto passado.
/*! Verifica a distância do carro em relação a um ponto passado utilizando o delta.x
*   e delta.y, sem levar em consideração o flagdir.
*   \param ponto_c robot é o par cartesiano em que o carro se encontra.
*   \return dist float retorna o valor da distância.
*   \sa robot()
*   \sa npi2pi()
*/

void controle::error_dc_frente(robot ponto_c)
{
	//cout << "Ponto Final: (" << ponto_f.x << ", " << ponto_f.y << ")" << endl;

	float deltaX = ponto_f.x - ponto_c.x;
	float deltaY = ponto_f.y - ponto_c.y;

	float ref = atan2(deltaY, deltaX);
	float theta;

	//cout << "ref: " << ref * (180/M_PI) << endl;
	//cout << "flag de direcao: " << flagdir << endl;

	dist = npi2pi(ref - ponto_c.theta);

}

//! Calcula o erro para ficar no angulo passado.
/*! Verifica a diferença do ângulo atual do carro para o ângulo objetivo.
*   Caso a diferença angular seja maior que 90 ou menor -90 o flagrot é setado
*   para 1 indicando que o carro deve rodar fora do intervalo de -90 até 90.
*   \param ponto_c robot é o par cartesiano em que o carro se encontra.
*   \return dist float retorna o valor da distância.
*   \sa flagrot
*   \sa robot()
*   \sa npi2pi()
*/

void controle::error_angulo(robot ponto_c)
{
    //cout << "Ponto Final: (" << ponto_f.x << ", " << ponto_f.y << ")" << endl;
	//cout << "ref: " << ref * (180/M_PI) << endl;
	//cout << "flag de direcao: " << flagdir << endl;

    dist_angulo = npi2pi(angleRef - ponto_c.theta);
	flagrot = 0;

	if (dist_angulo < -(M_PI / 2) || (dist_angulo >(M_PI / 2)))
	{
		ponto_c.theta += M_PI;
		dist_angulo = npi2pi(angleRef - ponto_c.theta);
		flagrot = 1;
	}
}

//! Limita um valor ao intervalo passado.
/*!
*   \param x float valor passado.
*   \param a float limite inferior.
*   \param b float limite superior.
*   \return float o retorno funciona da seguinte forma:
*      -  Caso o valor passado seja maior que o limite superior, esse limite é o retorno.
*      -  Caso o valor passado esteja dentro dos limites especificados, então o retorno é o próprio valor.
*      -  Caso o valor passado seja menor que o limite inferior, esse limite é o retorno.
*/

float controle::constrain(float x, float a, float b)
{
	if (x < a)
		return a;
	else if (x > b)
		return b;
	else
		return x;
}

//! 1ªversão de controle PID implementada.
/*! Nessa versão, o carro pode ir tanto de costas como de frente.
*   Esse controle é baseado num controle proporcional, derivativo e integral discretos.
*
*   \param ponto_c robot é o par cartesiano em que o carro se encontra.
*   \param novo_ponto_f robot é o par cartesiano vindo da estratégia, dizendo para onde o carro deve se movimentar.
*   \return v robot retorna a velocidade de cada motor do carro.
*   \sa robot()
*   \sa flag_direcao
*   \sa error2()
*   \sa dist
*   \sa arredonda()
*   \sa constrain()
*/

void controle::pid(robot ponto_c, robot novo_ponto_f){

	//cout << "PP Frente = " << (float)pp_f*1.0 << endl;
	//cout << "PI1 Frente = " << (float)pi1_f*1.0 << endl;
	//cout << "PD Frente = " << (float)pd_f*1.0 << endl;
	//cout << "PP Tras = " << (float)pp_t*1.0 << endl;
	//cout << "PI1 Tras = " << (float)pi1_t*1.0 << endl;
	//cout << "PD Tras = " << (float)pd_t*1.0 << endl;
	//cout << "================================================="<< endl;

	ponto_f.x = novo_ponto_f.x;
	ponto_f.y = novo_ponto_f.y;

	flag_direcao(ponto_c, ponto_c, ponto_f);

	error2(ponto_c);
	//cout << "error: " << dist * (180/M_PI) << endl;

	//error(ponto_c);

	//system("cls");
	//cout << "Erro: " << dist << endl;
	//cout << "Soma do erro: " << soma_e << endl;

	soma_e = soma_e + dist;
	novo_e = dist;
	dev_e = novo_e - e_old;

	float deltaV;

	if (soma_e >= 20)
		soma_e = 20;
	else if ((soma_e <= -20))
		soma_e = -20;


	//cout << "Soma do erro novo: " << soma_e << endl;

	if (flagdir == 1){

		deltaV = (pp_f * novo_e) + (pd_f * dev_e) + (pi1_f * soma_e);
		//cout << "i = " << (float) I << endl;

	}
	else {

		deltaV = (pp_t * novo_e) + (pd_t * dev_e) + (pi1_t * soma_e);
		//cout << "i = " << I << endl;
	}

	deltaV = constrain(deltaV, -2 * velocidade, 2 * velocidade);
	//cout << "deltaV = " << deltaV << endl;

	if (deltaV > 0)
	{
		if (flagdir == 1){
			v.x = velocidade;
			v.y = velocidade - abs(deltaV);
		}
		else {
			v.y = -1 * velocidade;
			v.x = -1 * (velocidade - abs(deltaV));
		}
	}
	else if (deltaV < 0)
	{
		if (flagdir == 1){
			v.y = velocidade;
			v.x = velocidade - abs(deltaV);
		}
		else {
			v.x = -1 * velocidade;
			v.y = -1 * (velocidade - abs(deltaV));
		}
	}
	else
	{
		if (flagdir == 1){
			v.y = velocidade;
			v.x = velocidade;
		}
		else {
			v.x = -1 * velocidade;
			v.y = -1 * velocidade;
		}
	}

	//printf("%f\n",v.y);
	//printf("%f\n",v.x);

	v.x = arredonda(v.x);
	v.y = arredonda(v.y);

	e_old = novo_e;
}

//! 2ªversão de controle PID implementada.
/*! Nessa versão, só foi levada em conta uma movimentação para frente.
*   Esse controle é baseado num controle proporcional, derivativo e integral discretos.
*
*   \param ponto_c robot é o par cartesiano em que o carro se encontra.
*   \param novo_ponto_f robot é o par cartesiano vindo da estratégia, dizendo para onde o carro deve se movimentar.
*   \return float o retorno funciona da seguinte forma:
*      -  Caso o valor passado seja maior que o limite superior, esse limite é o retorno.
*      -  Caso o valor passado esteja dentro dos limites especificados, então o retorno é o próprio valor.
*      -  Caso o valor passado seja menor que o limite inferior, esse limite é o retorno.
*   \sa robot()
*   \sa error_dc_frente()
*   \sa dist
*   \sa arredonda()
*   \sa constrain()
*/

void controle::pid_dc(robot ponto_c, robot novo_ponto_f){

	//cout << "PP Frente = " << (float)pp_f*1.0 << endl;
	//cout << "PI1 Frente = " << (float)pi1_f*1.0 << endl;
	//cout << "PD Frente = " << (float)pd_f*1.0 << endl;
	//cout << "PP Tras = " << (float)pp_t*1.0 << endl;
	//cout << "PI1 Tras = " << (float)pi1_t*1.0 << endl;
	//cout << "PD Tras = " << (float)pd_t*1.0 << endl;
	//cout << "================================================="<< endl;

	ponto_f.x = novo_ponto_f.x;
	ponto_f.y = novo_ponto_f.y;


	error_dc_frente(ponto_c);
	//cout << "error: " << dist * (180/M_PI) << endl;

	//error(ponto_c);

	//system("cls");
	//cout << "Erro: " << dist << endl;
	//cout << "Soma do erro: " << soma_e << endl;

	soma_e = soma_e + dist;
	novo_e = dist;
	dev_e = novo_e - e_old;

	float deltaV;

	if (soma_e >= 80)
		soma_e = 80;
	else if ((soma_e <= -80))
		soma_e = -80;

	deltaV = (pp_f * novo_e) + (pd_f * dev_e) + (pi1_f * soma_e);

	deltaV = constrain(deltaV, -2 * velocidade, 2 * velocidade);
	//cout << "deltaV = " << deltaV << endl;

	if (deltaV > 0)
	{
		v.x = velocidade;
		v.y = velocidade - abs(deltaV);
	}
	else if (deltaV < 0)
	{

		v.y = velocidade;
		v.x = velocidade - abs(deltaV);

	}
	else
	{
		v.y = velocidade;
		v.x = velocidade;
	}

	//printf("%f\n",v.y);
	//printf("%f\n",v.x);

	v.x = arredonda(v.x);
	v.y = arredonda(v.y);

	e_old = novo_e;
}

void controle::seta_velocidade(robot init, robot objective)
{
	if ((distance(init, objective) < distance_lim) && (distance(init, objective) > tolerancia))
	{
		velocidade = 15 * (distance(init, objective) / distance_lim);
		if (velocidade < 6)
			velocidade = 6;

	}
	else if ((distance(init, objective) < tolerancia) || (distance(init, objective) > distance_lim))
	{
		velocidade = 15;
	}
}

void controle::pid_angulo(robot ponto_c){

	soma_e = 0;
    //cout << "PP R Frente = " << (float)pp_r_f*1.0 << endl;
    //cout << "PI1 R Frente = " << (float)pi1_r_f*1.0 << endl;
    //cout << "PD R Frente = " << (float)pd_r_f*1.0 << endl;
    //cout << "PP R Tras = " << (float)pp_r_t*1.0 << endl;
    //cout << "PI1 R Tras = " << (float)pi1_r_t*1.0 << endl;
    //cout << "PD R Tras = " << (float)pd_r_t*1.0 << endl;
    //cout << "================================================="<< endl;

    error_angulo(ponto_c);

    //cout << "error: " << dist_angulo * (180/M_PI) << endl;

    //error(ponto_c);

	int rot_lim = 4;

    soma_e_r = soma_e_r + dist_angulo;
    novo_e_r = dist_angulo;
    dev_e_r = novo_e_r - e_old_r;

    float deltaV;



	if (soma_e_r >= 20)
		soma_e_r = 20;
	else if ((soma_e_r <= -20))
		soma_e_r = -20;


	//system("cls");
	//cout << "Erro: " << dist_angulo << endl;
	//cout << "Soma do erro: " << soma_e_r << endl;



    if (flagrot == 1)
	{
		deltaV = (pp_r_f * novo_e_r) + (pd_r_f * dev_e_r) + (pi1_r_f * soma_e_r);
    }
	else
	{
		deltaV = (pp_r_t * novo_e_r) + (pd_r_t * dev_e_r) + (pi1_r_t * soma_e_r);
    }

    deltaV = constrain(deltaV, -15 + rot_lim, 15 - rot_lim);


    //cout << "deltaV = " << deltaV << endl;

    if (deltaV > 0){
		//if ((abs(deltaV) > 0) && (abs(deltaV) < 9)) {
		//	deltaV = 9;
		//}

        v.x = rot_lim + abs(deltaV);
        v.y = -v.x;
    } else if (deltaV < 0) {

		//if ((abs(deltaV) > 0) && (abs(deltaV) < 9)) {
		//	deltaV = 9;
		//}

        v.x = -rot_lim -abs(deltaV);
        v.y = -v.x;
    } else {
        v.y = 0.0;
        v.x = 0.0;
    }

    //printf("%f\n",v.y);
    //printf("%f\n",v.x);

    v.x =  arredonda(v.x);
    v.y =  arredonda(v.y);

	if (abs(v.x) == rot_lim)
		v.x = 0;
	if (abs(v.y) == rot_lim)
		v.y = 0;

    e_old_r = novo_e_r;
}

void controle::controla(robot ponto_c, robot ponto_f, robot objective)
{
	//(ponto_c, objective);

	if (flag_precisao(ponto_c, ponto_f))
		pid_angulo(ponto_c);
	else
		pid(ponto_c, ponto_f);

	//cout << "Soma: " << soma_e << endl;
}

void controle::controla(robot ponto_c, robot ponto_f)
{
	if (flag_precisao(ponto_c, ponto_f))
		pid_angulo(ponto_c);
	else
		pid(ponto_c, ponto_f);
}

void controle::controla_dc(robot ponto_c, robot ponto_f)
{
	if (flag_precisao(ponto_c, ponto_f))
		pid_angulo(ponto_c);
	else
		pid_dc(ponto_c, ponto_f);
}

void controle::controle_bibi(robot carrinho, robot goal, robot objetivo)
{
}

float controle::arredonda(float in)
{

	float signal = 1.0;
	float temp = in;
	if(temp < 0)
	{
		signal = -1;
	}

	temp = abs(in);

	if((temp-(int)temp)>=0.5){
		temp = ceil(temp);
	}
	else
	{
		temp = floor(temp);
	}

	temp *=signal;
    //printf("%f\n",temp);
	return temp;


}

float controle::distance(robot first, robot second)
{
	float distance = 0;
	distance = sqrt(pow((first.x - second.x), 2) + pow(first.y - second.y, 2));

	return distance;
}


