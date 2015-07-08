/*
 * Gláuber (10 de outubro de 2014)
 * Esse código eventualmente vai quebrar. Boa sorte, não tem conserto, você terá que reescrever. Enjoy your day ! :)
 */
#include "control.h"
#include<vector>
//#include "../controle_bili/controle_bili.h"
using namespace cvb;
using namespace std;




#define M_PI 3.14159265358979323846

char char_pressed = cvWaitKey(1);





void controle_bili::ajuste_referencia(float x, float y, float xd, float yd){


	cout << "xd = " << xd << endl;
	cout << "yd = " << yd << endl;
	cout << "x = " << x << endl;
	cout << "y = " << y << endl;

	

	refaux = atan2(yd - y, xd - x);
	if (refaux == 0)
	{
		thetad = theta;
	}
	else
	{
		thetad = refaux;
	}
	
	
	
	
	cout << "refaux = " << refaux << endl;
	

}

void controle_bili::controle(int x, int y, int xd, int yd){




	//--------------------ANGULAR-------------------------------//



		//ang_error = thetad - theta;

		if (thetad - theta >= M_PI)
		{
			ang_error = -(2 * M_PI - (thetad - theta));

			
		}

		else if (thetad - theta <= -M_PI)
		{
			ang_error = 2 * M_PI + (thetad - theta);
			

			
		}
		else
		{
			ang_error = thetad - theta;
		}

		KI_w = -0.1;
		w_i = w_i + h*KI_w*(ang_error);



		if (w_i >= 3)
		{
			w_i = 0;
		}

		else if (w_i <= -3)
		{
			w_i = 0;
		}

		else if (abs(thetad - theta) <= 8*M_PI/180)
		{

			w_i = 0.0;

		}



		KD_w = -70;
		w_d = KD_u *((ang_error - ang_error_old) / h*h);

		KP_w = -12;
		w_p = KP_w*(ang_error);


		

		cout << "W_I= " << w_i << endl;


		//--------------------------------LINEAR-------------------------------//



		dist = sqrt(pow(x - xd, 2) + pow(y - yd, 2));

		KI_u = 0.009;
		u_i = u_i + h*KI_u*dist;

		KP_u = 0.05;
		u_p = KP_u*dist;


		KD_u = 0.6;
		u_d = KD_u *((dist - dist_old) / h*h);

		u = u_p + u_i + u_d;

		if (u_i >= 2)
		{
			u_i = 0;
		}

		else if (u_i <= -2)
		{
			u_i = -0;
		}

		if (pow(x - xd, 2) + pow(y - yd, 2) <= 140){
			//dist = 0.0;
			u_i = 0.0;
			w = 0.0;
			w_i = 0.0;
			u = 0.0;
		}

		if (w_i == 3 && u_i == 2 || w_i == -3 && u_i == -2 || w_i == 3 && u_i == -2 || w_i == -3 && u_i == 2)
		{
			w_i = 0;
			u_i = 0;
		}

		w = w_i + w_p + w_d;
		u = u_p + u_i + u_d;

	cout << "UI = " << u_i << endl;
	cout << "DIST = " << dist << endl;
	cout << "DIST_OLD = " << dist_old << endl;

	cout << "SUBTRAÇÃO = " << dist - dist_old << endl;

	cout << "u = " << u << endl;
	cout << "w = " << w << endl;

	
}

void controle_bili::modelagem(float e){
	


	

	v1 = ((0.5)*u - (w*0.5)/ 3.25) / 10.0;
	v2 = ((0.5)*u + w*0.5 / 3.25) / 10.0;
	
	
	if (v1 > 1) v1 = 1;
	if (v1 < -1) v1 = -1;

	if (v2 > 1) v2 = 1;
	if (v2 < -1) v2 = -1;

	if (v1 > 0){

		i = 1;
	}

	else {

		i = 0;
	}



	if (v2 > 0){

		o = 1;
	}

	else {

		o = 0;
	}


}

void controle_bili::rotina_controle(int num_carro, float e, vision* Camera, int xd, int yd){

	
	
	x = Camera->players[num_carro].x - 155;
	y = -(Camera->players[num_carro].y - 116);

	//xd = Camera.players[num_carro].goal_x - 155;
	//yd = -(Camera.players[num_carro].goal_y - 116);
	//xd = xd - 155;
	//yd = -(yd - 116);

	theta = -Camera->players[num_carro].theta;
	


	ajuste_referencia( x,  y,  xd, yd);

	controle(x, y, xd, yd);
	
	modelagem(e);

	if (Camera->players[num_carro].x == -1)
	{
		v1 = 0.0;
		v2 = 0.0;
		u_i = 0.0;
		w_i = 0.0;
		u = 0.0;
		w = 0.0;

	
	}

	ang_error_old = ang_error;
	dist_old = dist;
	x_old = x;
	y_old = y;
}




