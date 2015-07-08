#include "controle_bili.h"
#include "iostream"
#include <stdio.h>
#include <string>
#include"../serial/serial.h"
#include"../vision/vision.h"
#include"../robot/robot.h"
#include <time.h>
#include "Windows.h"
using namespace cvb;
using namespace std;




#define M_PI 3.14159265358979323846

char char_pressed = cvWaitKey(1);

void controle_bili::ajuste_distancia(float xd, float yd){
	
	int dist_orig = sqrt(pow(xd - x, 2) + pow(yd - y, 2));
	//cout << "DIST ORIG: " << dist_orig << endl;

	if (dist_orig < 26) //26
	{
		dist = dist_orig;
		n_xd = xd;
		n_yd = yd;		
	}
	else
	{
		cout << "x = " << x << " y = " << y << " xd = " << xd << " yd = " << yd << endl;
		n_xd = x + 25*(xd - x) / dist_orig;
		n_yd = y + 25*(yd - y) / dist_orig;
		cout << "n_xd = " << n_xd << " n_yd = " << n_yd << endl;
		dist = sqrt(pow(x - n_xd, 2) + pow(y - n_yd, 2));
		//cout << "ENTROU AQUI" << endl;
	}

	cout << "DIST: " << dist << endl;
}


void controle_bili::ajuste_referencia(float x, float y, float xd, float yd){
	dist = sqrt(pow(x - n_xd, 2) + pow(y - n_yd, 2));

	/*cout << "xd = " << xd << endl;
	cout << "yd = " << yd << endl;
	cout << "x = " << x << endl;
	cout << "y = " << y << endl;*/

	

	refaux = atan2(yd - y, xd - x);


	if (x == xd && y == yd)
	{
		thetad = theta;
	}
	else
	{
		thetad = refaux;

	}
	
	
	
	
	//cout << "refaux = " << refaux << endl;
	

}

