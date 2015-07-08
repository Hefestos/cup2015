#include "controle.h"
#include "iostream"


#define M_PI 3.14159265358979323846

using namespace std;


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

controle::controle(float* param)
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

	velocidade = 10.0;

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

void controle::setParametros_pid(float novo_pp_f, float novo_pi1_f, float novo_pd_f, float novo_pp_t, float novo_pi1_t, float novo_pd_t){

    pp_f = novo_pp_f;
    pi1_f = novo_pi1_f;
    pd_f = novo_pd_f;

    pp_t = novo_pp_t;
    pi1_t = novo_pi1_t;
    pd_t = novo_pd_t;
}

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

void controle::setParam(int* param)
{
	angleRef = M_PI / 2;
	tolerancia = 7;

	velocidade = 5.0;

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

bool controle::flag_precisao(robot ponto_c, robot ponto_f){
    float distancia = sqrt(pow((ponto_c.x - ponto_f.x),2) + pow((ponto_c.y - ponto_f.y),2));
    //cout << "distancia = " << distancia << endl;
	if (distancia < tolerancia)
		return true;
	else
		return false;
}

void controle::setParametros_pid_rotacao(float novo_pp_f, float novo_pi1_f, float novo_pd_f, float novo_pp_t, float novo_pi1_t, float novo_pd_t){

    pp_r_f = novo_pp_f;
    pi1_r_f = novo_pi1_f;
    pd_r_f = novo_pd_f;

    pp_r_t = novo_pp_t;
    pi1_r_t = novo_pi1_t;
    pd_r_t = novo_pd_t;
}

float controle::npi2pi(float theta){
    if (theta>0)
        theta = fmod(theta+M_PI,2.0*M_PI)-M_PI;
    else
        theta = fmod(theta-M_PI, 2.0*M_PI)+M_PI;
    return theta;
}

void controle::deltas(robot ponto_i, robot ponto_f){
    delta.x = ponto_f.x - ponto_i.x;
    delta.y = ponto_f.y - ponto_i.y;
}

void controle::coeficientes(robot ponto_i, robot ponto_f){
    if (delta.x == 0) {
        flag_p = 1;
    } else {
        coef.x = (ponto_f.y - ponto_i.y)/(ponto_f.x - ponto_i.x);
        coef.y = ponto_i.y - (coef.x*ponto_i.x);
    }
}

void controle::flag_Sentido(robot ponto_i, robot ponto_f){
    if (delta.x < 0) {
        flagsen =  0; // sentido negativo
    } else if (delta.x > 0) {
        flagsen =  1; // sentido positivo
    } else {
        if (delta.y < 0){
            flagsen =  0; // sentido negativo
        } else {
            flagsen =  1; // sentido positivo
        }
    }
}

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
    } else {
        theta = npi2pi(ponto_c.theta + M_PI);
        dist = npi2pi(ref - theta);
    }

}

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

float controle::constrain(float x, float a, float b)
{
	if (x < a)
		return a;
	else if (x > b)
		return b;
	else
		return x;
}

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

	if (soma_e >= 80)
		soma_e = 80;
	else if ((soma_e <= -80))
		soma_e = -80;


	//cout << "Soma do erro novo: " << soma_e << endl;

    if (flagdir == 1){

        deltaV = (pp_f * novo_e) + (pd_f * dev_e) + (pi1_f * soma_e);	
		//cout << "i = " << (float) I << endl;

    } else {

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
        } else {
            v.y = -1*velocidade;
            v.x = -1*(velocidade - abs(deltaV));
        }
    } 
	else if (deltaV < 0)
	{
        if (flagdir == 1){
            v.y = velocidade;
            v.x = velocidade - abs(deltaV);
        } else {
            v.x = -1*velocidade;
            v.y = -1*(velocidade - abs(deltaV));
        }
    }
	else 
	{
        if (flagdir == 1){
            v.y = velocidade;
            v.x = velocidade;
        } else {
            v.x = -1*velocidade;
            v.y = -1*velocidade;
        }
    }

    //printf("%f\n",v.y);
    //printf("%f\n",v.x);

    v.x =  arredonda(v.x);
    v.y =  arredonda(v.y);

    e_old = novo_e;
}

void controle::pid_angulo(robot ponto_c){


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



    soma_e_r = soma_e_r + dist_angulo;
    novo_e_r = dist_angulo;
    dev_e_r = novo_e_r - e_old_r;

    float deltaV;



	if (soma_e_r >= 80)
		soma_e_r = 80;
	else if ((soma_e_r <= -80))
		soma_e_r = -80;


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

    deltaV = constrain(deltaV, -velocidade*2, velocidade*2);
    //cout << "deltaV = " << deltaV << endl;

    if (deltaV > 0){
        v.x = abs(deltaV);
        v.y = -v.x;
    } else if (deltaV < 0) {
        v.x = -abs(deltaV);
        v.y = -v.x;
    } else {
        v.y = 0.0;
        v.x = 0.0;
    }

    //printf("%f\n",v.y);
    //printf("%f\n",v.x);

    v.x =  arredonda(v.x);
    v.y =  arredonda(v.y);

    e_old_r = novo_e_r;
}

void controle::controla(robot ponto_c, robot ponto_f)
{
	if (flag_precisao(ponto_c, ponto_f))
		pid_angulo(ponto_c);
	else
		pid(ponto_c, ponto_f);
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