//// ----- MÉTODOS DE AJUSTE PARALELO ------------------//
///*Nesses métodos faz-se a análise dos 3 ganhos ao msm tempo(PARALELO) e leva-se em consideração os parâmetros a serem ajustados
//em série, isto é, primeiro um, depois outro, por exemplo: tempo de assentamento, overshoot e tempo de subida */
//float controle_bili::improve_ts(char var, robot entry)
//{
//	/*IMPROVE BASEADO EM ALGUM LIVRO
//	Para melhorar o tempo de assentamento fazem-se as seguintes considerações:
//	1- Um tempo de assentamento >= "x" segundos será muito alto
//	2- Um tempo de assentamento < "x" segundos e >= "y" será alto
//	3- Um tempo de assentamento < de "y" segundos será considerado aceitavel*/
//	float ts;
//	switch (var)
//	{
//	case 'x':
//		ts = find_ts('x', entry, entry.group_x.size());
//		break;
//	case 'y':
//		ts = find_ts('y', entry, entry.group_y.size());
//		break;
//	case 't':
//		ts = find_ts('t', entry, entry.group_theta.size());
//		break;
//	}
//
//	if ((entry.limit_gain[0] == 0.0) && (entry.limit_gain[1] == 0.0) && (ts >= 150)) //muito alto
//	{
//		entry.gain[0] *= 0.5;
//	}
//	else if ((entry.limit_gain[2] == 0.0) && (entry.limit_gain[3] == 0.0) && (ts >= 150))
//	{
//		entry.gain[1] *= 1.5;
//	}
//	else if ((entry.limit_gain[4] == 0.0) && (entry.limit_gain[5] == 0.0) && (ts >= 150))
//	{
//		entry.gain[2] *= 0.3;
//	}
//	else if ((ts < 150) && (ts >= 75) && (entry.limit_gain[0] == 0.0) && (entry.limit_gain[1] == 0.0)) //alto
//	{
//		entry.gain[0] *= 0.8;
//	}
//	else if ((ts < 150) && (ts >= 75) && (entry.limit_gain[2] == 0.0) && (entry.limit_gain[3] == 0.0))
//	{
//		entry.gain[1] *= 1.2;
//	}
//	else if ((ts < 150) && (ts >= 75) && (entry.limit_gain[4] == 0.0) && (entry.limit_gain[5] == 0.0))
//	{
//		entry.gain[2] *= 0.5;
//	}
//
//	else if (ts >= 150) //muito alto
//	{
//		entry.gain[0] *= 0.5;
//		entry.gain[1] *= 1.5;
//		entry.gain[2] *= 0.3;
//		limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
//		limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
//		limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
//	}
//	else if ((ts < 150) && (ts >= 75)) //alto
//	{
//		entry.gain[0] *= 0.8;
//		entry.gain[1] *= 1.2;
//		entry.gain[2] *= 0.5;
//		limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
//		limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
//		limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
//	}
//	cout << "PROPORCIONAL = " << entry.gain[0] << endl;
//	cout << "INTEGRAL = " << entry.gain[1] << endl;
//	cout << "DERIVATIVO = " << entry.gain[2] << endl;
//}
//
//float controle_bili::improve_overshoot(char var, robot entry)
//{
//	/*IMPROVE BASEADO EM ALGUM LIVRO
//	Para melhorar o tempo de assentamento fazem-se as seguintes considerações:
//	1- Um tempo de assentamento >= "x" segundos será muito alto
//	2- Um tempo de assentamento < "x" segundos e >= "y" será alto
//	3- Um tempo de assentamento < de "y" segundos será considerado aceitavel*/
//	float x_d, y_d, theta_d, overshoot;
//
//	switch (var)
//	{
//	case 'x':
//		x_d = entry.group_x.back();
//		overshoot = find_overshoot('x', entry, entry.group_x.size());
//		break;
//	case 'y':
//		y_d = entry.group_y.back();
//		overshoot = find_overshoot('y', entry, entry.group_y.size());
//		break;
//	case 't':
//		theta_d = entry.group_theta.back();
//		overshoot = find_overshoot('t', entry, entry.group_theta.size());
//		break;
//	}
//
//	if ((overshoot > x_d*0.1) && (var == 'x') && (entry.limit_gain[0] == 0.0) && (entry.limit_gain[1] == 0.0)) // alto
//	{
//		entry.gain[0] *= 1.2;
//	}
//	else if ((overshoot > x_d*0.1) && (var == 'x') && (entry.limit_gain[2] == 0.0) && (entry.limit_gain[3] == 0.0))
//	{
//		entry.gain[1] *= 0.8;
//	}
//	else if ((overshoot > x_d*0.1) && (var == 'x') && (entry.limit_gain[4] == 0.0) && (entry.limit_gain[5] == 0.0))
//	{
//		entry.gain[2] *= 1.5;
//	}
//
//	else if ((overshoot > x_d*0.1) && (var == 'x')) // alto
//	{
//		entry.gain[0] *= 1.2;
//		entry.gain[1] *= 0.8;
//		entry.gain[2] *= 1.5;
//		limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
//		limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
//		limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
//	}
//
//	if ((overshoot > y_d*0.1) && (var == 'y') && (entry.limit_gain[0] == 0.0) && (entry.limit_gain[1] == 0.0)) // alto
//	{
//		entry.gain[0] *= 1.2;
//	}
//	else if ((overshoot > y_d*0.1) && (var == 'y') && (entry.limit_gain[2] == 0.0) && (entry.limit_gain[3] == 0.0))
//	{
//		entry.gain[1] *= 0.8;
//	}
//	else if ((overshoot > y_d*0.1) && (var == 'y') && (entry.limit_gain[4] == 0.0) && (entry.limit_gain[5] == 0.0))
//	{
//		entry.gain[2] *= 1.5;
//	}
//
//	else if ((overshoot > y_d*0.1) && (var == 'y')) // alto
//	{
//		entry.gain[0] *= 1.2;
//		entry.gain[1] *= 0.8;
//		entry.gain[2] *= 1.5;
//		limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
//		limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
//		limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
//	}
//
//	if ((overshoot > theta_d*0.1) && (var == 't') && (entry.limit_gain[0] == 0.0) && (entry.limit_gain[1] == 0.0)) // alto
//	{
//		entry.gain[0] *= 1.2;
//	}
//	else if ((overshoot > theta_d*0.1) && (var == 't') && (entry.limit_gain[2] == 0.0) && (entry.limit_gain[3] == 0.0))
//	{
//		entry.gain[1] *= 0.8;
//	}
//	else if ((overshoot > theta_d*0.1) && (var == 't') && (entry.limit_gain[4] == 0.0) && (entry.limit_gain[5] == 0.0))
//	{
//		entry.gain[2] *= 1.5;
//	}
//
//	else if ((overshoot > theta_d*0.1) && (var == 't')) // alto
//	{
//		entry.gain[0] *= 1.2;
//		entry.gain[1] *= 0.8;
//		entry.gain[2] *= 1.5;
//		limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
//		limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
//		limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
//	}
//	cout << "PROPORCIONAL = " << entry.gain[0] << endl;
//	cout << "INTEGRAL = " << entry.gain[1] << endl;
//	cout << "DERIVATIVO = " << entry.gain[2] << endl;
//}
//
//float controle_bili::improve_upertime(char var, robot entry)
//{
//	/*IMPROVE BASEADO EM ALGUM LIVRO
//	Para melhorar o tempo de assentamento fazem-se as seguintes considerações:
//	1- Um tempo de assentamento >= "x" segundos será muito alto
//	2- Um tempo de assentamento < "x" segundos e >= "y" será alto
//	3- Um tempo de assentamento < de "y" segundos será considerado aceitavel*/
//
//	float upertime;
//	switch (var)
//	{
//	case 'x':
//		upertime = find_upertime('x', entry, entry.group_x.size());
//		break;
//	case 'y':
//		upertime = find_upertime('y', entry, entry.group_y.size());
//		break;
//	case 't':
//		upertime = find_upertime('t', entry, entry.group_theta.size());
//		break;
//	}
//
//	if ((entry.limit_gain[0] == 0.0) && (entry.limit_gain[1] == 0.0) && (upertime >= 15)) //muito alto
//	{
//		entry.gain[0] *= 1.5;
//	}
//	else if ((entry.limit_gain[2] == 0.0) && (entry.limit_gain[3] == 0.0) && (upertime >= 15))
//	{
//		entry.gain[1] *= 1.5;
//	}
//	else if ((entry.limit_gain[4] == 0.0) && (entry.limit_gain[5] == 0.0) && (upertime >= 15))
//	{
//		entry.gain[2] *= 0.3;
//	}
//	else if ((upertime < 15) && (upertime >= 7.5) && (entry.limit_gain[0] == 0.0) && (entry.limit_gain[1] == 0.0)) //alto
//	{
//		entry.gain[0] *= 1.2;
//	}
//	else if ((upertime < 15) && (upertime >= 7.5) && (entry.limit_gain[2] == 0.0) && (entry.limit_gain[3] == 0.0))
//	{
//		entry.gain[1] *= 1.2;
//	}
//	else if ((upertime < 15) && (upertime >= 7.5) && (entry.limit_gain[4] == 0.0) && (entry.limit_gain[5] == 0.0))
//	{
//		entry.gain[2] *= 0.5;
//	}
//
//	else if (upertime >= 15) //muito alto
//	{
//		entry.gain[0] *= 1.5;
//		entry.gain[1] *= 1.5;
//		entry.gain[2] *= 0.3;
//		limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
//		limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
//		limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
//	}
//	else if ((upertime < 15) && (upertime >= 7.5)) //alto
//	{
//		entry.gain[0] *= 1.2;
//		entry.gain[1] *= 1.2;
//		entry.gain[2] *= 0.5;
//		limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
//		limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
//		limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
//	}
//	cout << "PROPORCIONAL = " << entry.gain[0] << endl;
//	cout << "INTEGRAL = " << entry.gain[1] << endl;
//	cout << "DERIVATIVO = " << entry.gain[2] << endl;
//}