void controle_bili::controle(int x, int y, int xd, int yd, float w){

	float delta = 0.0003;


	//--------------------ANGULAR-------------------------------//

	/////////////////////////////// DERIVADA SUJA DE THETA

	aux_theta = (delta / (delta + h))*aux_old_theta + (h / (delta + h))*dif_ang;

	deriv_theta = (-1 / delta)*aux_theta + (1 / delta)*dif_ang;
	
	////////////////////////////////////////////////////////////////

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

		//ang_error = thetad - theta;
		//dif_ang = ang_error;


		if (ang_error <= M_PI/2 && ang_error >= -M_PI/2){

			dif_ang = ang_error;
			flag_dir = 1;
			
		}
		else
		{
			
			flag_dir = -1;
			if (ang_error > M_PI / 2 && ang_error <= M_PI )
			{
				dif_ang = -(M_PI - ang_error);
				
				//cout << "ang_error = " << ang_error*180/M_PI << endl;
			}
			else //if (ang_error > -M_PI  && ang_error < -M_PI/2)
			{
				dif_ang = (M_PI + ang_error);
				
			}
		}

		if (dif_ang >= M_PI/2)
		{
			dif_ang = M_PI / 2;
		}
		else if (dif_ang <= -M_PI / 2)
		{
			dif_ang = -M_PI / 2;
		}

		dif_ang = -M_PI / 2 - theta;
		
		KI_w = 0.5;//0.4;//0.2;//1.4
		w_i = w_i + h*KI_w*(dif_ang);



		if (w_i >= 10)
		{
			w_i = 10;
		}

		else if (w_i <= -10)
		{
			w_i = 10;
		}

		else if (abs(dif_ang) <= 2*M_PI/180)
		{

			w_i = 0.0;

		}

		

		KD_w = 0.8;//16;
		w_d = KD_u *((dif_ang - dif_ang_old) / h*h);

		KP_w = 45;//35;//20//*(-0.0288 * (pow((abs(deriv_theta) + delta), 2)) + 0.151 * (abs(deriv_theta) + delta) + 59.215);
		//(abs(deriv_theta) + delta);//12
		w_p = KP_w*(dif_ang);


		

		cout << "//////////////////////////////////////////////DIF ANGULO: " << KD_w*deriv_theta << endl;
		cout << "//////////////////////////////////////////////W: " << w_d  << endl;

		//--------------------------------LINEAR-------------------------------//

		//////////////////////// DERIVADA SUJA DE X

		aux_x = (delta / (delta + h))*aux_old_x + (h / (delta + h))*x;

		deriv_x = (-1 / delta)*aux_x + (1 / delta)*x;

		//////////////////////// DERIVADA SUJA DE Y

		aux_y = (delta / (delta + h))*aux_old_y + (h / (delta + h))*y;

		deriv_y = (-1 / delta)*aux_y + (1 / delta)*y;

		vel = sqrt(pow(deriv_x, 2) + pow(deriv_x, 2));

		//////////////////////////////////////////////////////////////////////////
		
		
		

		

		KI_u = flag_dir*0.0;//0.02
		u_i = u_i + h*KI_u*dist;

		KP_u = flag_dir *0.36;//(7/ (2 * abs(vel) + 10));//0.2     3./(d*2 + 10);
		u_p = KP_u*dist;

		cout << "//////////////////////////////////////////////DIST: " << dist << endl;
		cout << "//////////////////////////////////////////////VEL: " << vel << endl;


		KD_u = flag_dir *3.4;//flag_dir * 300/(2*dist + 10 );//1
		u_d = KD_u *((dist - dist_old) / h*h);

		//u = u_p + u_i + u_d;

		if (u_i >= 5)
		{
			u_i = 0;
		}

		else if (u_i <= -5)
		{
			u_i = -0;
		}
		if (abs(xd-x) <= 0.2)
		{
			u_i = 0.0;
		}
		if (pow(x - xd, 2) + pow(y - yd, 2) <= 140){
			//dist = 0.0;
			u_i = 0.0;
			//dif_ang = 0.0;
			w_i = 0.0;
			//u_p = 0;//-flag_dir;
			w_p = 0;
			//dif_ang = -M_PI / 2 - theta;
			u_d = 0;
		}

		if (w_i == 3 && u_i == 2 || w_i == -3 && u_i == -2 || w_i == 3 && u_i == -2 || w_i == -3 && u_i == 2)
		{
			w_i = 0;
			u_i = 0;
		}


		w_teste = w_p + w_i + w_d;
		u = u_p + u_i + u_d;
		//u = 0;
		//w = -1;

		cout << "ang_error = " << ang_error * 180 / M_PI << endl;
		cout << "dif_ang = " << dif_ang * 180 / M_PI << endl;
		cout << "u = " << u << endl;
		cout << "w = " << w << endl;
		cout << "flag_dir = " << flag_dir << endl;

}

void controle_bili::modelagem(float w){
	


	

	v1 = ((0.5)*u - (w_teste*0.5) / 3.25) / 10.0;
	v2 = ((0.5)*u + w_teste*0.5 / 3.25) / 10.0;
	
	
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

void controle_bili::rotina_controle(int num_carro, float e, vision* Camera, int xd, int yd, float w){

	
	
	x = Camera->players[num_carro].x - 155;
	y = -(Camera->players[num_carro].y - 116);

	//xd = Camera.players[num_carro].goal_x - 155;
	//yd = -(Camera.players[num_carro].goal_y - 116);
	//xd = xd - 155;
	//yd = -(yd - 116);

	theta = -Camera->players[num_carro].theta;
	

	ajuste_distancia(xd, yd);
	
	ajuste_referencia(x, y, n_xd, n_yd);

	controle(x, y, n_xd , n_yd,w);
	
	modelagem(w);

	if (Camera->players[num_carro].x == -1)
	{
		v1 = 0.0;
		v2 = 0.0;
		u_i = 0.0;
		w_i = 0.0;
		u = 0.0;
		w = 0.0;

	
	}
	dif_ang_old = dif_ang;
	ang_error_old = ang_error;
	dist_old = dist;
	x_old = x;
	y_old = y;
	thetad_old = theta;
	aux_old_x = aux_x;
	aux_old_y = aux_y;
	aux_old_theta = aux_theta;
}