void controle_bili::limiter(float val, float up_lim, float down_lim)
{
	if (val >= up_lim)
	{
		val = up_lim;
	}
	else if (val <= down_lim)
	{
		val = down_lim;
	}
}

float controle_bili::find_ts(char var, robot entry, int n_samples)
{
	/*Inicializa vetores para fazer os cálculos, dependendo da entrada a ser
	recebida pode-se n criá-los*/
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> theta;
	std::vector<float> time;		// ccontador de amostras
	std::vector<float> time2stable;	// vector usado para encontrar o tempo de assentamento


	// ---- DEGRAU ---- //
	for (int i = 0; i < n_samples; i++)
	{
		time.push_back(i);
		switch (var)
		{
		case 'x':
			x.push_back(entry.group_x[i]);
			break;
		case 'y':
			y.push_back(entry.group_y[i]);
			break;
		case 't':
			theta.push_back(entry.group_theta[i]);
			break;
		}
	}
	// ---- FIM DO DEGRAU ---- //

	for (int i = time.size(); i != 0; i--)
	{
		if (var == 'x')
		{
			float stable_sup_t = x.back()*1.05;
			float stable_inf_t = x.back()*0.95;

			if (x[i - 1] > 0)
			{
				if ((x[i - 1] < stable_sup_t) && (x[i - 1] > stable_inf_t))
				{
					time2stable.push_back(time[i - 1]);
				}
				else
				{
					break;
				}
			}
			else
			{
				if ((x[i - 1] > stable_sup_t) && (x[i - 1] < stable_inf_t))
				{
					time2stable.push_back(time[i - 1]);
				}
				else
				{
					break;
				}
			}
		}
		if (var == 'y')
		{
			float stable_sup_t = y.back()*1.05;
			float stable_inf_t = y.back()*0.95;

			if (y[i - 1] > 0)
			{
				if ((y[i - 1] < stable_sup_t) && (y[i - 1] > stable_inf_t))
				{
					time2stable.push_back(time[i - 1]);
				}
				else
				{
					break;
				}
			}
			else
			{
				if ((y[i - 1] > stable_sup_t) && (y[i - 1] < stable_inf_t))
				{
					time2stable.push_back(time[i - 1]);
				}
				else
				{
					break;
				}
			}
		}
		if (var == 't')
		{
			float stable_sup_t = theta.back()*1.05;
			float stable_inf_t = theta.back()*0.95;

			if (theta[i - 1] > 0)
			{
				if ((theta[i - 1] < stable_sup_t) && (theta[i - 1] > stable_inf_t))
				{
					time2stable.push_back(time[i - 1]);
				}
				else
				{
					break;
				}
			}
			else
			{
				if ((theta[i - 1] > stable_sup_t) && (theta[i - 1] < stable_inf_t))
				{
					time2stable.push_back(time[i - 1]);
				}
				else
				{
					break;
				}
			}
		}
	}

	//cout << "t_s = " << time2stable.back() << endl;
	return time2stable.back();
}

float controle_bili::find_overshoot(char var, robot entry, int n_samples)
{

	float x_d, y_d, theta_d;
	float maxi = 0.0;	// valor máximo da resposta do sistema
	float overshoot;	// valor do overshoot
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> theta;
	std::vector<float> time;

	switch (var)
	{
	case 'x':
		x_d = entry.group_x.back();
		break;
	case 'y':
		y_d = entry.group_y.back();
		break;
	case 't':
		theta_d = entry.group_theta.back();
		break;
	}

	// ---- DEGRAU ---- //
	for (int i = 0; i < n_samples; i++)
	{
		time.push_back(i);
		switch (var)
		{
		case 'x':
			x.push_back(entry.group_x[i]);
			break;
		case 'y':
			y.push_back(entry.group_y[i]);
			break;
		case 't':
			theta.push_back(entry.group_theta[i]);
			break;
		}
	}
	// ---- FIM DO DEGRAU ---- //

	switch (var)
	{
	case 'x':
		maxi = *std::max_element(x.begin(), x.end());
		overshoot = maxi - x_d;
		break;
	case 'y':
		maxi = *std::max_element(y.begin(), y.end());
		overshoot = maxi - y_d;
		break;
	case 't':
		maxi = *std::max_element(theta.begin(), theta.end());
		overshoot = maxi - theta_d;
		break;
	}

	float temp1, temp2;
	if (x_d >= 0.0)
	{
		temp1 = overshoot - 1.0;
		temp2 = x_d - 1.0;
	}
	else
	{
		temp1 = maxi + 1.0;
		temp2 = x_d + 1.0;
	}

	return overshoot;
}

float controle_bili::find_upertime(char var, robot entry, int n_samples)
{
	float x_d, y_d, theta_d, upertime; // upertime é o tempo de subida
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> theta;
	std::vector<float> time;

	// ----------  Switch para saber qual variável será analisada ------------------///
	switch (var)
	{
	case 'x':
		x_d = entry.group_x.back();
		break;
	case 'y':
		y_d = entry.group_y.back();
		break;
	case 't':
		theta_d = entry.group_theta.back();
		break;
	}

	// ---- DEGRAU ---- //
	for (int i = 0; i < n_samples; i++)
	{
		time.push_back(i);
		switch (var)
		{
		case 'x':
			x.push_back(entry.group_x[i]);
			break;
		case 'y':
			y.push_back(entry.group_y[i]);
			break;
		case 't':
			theta.push_back(entry.group_theta[i]);
			break;
		}
	}
	// ---- FIM DO DEGRAU ---- //

	for (int i = 0; i != time.size(); i++)
	{
		if (var == 'x')
		{
			if ((x_d >= 0) && (x[0] < x_d) && (x[i] >= x_d*0.9))
			{
				upertime = time[i];
				break;
			}
			else if ((x_d >= 0) && (x[0] >= x_d) && (x[i] <= x_d*0.9))
			{
				upertime = time[i];
				break;
			}

			else if ((x_d < 0) && (x[0] < x_d) && (x[i] >= x_d*0.9))
			{
				upertime = time[i];
				break;
			}
			else if ((x_d < 0) && (x[0] >= x_d) && (x[i] <= x_d*0.9))
			{
				upertime = time[i];
				break;
			}
		}
		if (var == 'y')
		{
			if ((y_d >= 0) && (y[0] < y_d) && (y[i] >= y_d*0.9))
			{
				upertime = time[i];
				break;
			}
			else if ((y_d >= 0) && (y[0] >= y_d) && (y[i] <= y_d*0.9))
			{
				upertime = time[i];
				break;
			}

			else if ((y_d < 0) && (y[0] < y_d) && (y[i] >= y_d*0.9))
			{
				upertime = time[i];
				break;
			}
			else if ((y_d < 0) && (y[0] >= y_d) && (y[i] <= y_d*0.9))
			{
				upertime = time[i];
				break;
			}
		}
		if (var == 't')
		{
			if ((theta_d >= 0) && (theta[0] < theta_d) && (theta[i] >= theta_d*0.9))
			{
				upertime = time[i];
				break;
			}
			else if ((theta_d >= 0) && (theta[0] >= theta_d) && (theta[i] <= theta_d*0.9))
			{
				upertime = time[i];
				break;
			}

			else if ((theta_d < 0) && (theta[0] < theta_d) && (theta[i] >= theta_d*0.9))
			{
				upertime = time[i];
				break;
			}
			else if ((theta_d < 0) && (theta[0] >= theta_d) && (theta[i] <= theta_d*0.9))
			{
				upertime = time[i];
				break;
			}
		}
	}
	//cout << "tempo de subida = " << upertime << endl;
	return upertime;
}


// ------- MÉTODOS DE AJSUTE EM SÉRIE  ----------------------- //
/*Nesses métodos faz-se a análise dos 3 ganhos um depois do outro(SÉRIE) e leva-se em consideração os parâmetros a serem ajustados
em série, isto é, primeiro um, depois outro, por exemplo: tempo de assentamento, overshoot e tempo de subida */
//
//float controle_bili::improve_PID(robot entry, char var)
//{
//	// Setando as variáveis necessárias
//	float upertime, overshoot, ts;
//	float ts_prev, overshoot_prev, upertime_prev;   //prev é o valor anterior
//	int step_flag = 0;
//	float wanted_ts, wanted_overshoot, wanted_upertime;
//	switch (var)
//	{
//	case 'x':
//		ts_prev = find_ts('x', entry, entry.group_x.size());
//		overshoot_prev = find_overshoot('x', entry, entry.group_x.size());
//		upertime_prev = find_upertime('x', entry, entry.group_x.size());
//		break;
//	case 'y':
//		ts_prev = find_ts('y', entry, entry.group_y.size());
//		overshoot_prev = find_overshoot('y', entry, entry.group_y.size());
//		upertime_prev = find_upertime('y', entry, entry.group_y.size());
//		break;
//	case 't':
//		ts_prev = find_ts('t', entry, entry.group_theta.size());
//		overshoot_prev = find_overshoot('t', entry, entry.group_theta.size());
//		upertime_prev = find_upertime('t', entry, entry.group_theta.size());
//		break;
//	}
//
//	/*
//	TEMPO DE ASSENTAMENTO
//	*/
//
//	// -------------------- GANHO PROPORCIONAL ---------------------------------//
//
//	entry.gain[0] += 0.5;     // Começa subindo e com o passo alto
//	step_flag = 1;
//
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			ts = find_ts('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			ts = find_ts('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			ts = find_ts('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((ts_prev >= ts) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 0.5;
//		}
//		else if ((ts_prev >= ts) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 0.25;
//		}
//		else if ((ts_prev >= ts) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 0.01;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -0.5;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -0.25;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -0.01;
//		}
//		else if ((ts_prev >= ts) && (ts <= wanted_ts)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[0] >= 0)
//			{
//				entry.limit_gain[1] = entry.gain[0] * 1.1;
//				entry.limit_gain[0] = entry.gain[0] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[0] = entry.gain[0] * 1.1;
//				entry.limit_gain[1] = entry.gain[0] * 0.9;
//			}
//			break;
//		}
//		else if ((ts_prev < ts) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 2;
//		}
//		else if ((ts_prev < ts) && (step_flag == 2))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 3;
//		}
//		else if ((ts_prev < ts) && (step_flag == 3))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -1;
//		}
//		else if ((ts_prev < ts) && (step_flag == -1))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -2;
//		}
//		else if ((ts_prev < ts) && (step_flag == -2))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -3;
//		}
//		else if ((ts_prev < ts) && (step_flag == -3))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 1;
//		}
//		ts_prev = ts;
//	}
//
//	// -------------------- GANHO INTEGRAL ---------------------------------//
//	entry.gain[1] += 1.5;     // Começa subindo e com o passo alto
//	step_flag = 1;
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			ts = find_ts('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			ts = find_ts('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			ts = find_ts('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((ts_prev >= ts) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 1.5;
//		}
//		else if ((ts_prev >= ts) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 0.75;
//		}
//		else if ((ts_prev >= ts) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 0.01;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -1.5;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -0.75;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -0.01;
//		}
//		else if ((ts_prev >= ts) && (ts <= wanted_ts)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[1] >= 0)
//			{
//				entry.limit_gain[3] = entry.gain[1] * 1.1;
//				entry.limit_gain[2] = entry.gain[1] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[2] = entry.gain[1] * 1.1;
//				entry.limit_gain[3] = entry.gain[1] * 0.9;
//			}
//			break;
//		}
//		else if ((ts_prev < ts) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 2;
//		}
//		else if ((ts_prev < ts) && (step_flag == 2))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 3;
//		}
//		else if ((ts_prev < ts) && (step_flag == 3))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -1;
//		}
//		else if ((ts_prev < ts) && (step_flag == -1))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -2;
//		}
//		else if ((ts_prev < ts) && (step_flag == -2))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -3;
//		}
//		else if ((ts_prev < ts) && (step_flag == -3))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 1;
//		}
//		ts_prev = ts;
//	}
//
//	// -------------------- GANHO DERIVATIVO ---------------------------------//
//
//	entry.gain[2] += 0.5;     // Começa subindo e com o passo alto
//	step_flag = 1;
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			ts = find_ts('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			ts = find_ts('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			ts = find_ts('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((ts_prev >= ts) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 0.3;
//		}
//		else if ((ts_prev >= ts) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 0.15;
//		}
//		else if ((ts_prev >= ts) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 0.01;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -0.3;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -0.15;
//		}
//		else if ((ts_prev >= ts) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -0.01;
//		}
//		else if ((ts_prev >= ts) && (ts <= wanted_ts)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[2] >= 0)
//			{
//				entry.limit_gain[5] = entry.gain[2] * 1.1;
//				entry.limit_gain[4] = entry.gain[2] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[4] = entry.gain[2] * 1.1;
//				entry.limit_gain[5] = entry.gain[2] * 0.9;
//			}
//			break;
//		}
//		else if ((ts_prev < ts) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 2;
//		}
//		else if ((ts_prev < ts) && (step_flag == 2))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 3;
//		}
//		else if ((ts_prev < ts) && (step_flag == 3))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -1;
//		}
//		else if ((ts_prev < ts) && (step_flag == -1))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -2;
//		}
//		else if ((ts_prev < ts) && (step_flag == -2))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -3;
//		}
//		else if ((ts_prev < ts) && (step_flag == -3))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 1;
//		}
//		ts_prev = ts;
//	}
//
//	/*
//	OVERSHOOT
//	*/
//
//	// -------------------- GANHO PROPORCIONAL ---------------------------------//
//
//	entry.gain[0] += 1.2;     // Começa subindo e com o passo alto
//	step_flag = 1;
//
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			overshoot = find_overshoot('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			overshoot = find_overshoot('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			overshoot = find_overshoot('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((overshoot_prev >= overshoot) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 1.2;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 0.6;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 0.01;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -1.2;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -0.6;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -0.01;
//		}
//		else if ((overshoot_prev >= overshoot) && (overshoot <= wanted_overshoot)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[0] >= 0)
//			{
//				entry.limit_gain[1] = entry.gain[0] * 1.1;
//				entry.limit_gain[0] = entry.gain[0] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[0] = entry.gain[0] * 1.1;
//				entry.limit_gain[1] = entry.gain[0] * 0.9;
//			}
//			break;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 2;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 2))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 3;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 3))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -1;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -1))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -2;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -2))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -3;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -3))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 1;
//		}
//		overshoot_prev = overshoot;
//	}
//
//	// -------------------- GANHO INTEGRAL ---------------------------------//
//
//	entry.gain[1] += 0.8;     // Começa subindo e com o passo alto
//	step_flag = 1;
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			overshoot = find_overshoot('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			overshoot = find_overshoot('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			overshoot = find_overshoot('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((overshoot_prev >= overshoot) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 0.8;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 0.4;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 0.01;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -0.8;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -0.4;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -0.01;
//		}
//		else if ((overshoot_prev >= overshoot) && (overshoot <= wanted_overshoot)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[1] >= 0)
//			{
//				entry.limit_gain[3] = entry.gain[1] * 1.1;
//				entry.limit_gain[2] = entry.gain[1] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[2] = entry.gain[1] * 1.1;
//				entry.limit_gain[3] = entry.gain[1] * 0.9;
//			}
//			break;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 2;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 2))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 3;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 3))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -1;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -1))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -2;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -2))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -3;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -3))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 1;
//		}
//		overshoot_prev = overshoot;
//	}
//
//	// -------------------- GANHO DERIVATIVO ---------------------------------//
//
//	entry.gain[2] += 1.5;     // Começa subindo e com o passo alto
//	step_flag = 1;
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			overshoot = find_overshoot('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			overshoot = find_overshoot('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			overshoot = find_overshoot('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((overshoot_prev >= overshoot) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 1.5;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 0.75;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 0.01;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -1.5;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -0.75;
//		}
//		else if ((overshoot_prev >= overshoot) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -0.01;
//		}
//		else if ((overshoot_prev >= overshoot) && (overshoot <= wanted_overshoot)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[2] >= 0)
//			{
//				entry.limit_gain[5] = entry.gain[2] * 1.1;
//				entry.limit_gain[4] = entry.gain[2] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[4] = entry.gain[2] * 1.1;
//				entry.limit_gain[5] = entry.gain[2] * 0.9;
//			}
//			break;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 2;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 2))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 3;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == 3))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -1;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -1))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -2;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -2))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -3;
//		}
//		else if ((overshoot_prev < overshoot) && (step_flag == -3))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 1;
//		}
//		overshoot_prev = overshoot;
//	}
//
//	/*
//	TEMPO DE SUBIDA
//	*/
//
//	// -------------------- GANHO PROPORCIONAL ---------------------------------//
//
//	entry.gain[0] += 1.5;     // Começa subindo e com o passo alto
//	step_flag = 1;
//
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			upertime = find_upertime('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			upertime = find_upertime('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			upertime = find_upertime('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((upertime_prev >= upertime) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 1.5;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 0.75;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += 0.01;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -1.5;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -0.75;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[0] = entry.gain[0];
//			entry.gain[0] += -0.01;
//		}
//		else if ((upertime_prev >= upertime) && (upertime <= wanted_upertime)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[0] >= 0)
//			{
//				entry.limit_gain[1] = entry.gain[0] * 1.1;
//				entry.limit_gain[0] = entry.gain[0] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[0] = entry.gain[0] * 1.1;
//				entry.limit_gain[1] = entry.gain[0] * 0.9;
//			}
//			break;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 2;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 2))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 3;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 3))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -1;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -1))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -2;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -2))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = -3;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -3))
//		{
//			entry.gain[0] = entry.old_gain[0];
//			step_flag = 1;
//		}
//		upertime_prev = upertime;
//	}
//
//	// -------------------- GANHO INTEGRAL ---------------------------------//
//
//	entry.gain[1] += 1.5;     // Começa subindo e com o passo alto
//	step_flag = 1;
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			upertime = find_upertime('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			upertime = find_upertime('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			upertime = find_upertime('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((upertime_prev >= upertime) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 1.5;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 0.75;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += 0.01;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -1.5;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -0.75;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[1] = entry.gain[1];
//			entry.gain[1] += -0.01;
//		}
//		else if ((upertime_prev >= upertime) && (upertime <= wanted_upertime)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[1] >= 0)
//			{
//				entry.limit_gain[3] = entry.gain[1] * 1.1;
//				entry.limit_gain[2] = entry.gain[1] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[2] = entry.gain[1] * 1.1;
//				entry.limit_gain[3] = entry.gain[1] * 0.9;
//			}
//			break;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 2;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 2))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 3;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 3))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -1;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -1))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -2;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -2))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = -3;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -3))
//		{
//			entry.gain[1] = entry.old_gain[1];
//			step_flag = 1;
//		}
//		upertime_prev = upertime;
//	}
//
//	// -------------------- GANHO DERIVATIVO ---------------------------------//
//
//	entry.gain[2] += 0.3;     // Começa subindo e com o passo alto
//	step_flag = 1;
//	while (true)
//	{
//		/**testing ->>> é necessário fazer de alguma forma q de pra testar antes de executar a parte abaixo**/
//		switch (var)
//		{
//		case 'x':
//			upertime = find_upertime('x', entry, entry.group_x.size());
//			break;
//		case 'y':
//			upertime = find_upertime('y', entry, entry.group_y.size());
//			break;
//		case 't':
//			upertime = find_upertime('t', entry, entry.group_theta.size());
//			break;
//		}
//		if ((upertime_prev >= upertime) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 0.3;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 0.15;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += 0.01;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -0.3;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -0.15;
//		}
//		else if ((upertime_prev >= upertime) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
//		{
//			entry.old_gain[2] = entry.gain[2];
//			entry.gain[2] += -0.01;
//		}
//		else if ((upertime_prev >= upertime) && (upertime <= wanted_upertime)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
//		{
//			if (entry.gain[2] >= 0)
//			{
//				entry.limit_gain[5] = entry.gain[2] * 1.1;
//				entry.limit_gain[4] = entry.gain[2] * 0.9;
//			}
//			else
//			{
//				entry.limit_gain[4] = entry.gain[2] * 1.1;
//				entry.limit_gain[5] = entry.gain[2] * 0.9;
//			}
//			break;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 1))   //n melhorou, logo muda
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 2;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 2))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 3;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == 3))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -1;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -1))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -2;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -2))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = -3;
//		}
//		else if ((upertime_prev < upertime) && (step_flag == -3))
//		{
//			entry.gain[2] = entry.old_gain[2];
//			step_flag = 1;
//		}
//		upertime_prev = upertime;
//	}
//}
//


int controle_bili::improve_p_ts(robot entry, char var, float wanted_ts, int step_flag)
{
	float ts, ts_prev;
	switch (var)
	{
	case 'x':
		ts_prev = entry.condition_x[3];
		ts = entry.condition_x[0];
		break;
	case 'y':
		ts_prev = entry.condition_y[3];
		ts = entry.condition_y[0];
		break;
	case 't':
		ts_prev = entry.condition_theta[3];
		ts = entry.condition_theta[0];
		break;
	}
	if ((ts_prev >= ts) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 0.5;
	}
	else if ((ts_prev >= ts) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 0.25;
	}
	else if ((ts_prev >= ts) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 0.01;
	}
	else if ((ts_prev >= ts) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -0.5;
	}
	else if ((ts_prev >= ts) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -0.25;
	}
	else if ((ts_prev >= ts) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -0.01;
	}
	else if ((ts_prev >= ts) && (ts <= wanted_ts)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[0] >= 0)
		{
			entry.limit_gain[1] = entry.gain[0] * 1.1;
			entry.limit_gain[0] = entry.gain[0] * 0.9;
		}
		else
		{
			entry.limit_gain[0] = entry.gain[0] * 1.1;
			entry.limit_gain[1] = entry.gain[0] * 0.9;
		}
		step_flag = 4;
	}
	else if ((ts_prev < ts) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 2;
	}
	else if ((ts_prev < ts) && (step_flag == 2))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 3;
	}
	else if ((ts_prev < ts) && (step_flag == 3))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -1;
	}
	else if ((ts_prev < ts) && (step_flag == -1))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -2;
	}
	else if ((ts_prev < ts) && (step_flag == -2))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -3;
	}
	else if ((ts_prev < ts) && (step_flag == -3))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 1;
	}
	ts_prev = ts;
	return step_flag;
}
int controle_bili::improve_i_ts(robot entry, char var, float wanted_ts, int step_flag)
{
	float ts, ts_prev;
	switch (var)
	{
	case 'x':
		ts_prev = entry.condition_x[3];
		ts = entry.condition_x[0];
		break;
	case 'y':
		ts_prev = entry.condition_y[3];
		ts = entry.condition_y[0];
		break;
	case 't':
		ts_prev = entry.condition_theta[3];
		ts = entry.condition_theta[0];
		break;
	}
	if ((ts_prev >= ts) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 1.5;
	}
	else if ((ts_prev >= ts) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 0.75;
	}
	else if ((ts_prev >= ts) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 0.01;
	}
	else if ((ts_prev >= ts) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -1.5;
	}
	else if ((ts_prev >= ts) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -0.75;
	}
	else if ((ts_prev >= ts) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -0.01;
	}
	else if ((ts_prev >= ts) && (ts <= wanted_ts)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[1] >= 0)
		{
			entry.limit_gain[3] = entry.gain[1] * 1.1;
			entry.limit_gain[2] = entry.gain[1] * 0.9;
		}
		else
		{
			entry.limit_gain[2] = entry.gain[1] * 1.1;
			entry.limit_gain[3] = entry.gain[1] * 0.9;
		}
		step_flag = 4;
	}
	else if ((ts_prev < ts) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 2;
	}
	else if ((ts_prev < ts) && (step_flag == 2))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 3;
	}
	else if ((ts_prev < ts) && (step_flag == 3))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -1;
	}
	else if ((ts_prev < ts) && (step_flag == -1))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -2;
	}
	else if ((ts_prev < ts) && (step_flag == -2))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -3;
	}
	else if ((ts_prev < ts) && (step_flag == -3))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 1;
	}
	ts_prev = ts;
	return step_flag;
}
int controle_bili::improve_d_ts(robot entry, char var, float wanted_ts, int step_flag)
{
	float ts, ts_prev;
	switch (var)
	{
	case 'x':
		ts_prev = entry.condition_x[3];
		ts = entry.condition_x[0];
		break;
	case 'y':
		ts_prev = entry.condition_y[3];
		ts = entry.condition_y[0];
		break;
	case 't':
		ts_prev = entry.condition_theta[3];
		ts = entry.condition_theta[0];
		break;
	}
	if ((ts_prev >= ts) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 0.3;
	}
	else if ((ts_prev >= ts) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 0.15;
	}
	else if ((ts_prev >= ts) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 0.01;
	}
	else if ((ts_prev >= ts) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -0.3;
	}
	else if ((ts_prev >= ts) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -0.15;
	}
	else if ((ts_prev >= ts) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -0.01;
	}
	else if ((ts_prev >= ts) && (ts <= wanted_ts)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[2] >= 0)
		{
			entry.limit_gain[5] = entry.gain[2] * 1.1;
			entry.limit_gain[4] = entry.gain[2] * 0.9;
		}
		else
		{
			entry.limit_gain[4] = entry.gain[2] * 1.1;
			entry.limit_gain[5] = entry.gain[2] * 0.9;
		}
		step_flag = 4;
	}
	else if ((ts_prev < ts) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 2;
	}
	else if ((ts_prev < ts) && (step_flag == 2))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 3;
	}
	else if ((ts_prev < ts) && (step_flag == 3))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -1;
	}
	else if ((ts_prev < ts) && (step_flag == -1))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -2;
	}
	else if ((ts_prev < ts) && (step_flag == -2))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -3;
	}
	else if ((ts_prev < ts) && (step_flag == -3))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 1;
	}
	ts_prev = ts;
	return step_flag;
}

int controle_bili::improve_p_overshoot(robot entry, char var, float wanted_overshoot, int step_flag)
{
	float overshoot, overshoot_prev;
	switch (var)
	{
	case 'x':
		overshoot_prev = entry.condition_x[4];
		overshoot = entry.condition_x[1];
		break;
	case 'y':
		overshoot_prev = entry.condition_y[4];
		overshoot = entry.condition_y[1];
		break;
	case 't':
		overshoot = entry.condition_theta[4];
		overshoot = entry.condition_theta[1];
		break;
	}
	if ((overshoot_prev >= overshoot) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 1.2;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 0.6;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 0.01;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -1.2;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -0.6;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -0.01;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (overshoot <= wanted_overshoot)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[0] >= 0)
		{
			float old_limit_sup = entry.limit_gain[1];
			float old_limit_inf = entry.limit_gain[0];

			entry.limit_gain[1] = entry.gain[0] * 1.1;
			entry.limit_gain[0] = entry.gain[0] * 0.9;

			limiter(entry.limit_gain[1], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[0], old_limit_sup, old_limit_inf);
		}
		else
		{
			float old_limit_sup = entry.limit_gain[0];
			float old_limit_inf = entry.limit_gain[1];

			entry.limit_gain[0] = entry.gain[0] * 1.1;
			entry.limit_gain[1] = entry.gain[0] * 0.9;

			limiter(entry.limit_gain[1], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[0], old_limit_sup, old_limit_inf);
		}
		step_flag = 4;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 2;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 2))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 3;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 3))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -1;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -1))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -2;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -2))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -3;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -3))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 1;
	}
	overshoot_prev = overshoot;
	return step_flag;
}
int controle_bili::improve_i_overshoot(robot entry, char var, float wanted_overshoot, int step_flag)
{
	float overshoot, overshoot_prev;
	switch (var)
	{
	case 'x':
		overshoot_prev = entry.condition_x[4];
		overshoot = entry.condition_x[1];
		break;
	case 'y':
		overshoot_prev = entry.condition_y[4];
		overshoot = entry.condition_y[1];
		break;
	case 't':
		overshoot = entry.condition_theta[4];
		overshoot = entry.condition_theta[1];
		break;
	}
	if ((overshoot_prev >= overshoot) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 0.8;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 0.4;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}

	}
	else if ((overshoot_prev >= overshoot) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 0.01;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -0.8;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -0.4;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -0.01;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (overshoot <= wanted_overshoot)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[1] >= 0)
		{
			float old_limit_sup = entry.limit_gain[3];
			float old_limit_inf = entry.limit_gain[2];

			entry.limit_gain[3] = entry.gain[1] * 1.1;
			entry.limit_gain[2] = entry.gain[1] * 0.9;

			limiter(entry.limit_gain[3], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[2], old_limit_sup, old_limit_inf);
		}
		else
		{
			float old_limit_sup = entry.limit_gain[2];
			float old_limit_inf = entry.limit_gain[3];

			entry.limit_gain[2] = entry.gain[1] * 1.1;
			entry.limit_gain[3] = entry.gain[1] * 0.9;

			limiter(entry.limit_gain[3], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[2], old_limit_sup, old_limit_inf);
		}
		step_flag = 4;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 2;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 2))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 3;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 3))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -1;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -1))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -2;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -2))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -3;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -3))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 1;
	}
	overshoot_prev = overshoot;
	return step_flag;
}
int controle_bili::improve_d_overshoot(robot entry, char var, float wanted_overshoot, int step_flag)
{
	float overshoot, overshoot_prev;
	switch (var)
	{
	case 'x':
		overshoot_prev = entry.condition_x[4];
		overshoot = entry.condition_x[1];
		break;
	case 'y':
		overshoot_prev = entry.condition_y[4];
		overshoot = entry.condition_y[1];
		break;
	case 't':
		overshoot = entry.condition_theta[4];
		overshoot = entry.condition_theta[1];
		break;
	}
	if ((overshoot_prev >= overshoot) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 1.5;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 0.75;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 0.01;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -1.5;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -0.75;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -0.01;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((overshoot_prev >= overshoot) && (overshoot <= wanted_overshoot)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[2] >= 0)
		{
			float old_limit_sup = entry.limit_gain[5];
			float old_limit_inf = entry.limit_gain[4];

			entry.limit_gain[5] = entry.gain[2] * 1.1;
			entry.limit_gain[4] = entry.gain[2] * 0.9;

			limiter(entry.limit_gain[5], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[4], old_limit_sup, old_limit_inf);
		}
		else
		{
			float old_limit_sup = entry.limit_gain[4];
			float old_limit_inf = entry.limit_gain[5];

			entry.limit_gain[4] = entry.gain[2] * 1.1;
			entry.limit_gain[5] = entry.gain[2] * 0.9;

			limiter(entry.limit_gain[5], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[4], old_limit_sup, old_limit_inf);
		}
		step_flag = 4;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 2;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 2))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 3;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == 3))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -1;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -1))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -2;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -2))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -3;
	}
	else if ((overshoot_prev < overshoot) && (step_flag == -3))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 1;
	}
	overshoot_prev = overshoot;
	return step_flag;
}

int controle_bili::improve_p_upertime(robot entry, char var, float wanted_upertime, int step_flag)
{
	float upertime, upertime_prev;
	switch (var)
	{
	case 'x':
		upertime_prev = entry.condition_x[5];
		upertime = entry.condition_x[2];
		break;
	case 'y':
		upertime_prev = entry.condition_y[5];
		upertime = entry.condition_y[2];
		break;
	case 't':
		upertime_prev = entry.condition_theta[5];
		upertime = entry.condition_theta[2];
		break;
	}
	if ((upertime_prev >= upertime) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 1.5;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 0.75;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += 0.01;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -1.5;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -0.75;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[0] = entry.gain[0];
		entry.gain[0] += -0.01;
		if (entry.gain[0] >= 0)
		{
			limiter(entry.gain[0], entry.limit_gain[1], entry.limit_gain[0]);
		}
		else
		{
			limiter(entry.gain[0], entry.limit_gain[0], entry.limit_gain[1]);
		}
	}
	else if ((upertime_prev >= upertime) && (upertime <= wanted_upertime)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[0] >= 0)
		{
			float old_limit_sup = entry.limit_gain[1];
			float old_limit_inf = entry.limit_gain[0];

			entry.limit_gain[1] = entry.gain[0] * 1.1;
			entry.limit_gain[0] = entry.gain[0] * 0.9;

			limiter(entry.limit_gain[1], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[0], old_limit_sup, old_limit_inf);
		}
		else
		{
			float old_limit_sup = entry.limit_gain[0];
			float old_limit_inf = entry.limit_gain[1];

			entry.limit_gain[0] = entry.gain[0] * 1.1;
			entry.limit_gain[1] = entry.gain[0] * 0.9;

			limiter(entry.limit_gain[1], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[0], old_limit_sup, old_limit_inf);
		}
		step_flag = 4;
	}
	else if ((upertime_prev < upertime) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 2;
	}
	else if ((upertime_prev < upertime) && (step_flag == 2))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 3;
	}
	else if ((upertime_prev < upertime) && (step_flag == 3))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -1;
	}
	else if ((upertime_prev < upertime) && (step_flag == -1))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -2;
	}
	else if ((upertime_prev < upertime) && (step_flag == -2))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = -3;
	}
	else if ((upertime_prev < upertime) && (step_flag == -3))
	{
		entry.gain[0] = entry.old_gain[0];
		step_flag = 1;
	}
	upertime_prev = upertime;
	return step_flag;
}
int controle_bili::improve_i_upertime(robot entry, char var, float wanted_upertime, int step_flag)
{
	float upertime, upertime_prev;
	switch (var)
	{
	case 'x':
		upertime_prev = entry.condition_x[5];
		upertime = entry.condition_x[2];
		break;
	case 'y':
		upertime_prev = entry.condition_y[5];
		upertime = entry.condition_y[2];
		break;
	case 't':
		upertime_prev = entry.condition_theta[5];
		upertime = entry.condition_theta[2];
		break;
	}
	if ((upertime_prev >= upertime) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 1.5;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}

	}
	else if ((upertime_prev >= upertime) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 0.75;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}

	}
	else if ((upertime_prev >= upertime) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += 0.01;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}

	}
	else if ((upertime_prev >= upertime) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -1.5;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}

	}
	else if ((upertime_prev >= upertime) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -0.75;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}

	}
	else if ((upertime_prev >= upertime) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[1] = entry.gain[1];
		entry.gain[1] += -0.01;
		if (entry.gain[1] >= 0)
		{
			limiter(entry.gain[1], entry.limit_gain[3], entry.limit_gain[2]);
		}
		else
		{
			limiter(entry.gain[1], entry.limit_gain[2], entry.limit_gain[3]);
		}

	}
	else if ((upertime_prev >= upertime) && (upertime <= wanted_upertime)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[1] >= 0)
		{
			float old_limit_sup = entry.limit_gain[3];
			float old_limit_inf = entry.limit_gain[2];

			entry.limit_gain[3] = entry.gain[1] * 1.1;
			entry.limit_gain[2] = entry.gain[1] * 0.9;

			limiter(entry.limit_gain[3], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[2], old_limit_sup, old_limit_inf);
		}
		else
		{
			float old_limit_sup = entry.limit_gain[2];
			float old_limit_inf = entry.limit_gain[3];

			entry.limit_gain[2] = entry.gain[1] * 1.1;
			entry.limit_gain[3] = entry.gain[1] * 0.9;

			limiter(entry.limit_gain[3], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[2], old_limit_sup, old_limit_inf);
		}
		step_flag = 4;
	}
	else if ((upertime_prev < upertime) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 2;
	}
	else if ((upertime_prev < upertime) && (step_flag == 2))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 3;
	}
	else if ((upertime_prev < upertime) && (step_flag == 3))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -1;
	}
	else if ((upertime_prev < upertime) && (step_flag == -1))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -2;
	}
	else if ((upertime_prev < upertime) && (step_flag == -2))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = -3;
	}
	else if ((upertime_prev < upertime) && (step_flag == -3))
	{
		entry.gain[1] = entry.old_gain[1];
		step_flag = 1;
	}
	upertime_prev = upertime;
	return step_flag;
}
int controle_bili::improve_d_upertime(robot entry, char var, float wanted_upertime, int step_flag)
{
	float upertime, upertime_prev;
	switch (var)
	{
	case 'x':
		upertime_prev = entry.condition_x[5];
		upertime = entry.condition_x[2];
		break;
	case 'y':
		upertime_prev = entry.condition_y[5];
		upertime = entry.condition_y[2];
		break;
	case 't':
		upertime_prev = entry.condition_theta[5];
		upertime = entry.condition_theta[2];
		break;
	}
	if ((upertime_prev >= upertime) && (step_flag == 1))       //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 0.3;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == 2))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 0.15;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == 3))  //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += 0.01;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == -1)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -0.3;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == -2)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -0.15;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((upertime_prev >= upertime) && (step_flag == -3)) //melhorou, mas ainda n chegou no desejado logo continua
	{
		entry.old_gain[2] = entry.gain[2];
		entry.gain[2] += -0.01;
		if (entry.gain[2] >= 0)
		{
			limiter(entry.gain[2], entry.limit_gain[5], entry.limit_gain[4]);
		}
		else
		{
			limiter(entry.gain[2], entry.limit_gain[4], entry.limit_gain[5]);
		}
	}
	else if ((upertime_prev >= upertime) && (upertime <= wanted_upertime)) //melhorou e está dentro do valor desejado, logo para o loop e devolveo ganho
	{
		if (entry.gain[2] >= 0)
		{
			float old_limit_sup = entry.limit_gain[5];
			float old_limit_inf = entry.limit_gain[4];

			entry.limit_gain[5] = entry.gain[2] * 1.1;
			entry.limit_gain[4] = entry.gain[2] * 0.9;

			limiter(entry.limit_gain[5], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[4], old_limit_sup, old_limit_inf);
		}
		else
		{
			float old_limit_sup = entry.limit_gain[4];
			float old_limit_inf = entry.limit_gain[5];

			entry.limit_gain[4] = entry.gain[2] * 1.1;
			entry.limit_gain[5] = entry.gain[2] * 0.9;

			limiter(entry.limit_gain[5], old_limit_sup, old_limit_inf);
			limiter(entry.limit_gain[4], old_limit_sup, old_limit_inf);
		}
		step_flag = 4;
	}
	else if ((upertime_prev < upertime) && (step_flag == 1))   //n melhorou, logo muda
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 2;
	}
	else if ((upertime_prev < upertime) && (step_flag == 2))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 3;
	}
	else if ((upertime_prev < upertime) && (step_flag == 3))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -1;
	}
	else if ((upertime_prev < upertime) && (step_flag == -1))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -2;
	}
	else if ((upertime_prev < upertime) && (step_flag == -2))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = -3;
	}
	else if ((upertime_prev < upertime) && (step_flag == -3))
	{
		entry.gain[2] = entry.old_gain[2];
		step_flag = 1;
	}
	upertime_prev = upertime;
	return step_flag;
}

