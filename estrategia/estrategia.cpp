#include "estrategia.h"


estrategia::estrategia(int nh, int nw)
{
	reduce = 0.25;

	height = ceil(reduce * nh);
	width = ceil(reduce * nw);


	field = cv::Mat::zeros(height, width, CV_8UC1);
	closed = Mat(height, width, CV_32SC1, Scalar(0));
	expand = Mat(height, width, CV_32SC1, Scalar(0));

	for (int i = 0; i < 3; i++)
	{
		fieldI[i] = cv::Mat::zeros(height, width, CV_8UC1);
		closedI[i] = Mat(height, width, CV_32SC1, Scalar(0));
		expandI[i] = Mat(height, width, CV_32SC1, Scalar(0));
	}
	//cv::circle(field, cvPoint(30, 50), 20, 128, -1);

	for (int i = 0; i < 3; i++)
	{
		path[i].clear();
	}


	y_min = 0;
	y_max = height-1;

	x_min = 0;
	x_max = width-1;

	side = 0;

	for (int i = 0; i < 3; i++)
		players[i].comportamento = i;
	comportamento1_old = 2;
	comportamento2_old = 3;
}

estrategia::estrategia(int nh, int nw, int nside, robot canto_gol1, robot canto_gol2, robot canto_area1, robot canto_area2)
{
	reduce = 0.25;

	height = ceil(reduce * nh);
	width = ceil(reduce * nw);


	field = cv::Mat::zeros(height, width, CV_8UC1);
	closed = Mat(height, width, CV_32SC1, Scalar(0));
	expand = Mat(height, width, CV_32SC1, Scalar(0));

	for (int i = 0; i < 3; i++)
	{
		fieldI[i] = cv::Mat::zeros(height, width, CV_8UC1);
		closedI[i] = Mat(height, width, CV_32SC1, Scalar(0));
		expandI[i] = Mat(height, width, CV_32SC1, Scalar(0));
	}
	//cv::circle(field, cvPoint(30, 50), 20, 128, -1);

	for (int i = 0; i < 3; i++)
	{
		path[i].clear();
	}

	x_gol_min = min(canto_gol1.x, canto_gol2.x);
	x_gol_max = max(canto_gol1.x, canto_gol2.x);
	y_gol_min = min(canto_gol1.y, canto_gol2.y);
	y_gol_max = max(canto_gol1.y, canto_gol2.y);

	x_area_gol1 = min(canto_area1.x, canto_area2.x);
	x_area_gol2 = max(canto_area1.x, canto_area2.x);
	y_area_gol_min = min(canto_area1.y, canto_area2.y);;
	y_area_gol_max = max(canto_area1.y, canto_area2.y);


	y_min = 0;
	y_max = height - 1;

	x_min = 0;
	x_max = width - 1;

	side = nside;

	for (int i = 0; i < 3; i++)
		players[i].comportamento = i;
	comportamento1_old = 2;
	comportamento2_old = 3;


	weight_data = 0.5;
	weight_smooth = 0.2;

	raio = 13;
}

estrategia::~estrategia()
{


}

void estrategia::Config(int nh, int nw)
{
	//cout << "Entre com seu lado do campo" << endl;
	//cout << "OBS: Lado = 1 x próximo de 0 eh o seu gol" << endl;
	//cin >> side;
	//cout << "Entre com o x minimo do gol" << endl;
	//cin >> x_gol_min;
	//cout << "Entre com o y minimo do gol" << endl;
	//cin >> y_gol_min;
	//cout << "Entre com o x minimo do campo" << endl;
	//cin >> x_min;
	//cout << "Entre com o y minimo do campo" << endl;
	//cin >> y_min;
	//cout << "Entre com o x maximo do gol" << endl;
	//cin >> x_gol_max;
	//cout << "Entre com o y maximo do gol" << endl;
	//cin >> y_gol_max;
	//cout << "Entre com o x maximo do campo" << endl;
	//cin >> x_max;
	//cout << "Entre com o y maximo do campo" << endl;
	//cin >> y_max;

	height = (nh *reduce);
	width = (nw * reduce);

	x_limite1 = 3 * (x_gol_max + x_gol_min) / 4;
	x_limite2 = (x_gol_max + x_gol_min) / 4;

	meio_campo.x = nw / 2;
	meio_campo.y = nh / 2;


	ponto_gol.y = (y_gol_max + y_gol_min) / 2;
	ponto_gol_adv.y = (y_gol_max + y_gol_min) / 2;

	if (side == 1)
	{
		ponto_gol.x = x_gol_min + 10;
		ponto_gol_adv.x = x_gol_max;
	}
	else
	{
		ponto_gol.x = x_gol_max - 10;
		ponto_gol_adv.x = x_gol_min;
	}
}

void estrategia::ConfigAreas(robot canto_gol1, robot canto_gol2, robot canto_area1, robot canto_area2)
{
	x_gol_min = min(canto_gol1.x, canto_gol2.x);
	x_gol_max = max(canto_gol1.x, canto_gol2.x);
	y_gol_min = min(canto_gol1.y, canto_gol2.y);
	y_gol_max = max(canto_gol1.y, canto_gol2.y);

	x_area_gol1 = min((int)canto_area1.x, (int)canto_area2.x);
	x_area_gol2 = max((int)canto_area1.x, (int)canto_area2.x);

	//if (canto_area1.x < canto_area2.x)
	//{
	//	x_area_gol1 = (int)canto_area1.x;
	//	x_area_gol2 = (int)canto_area2.x;
	//}
	//else
	//{
	//	x_area_gol1 = (int)canto_area2.x;
	//	x_area_gol2 = (int)canto_area1.x;
	//}

	y_area_gol_min = min(canto_area1.y, canto_area2.y);;
	y_area_gol_max = max(canto_area1.y, canto_area2.y);
}

void estrategia::update(robot* new_players)
{
	for (int i = 0; i < 7; i++)
		players[i] = new_players[i];
}

void estrategia::ConfigWeight(int* newweight)
{
	adv_radius = newweight[0];
	adv_weight = newweight[1];
	ball_radius = newweight[2];
	ball_weight = newweight[3];
	ball_h = newweight[4];
	ball_w = newweight[5];
	player_radius = newweight[6];
	weight_data = (float) newweight[7] / 100;
	weight_smooth = (float) newweight[8] / 100;
	raio = newweight[9];
}

// -----------------COMPORTAMENTO ------------//

void estrategia::comportamento()
{
	/*----------------------------------------  SETANDO AS DISTANCIAS  ----------------------------------------------*/
	Cria_distancia();
	/*-------------------------------------------  COMPORTAMETNTOS  -------------------------------------------------*/

	//Comportamento = 1 --> Goleiro
	//Comportamento = 2 --> Atacante
	//Comportamento = 3 --> Lateral

	/*------------------------------------------  SEM POSSE DE BOLA  ------------------------------------------------*/
	if ((players[1].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[2].distancia_gol) && (players[0].posse != 1) && (players[1].posse != 1) && (players[2].posse != 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[0].distancia_gol) && (players[0].posse != 1) && (players[1].posse != 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}

	/*------------------------------------------------  COM POSSE DE BOLA  -------------------------------------------------*/

	else if ((players[0].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[2].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse != 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[0].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[2].distancia_gol) && (players[0].posse = 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[0].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[2].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[0].distancia_gol < players[2].distancia_gol) && (players[2].distancia_gol < players[1].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[0].distancia_gol < players[2].distancia_gol) && (players[2].distancia_gol < players[1].distancia_gol) && (players[0].posse = 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[0].distancia_gol < players[2].distancia_gol) && (players[2].distancia_gol < players[1].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[2].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse != 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[2].distancia_gol) && (players[0].posse != 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[2].distancia_gol) && (players[0].posse = 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[2].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[2].distancia_gol) && (players[2].distancia_gol < players[0].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse != 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[2].distancia_gol) && (players[2].distancia_gol < players[0].distancia_gol) && (players[0].posse != 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[2].distancia_gol) && (players[2].distancia_gol < players[0].distancia_gol) && (players[0].posse != 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[2].distancia_gol) && (players[2].distancia_gol < players[0].distancia_gol) && (players[0].posse = 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[1].distancia_gol < players[2].distancia_gol) && (players[2].distancia_gol < players[0].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[1].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[1].distancia_gol) && (players[0].posse != 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[1].distancia_gol) && (players[0].posse = 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[0].distancia_gol) && (players[0].distancia_gol < players[1].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[0].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[0].distancia_gol) && (players[0].posse != 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[0].distancia_gol) && (players[0].posse != 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[0].distancia_gol) && (players[0].posse = 1) && (players[1].posse = 1) && (players[2].posse != 1))
	{
		players[2].comportamento = 1;
		if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[0].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[0].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[0].comportamento = 3;
		}
	}
	else if ((players[2].distancia_gol < players[1].distancia_gol) && (players[1].distancia_gol < players[0].distancia_gol) && (players[0].posse = 1) && (players[1].posse != 1) && (players[2].posse = 1))
	{
		players[1].comportamento = 1;
		if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[0].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[0].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[0].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
	else
	{
		players[0].comportamento = 1;
		if ((players[1].distancia_bola < players[2].distancia_bola) && (side == 1) && (players[6].x < x_limite1))
		{
			players[1].comportamento = 3;
			players[2].comportamento = 2;
		}
		else if ((players[1].distancia_bola < players[2].distancia_bola) && (side == 0) && (players[6].x > x_limite2))
		{
			players[1].comportamento = 3;
			players[2].comportamento = 2;
		}
		else
		{
			players[1].comportamento = 2;
			players[2].comportamento = 3;
		}
	}
}

void estrategia::comportamento2()
{
	/*-------------------------------------------  SETANDO AS DISTANCIAS  ----------------------------------------------*/
	Cria_distancia();
	/*---------------------------------------------  COMPORTAMETNTOS  --------------------------------------------------*/

	//Comportamento = 1 --> Goleiro
	//Comportamento = 2 --> Atacante
	//Comportamento = 3 --> Lateral

	players[0].comportamento = 1;  //O goleiro é fixo
	players[1].comportamento = 2;
	players[2].comportamento = 3;

	//cout << "distancia do verde a bola " << players[1].distancia_bola << endl;
	//cout << "distancia do marrom a bola " << players[2].distancia_bola << endl;
	//cout << "limite 1 " << x_limite1 << endl;
	//cout << "limite 2 " << x_limite2 << endl;
	//cout << "bola_x " << players[6].x << endl;

	if ((players[6].x > x_limite1) && (players[1].distancia_bola > 80.0) && (side == 1) && (players[2].distancia_bola <= 60.0))
	{
		players[1].comportamento = 3;
		players[2].comportamento = 2;
		//cout << "case1" << endl;
	}
	else if ((players[6].x < x_limite2) && (players[1].distancia_bola > 80.0) && (side == 0) && (players[2].distancia_bola <= 60.0))
	{
		players[1].comportamento = 3;
		players[2].comportamento = 2;
		//cout << "case2" << endl;
	}
	else
	{
		players[1].comportamento = 2;
		players[2].comportamento = 3;
		//cout << "case3" << endl;
	}
}

void estrategia::comportamento3()
{
	/*-------------------------------------------  SETANDO AS DISTANCIAS  ----------------------------------------------*/
	Cria_distancia();
	/*---------------------------------------------  COMPORTAMETNTOS  --------------------------------------------------*/
	//Comportamento = 1 --> Goleiro
	//Comportamento = 2 --> Atacante
	//Comportamento = 3 --> Lateral
	//Comportamento = 4 --> Zagueiro

	players[0].comportamento = 1;  //Goleiro é fixo

	if (bola_na_area())
	{
		players[1].comportamento = 4;
		players[2].comportamento = 4;
		//cout << "caso1" << endl;
	}
	else if ((flag_bola_area == 1) && (!bola_na_area()) && (players[1].distancia_bola < players[2].distancia_bola))
	{
		players[1].comportamento = 3;
		players[2].comportamento = 2;
		//cout << "caso2" << endl;
	}
	else if ((flag_bola_area == 1) && (!bola_na_area()) && (players[2].distancia_bola < players[1].distancia_bola))
	{
		players[1].comportamento = 2;
		players[2].comportamento = 3;
		//cout << "caso3" << endl;
	}
	/*else if ((players[6].x > x_limite1) && (players[1].distancia_bola > 80.0) && (side == 1) && (players[2].distancia_bola <= 60.0))
	{
		players[1].comportamento = 3;
		players[2].comportamento = 2;
		std::cout << "caso4" << endl;
	}
	else if ((players[6].x < x_limite2) && (players[1].distancia_bola > 80.0) && (side == 0) && (players[2].distancia_bola <= 60.0))
	{
		players[1].comportamento = 3;
		players[2].comportamento = 2;
		std::cout << "caso5" << endl;
	}*/
	else
	{
		players[1].comportamento = comportamento1_old;
		players[2].comportamento = comportamento2_old;
		// cout << "caso6" << endl;
	}
	flag_bola_area = bola_na_area();
	comportamento1_old = players[1].comportamento;
	comportamento2_old = players[2].comportamento;

	//std::cout << "Bola na Area: " << flag_bola_area << endl;

}

// -----------------OBJETIVOS ---------------//

void estrategia::goleiro(void)
{

	ponto_f.x = ponto_gol.x;

	if (players[6].y >= y_gol_max)
    {
		ponto_f.y = y_gol_max;
    }
	else if (players[6].y <= y_gol_min)
    {
		ponto_f.y = y_gol_min;
    }
    else
    {
         ponto_f.y = players[6].y;
    }
}

void estrategia::goleiro2(void)
{
	ponto_f.x = ponto_gol.x;
	float delta_x = ponto_gol.x - players[6].x;

	if ((delta_x < meio_campo.x) && (players[6].y >= y_gol_max))
	{
		ponto_f.y = y_gol_max;
		//cout << "caso1" << endl;
	}
	else if ((delta_x < meio_campo.x) && (players[6].y < y_gol_max) && (players[6].y > y_gol_min))
	{
		ponto_f.y = players[6].y;
		//cout << "caso2" << endl;
	}
	else if ((delta_x < meio_campo.x) && (players[6].y <= y_gol_min))
	{
		ponto_f.y = y_gol_min;
		//cout << "caso3" << endl;
	}
	else if ((delta_x > meio_campo.x) && (players[6].y < y_gol_max) && (players[6].y > y_gol_min))
	{
		ponto_f.y = players[6].y;
		//cout << "caso4" << endl;
	}
	else
	{
		ponto_f.y = ponto_gol.y;
		//cout << "caso5" << endl;
	}
}

void estrategia::goleiro_gago(void)
{
	robot objective;
    //float y_max = 148; // y maximo que o goleiro pode ir
    //float y_min = 64; // y minimo que o goleiro pode ir
	//float y_med = (y_gol_max + y_gol_min) / 2.0;
	float coeficiente;

	if (side == 1){
		if (players[6].x*1.0 < ponto_gol.x*1.0){
			coeficiente = 1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}
		else {
			coeficiente = ponto_gol.x*1.0 / players[6].x*1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}
	}
	else {
		if (players[6].x*1.0 > ponto_gol.x*1.0){
			coeficiente = 1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}
		else {
			coeficiente = players[6].x*1.0 / ponto_gol.x*1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}

	}

	coeficiente = pow(coeficiente, 0.5);


	//cout << "coeficiente = " << coeficiente << endl;
	//cout << "side = " << side << endl;
	

	objective.x = ponto_gol.x;

	if (players[6].y > ponto_gol.y)
	{
		if (players[6].y > y_gol_max){
			//cout << "Bola Acima do max" << endl;
			objective.y = ponto_gol.y + (abs(ponto_gol.y - y_gol_min)* coeficiente);
		}
		else {
			//cout << "Bola Acima" << endl;
			objective.y = ponto_gol.y + (abs(ponto_gol.y - players[6].y) * coeficiente);
		}
	}
	else if (players[6].y < ponto_gol.y)
	{
		if (players[6].y < y_gol_min){
			//cout << "Bola Abaixo do min" << endl;
			objective.y = ponto_gol.y - (abs(ponto_gol.y - y_gol_min)* coeficiente);
		}
		else {
			//cout << "Bola Abaixo" << endl;
			objective.y = ponto_gol.y - (abs(ponto_gol.y - players[6].y) * coeficiente);
		}
    } else
	{
		//cout << "Bola no meio" << endl;
		objective.y = ponto_gol.y;
    }

	//for (int i = 0; i < 3; i++)
	//{
	//	if (players[i].comportamento == 1)
	//		players[i].setObjective(objective);
	//}

	if (players[6].x == -1)
		objective.y = players[0].y;
	players[0].setObjective(objective);
}

void estrategia::goleiro_definitivo(void){

	if (players[6].stop){
		auxilia_goleiro(players[6].y);
	}
	else {

		float ponto_final;

		if (side == 1){

			ponto_final = players[6].y - (x_area_gol1 - players[6].x) * tan(players[6].theta);

		}
		else {

			ponto_final = players[6].y - (x_area_gol2 - players[6].x) * tan(players[6].theta);

		}

			if (players[6].y > ponto_gol.y)
			{
				auxilia_goleiro(ponto_final);
			}
			else if (players[6].y < ponto_gol.y)
			{
				auxilia_goleiro(ponto_final);
			}
			else
			{
				auxilia_goleiro(ponto_gol.y);
			}

		//auxilia_goleiro(ponto_final);

		cout << "ponto final: " << ponto_final << endl;
	}

}

void estrategia::auxilia_goleiro(float ponto_final)
{

	robot objective;

	float coeficiente;

	float destino_x_bola;

	 if (players[6].stop == 1){
		 destino_x_bola = ponto_gol.x;
	 }
	 else if (players[6].stop == 0){
		 if (side == 1){

			 destino_x_bola = x_area_gol1;

		 }
		 else {

			 destino_x_bola = x_area_gol2;

		 }
	 }

	if (side == 1)
	{
		if (players[6].x*1.0 < destino_x_bola*1.0){
			coeficiente = 1.0;								  // constante de proporcionalidade da distancia da bola ao goleiro
		}
		else 
		{
			coeficiente = destino_x_bola*1.0 / players[6].x*1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}
	}
	else {
		if (players[6].x*1.0 > destino_x_bola*1.0){
			coeficiente = 1.0;								  // constante de proporcionalidade da distancia da bola ao goleiro
		}
		else {
			coeficiente = players[6].x*1.0 / destino_x_bola*1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}

	}

	coeficiente = pow(coeficiente, 0.5);


	 //coeficiente = 1.0;

	if (bola_na_area())
	{
		objective = players[6];
	}
	else
	{
		objective.x = ponto_gol.x;

		if (ponto_final > ponto_gol.y)
		{
			if (ponto_final > y_gol_max){
				//cout << "Bola Acima do max" << endl;
				objective.y = ponto_gol.y + (abs(ponto_gol.y - y_gol_min)* coeficiente);
			}
			else {
				//cout << "Bola Acima" << endl;
				objective.y = ponto_gol.y + (abs(ponto_gol.y - ponto_final) * coeficiente);
			}
		}
		else if (ponto_final < ponto_gol.y)
		{
			if (ponto_final < y_gol_min){
				//cout << "Bola Abaixo do min" << endl;
				objective.y = ponto_gol.y - (abs(ponto_gol.y - y_gol_min)* coeficiente);
			}
			else {
				//cout << "Bola Abaixo" << endl;
				objective.y = ponto_gol.y - (abs(ponto_gol.y - ponto_final) * coeficiente);
			}
		}
		else
		{
			//cout << "Bola no meio" << endl;
			objective.y = ponto_gol.y;
		}
	}
	if (players[6].x == -1)
		objective.y = players[0].y;
	players[0].setObjective(objective);


}

void estrategia::goleiro3(void)
{
	robot objective;

	float coeficiente;

	if (side == 1){
		if (players[6].x*1.0 < ponto_gol.x*1.0){
			coeficiente = 1.0;								  // constante de proporcionalidade da distancia da bola ao goleiro
		}
		else {
			coeficiente = ponto_gol.x*1.0 / players[6].x*1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}
	}
	else {
		if (players[6].x*1.0 > ponto_gol.x*1.0){
			coeficiente = 1.0;								  // constante de proporcionalidade da distancia da bola ao goleiro
		}
		else {
			coeficiente = players[6].x*1.0 / ponto_gol.x*1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}

	}

	coeficiente = pow(coeficiente, 0.5);

	if (bola_na_area())
	{
		objective = players[6];
	}
	else
	{
		objective.x = ponto_gol.x;

		if (players[6].y > ponto_gol.y)
		{
			if (players[6].y > y_gol_max){
				//cout << "Bola Acima do max" << endl;
				objective.y = ponto_gol.y + (abs(ponto_gol.y - y_gol_min)* coeficiente);
			}
			else {
				//cout << "Bola Acima" << endl;
				objective.y = ponto_gol.y + (abs(ponto_gol.y - players[6].y) * coeficiente);
			}
		}
		else if (players[6].y < ponto_gol.y)
		{
			if (players[6].y < y_gol_min){
				//cout << "Bola Abaixo do min" << endl;
				objective.y = ponto_gol.y - (abs(ponto_gol.y - y_gol_min)* coeficiente);
			}
			else {
				//cout << "Bola Abaixo" << endl;
				objective.y = ponto_gol.y - (abs(ponto_gol.y - players[6].y) * coeficiente);
			}
		}
		else
		{
			//cout << "Bola no meio" << endl;
			objective.y = ponto_gol.y;
		}
	}
	if (players[6].x == -1)
		objective.y = players[0].y;
	players[0].setObjective(objective);
}

void estrategia::goleiro4(void)
{
	// float projecao = players[6].y + (ponto_gol.x - players[6].x) * tan(players[6].theta);

	float projecao;


	if (side == 1){

		projecao = players[6].y + (x_area_gol1 - players[6].x) * tan(players[6].theta);



	}
	else {


			projecao = players[6].y + (x_area_gol2 - players[6].x) * tan(players[6].theta);

	}

	if (players[6].stop == 1){
		auxilia_goleiro(players[6].y);
	}
	else if (players[6].stop == 0){


		if (side == 1){
			
			if (abs(players[6].theta) < (M_PI / 2)){
				//cout << " ======================================================== caso do side = 1" << endl;
				auxilia_goleiro(ponto_gol.y);
			}
			else {

				auxilia_goleiro(projecao);
			}



		}
		else {

			if (abs(players[6].theta) > (M_PI/2)){
				//cout << " ======================================================== caso do side = 0" << endl;
				auxilia_goleiro(ponto_gol.y);
			}
			else {

				auxilia_goleiro(projecao);
			}

		}

	}


	//if (players[6].x == -1)
	//	objective.y = players[0].y;
	//players[0].setObjective(objective);
}

void estrategia::auxilia_atacante(float ponto_final)
{
	float x_maximo = x_max / reduce;
	float y_maximo = y_max / reduce;

	int atacante = -1;

	robot objective;

	float coeficiente;

	float destino_x_bola;

	for (int i = 0; i < 3; ++i)
	{
		if (players[i].comportamento == 2)
		{
			atacante = i;
		}
	}

	float ponto_medio2 = meio_campo.x + (meio_campo.x / 2); // perto do x maximo
	float ponto_medio1 = meio_campo.x - (meio_campo.x / 2); // perto do x minimo

	if (players[6].stop == 1){
		if (side == 1){

			destino_x_bola = ponto_medio1; // o -5 eh para nao projetar a bola nele

		}
		else {

			destino_x_bola = ponto_medio2;

		}
	}
	else if (players[6].stop == 0){
		if (side == 1){

			destino_x_bola = ponto_medio1 + 5; // o -5 eh para nao projetar a bola nele

		}
		else {

			destino_x_bola = ponto_medio2 - 5;

		}
	}

	if (side == 1)
	{
		if (players[6].x*1.0 < destino_x_bola*1.0){
			coeficiente = 1.0;								  // constante de proporcionalidade da distancia da bola ao goleiro
		}
		else
		{
			coeficiente = destino_x_bola*1.0 / players[6].x*1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}
	}
	else {
		if (players[6].x*1.0 > destino_x_bola*1.0){
			coeficiente = 1.0;								  // constante de proporcionalidade da distancia da bola ao goleiro
		}
		else {
			coeficiente = players[6].x*1.0 / destino_x_bola*1.0; // constante de proporcionalidade da distancia da bola ao goleiro
		}

	}

	coeficiente = pow(coeficiente, 0.5);
	// coeficiente = 1.0;

	if (bola_na_area())
	{
		 // nao faz nada
	}
	else
	{
		//objective.x = ponto_gol.x;

		if (side == 1){

			objective.x = ponto_medio1;

		}
		else {

			objective.x = ponto_medio2;

		}

		if (ponto_final > ponto_gol.y)
		{
			if (ponto_final > y_maximo){
				//cout << "Bola Acima do max" << endl;
				objective.y = ponto_gol.y + (abs(ponto_gol.y)* coeficiente);
			}
			else {
				//cout << "Bola Acima" << endl;
				objective.y = ponto_gol.y + (abs(ponto_gol.y - ponto_final) * coeficiente);
			}
		}
		else if (ponto_final < ponto_gol.y)
		{

			if (ponto_final < y_min){
				//cout << "Bola Abaixo do min" << endl;
				objective.y = ponto_gol.y - (abs(ponto_gol.y)* coeficiente);
			}
			else {
				//cout << "Bola Abaixo" << endl;
				objective.y = ponto_gol.y - (abs(ponto_gol.y - ponto_final) * coeficiente);
			}
		}
		else
		{
			//cout << "Bola no meio" << endl;
			objective.y = ponto_gol.y;
		}
	}


	if (atacante != -1)
	{
		if (players[6].x == -1)
			objective.y = players[atacante].y;
		players[atacante].setObjective(objective);
	}


	/*cout << "coeficiente =" << coeficiente << endl;
	cout << "ponto final = " << ponto_final << endl;
	cout << "ponto medio 2 = " << ponto_medio2 << endl;
	cout << "ponto medio 1 = " << ponto_medio1 << endl;
	cout << "meio campo = " << meio_campo.x << endl;



	cout << "x max = " << x_max / reduce << endl;
	cout << "y max = " << y_max / reduce << endl;
	cout << "x min = " << x_min / reduce << endl;
	cout << "y min = " << y_min / reduce << endl;*/
}

void estrategia::atacante(void)
{
	robot objective1;
	int atacante;
	
	for (int i = 0; i < 3; ++i)
	{
		if (players[i].comportamento == 2)
		{
			atacante = i;
		}
	}

	objective1.y = meio_campo.y;

	if (players[6].x == -1)
	{
		players[atacante].setObjective(players[atacante]);
		
	}
	else
	{
		if ((side == 1) && players[atacante].posse != 1)
		{
			objective1.x = players[6].x - 10;
			
		}
		else if ((side == 0) && players[atacante].posse != 1)
		{
			objective1.x = players[6].x + 10;
			
		}
		else
		{
			objective1.x = ponto_gol_adv.x;
			
		}
		for (int i = 0; i < 3; i++)
		{
			if (players[i].comportamento == 2)
				players[i].setObjective(objective1);
		}
	}
}

void estrategia::atacante2(void)
{

	float ponto_medio2 = meio_campo.x + (meio_campo.x / 2); // perto do x maximo
	float ponto_medio1 = meio_campo.x - (meio_campo.x / 2); // perto do x minimo


	float projecao;


	if (side == 1){

		projecao = players[6].y + (ponto_medio1 + 5 - players[6].x) * tan(players[6].theta);



	}
	else {


		projecao = players[6].y + (ponto_medio2 - 5 - players[6].x) * tan(players[6].theta);

	}

	if ((side == 1) && (players[6].x < ponto_medio1)){

		auxilia_atacante(ponto_gol.y);
	}
	else if ((side == 0) && (players[6].x > ponto_medio2))
	{
		auxilia_atacante(ponto_gol.y);
	}
	else {

		if (players[6].stop == 1){
			auxilia_atacante(players[6].y);
		}
		else if (players[6].stop == 0){


			if (side == 1){

				if (abs(players[6].theta) < (M_PI / 2)){
					//cout << " ======================================================== caso do side = 1" << endl;
					auxilia_atacante(ponto_gol.y);
				}
				else {

					auxilia_atacante(projecao);
				}



			}
			else {

				if (abs(players[6].theta) > (M_PI / 2)){
					//cout << " ======================================================== caso do side = 0" << endl;
					auxilia_atacante(ponto_gol.y);
				}
				else {

					auxilia_atacante(projecao);
				}

			}

		}
	}
}

void estrategia::lateral()
{
	robot objective;
	int lateral, atacante;
	lateral = 1;
	atacante = 2;
	for (int i = 0; i < 3; ++i)
	{
		if (players[i].comportamento == 3)
		{
			lateral = i;
		}
	}
	for (int j = 0; j < 3; ++j)
	{
		if (players[j].comportamento == 2)
		{
			atacante = j;
		}
	}


	if (players[6].x == -1)
	{
		players[lateral].setObjective(players[lateral]);
	}
	else
	{
		if ((players[lateral].posse != 1) && (players[atacante].posse == 1))
		{
			objective = meio_campo;
			//cout << "Meio do campo" << endl;
		}
		else if ((players[lateral].posse == 1) && (players[atacante].posse != 1))
		{
			objective.y = faz_gol();
			objective.x = ponto_gol_adv.x;
			//cout << "Leva bola pro gol" << endl;
		}
		else
		{
			objective = players[6];
			//cout << "Bola, bola bola" << endl;
		}

		for (int i = 0; i < 3; i++)
		{
			if (players[i].comportamento == 3)
				players[i].setObjective(objective);
		}
	}
}

float estrategia::faz_gol(void)
{
	float gol;

	for (int i = 3; i < 6; ++i)
	{
		if ((side == 1) && (players[i].x <= x_limite1) && (players[i].y >= y_gol_min) && (players[i].y <= y_gol_max))
		{
			float dist_inf = players[i].y - y_gol_min;
			float dist_sup = y_gol_max - players[i].y;
			if (dist_inf < dist_sup)
			{
				gol = y_gol_max;
			}
			else
			{
				gol = y_gol_min;
			}
		}
		else if ((side == 0) && (players[i].x >= x_limite2) && (players[i].y >= y_gol_min) && (players[i].y <= y_gol_max))
		{
			float dist_inf = players[i].y - y_gol_min;
			float dist_sup = y_gol_max - players[i].y;
			if (dist_inf < dist_sup)
			{
				gol = y_gol_max;
			}
			else
			{
				gol = y_gol_min;
			}
		}
		else
			gol = ponto_gol_adv.y;
	}
	return gol;
	//float defensores[6];	//Vetor que define o ymin e ymax,[min1; min2; min3; max1; max2; max3], defendido por cada carrinho que esteja na defesa.
	//for (int i = 0; i < 3; ++i)
	//{
	//	if ((side == 1) && (players[i + 3].x <= x_limite1) && (players[i + 3].y >= y_gol_min) && (players[i + 3].y <= y_gol_max))
	//	{
	//		defensores[i]     = players[i + 3].y - 17;
	//		defensores[i + 3] = players[i + 3].y + 17;
	//	}
	//	else if ((side == 0) && (players[i + 3].x >= x_limite2) && (players[i + 3].y >= y_gol_min) && (players[i + 3].y <= y_gol_max))
	//	{
	//		defensores[i] = players[i + 3].y - 17;
	//		defensores[i + 3] = players[i + 3].y + 17;
	//	}
	//}
	//for (int i = 0; i < 3; ++i)
	//{
	//	if ()
	//}
}

void estrategia::zagueiro(void)
{
	robot objective;
	if (bola_na_area())
	{
		if (players[1].y < players[2].y)
		{
			players[1].setObjective(players[6].x, y_area_gol_min - 17);
			players[2].setObjective(players[6].x, y_area_gol_max + 17);
		}
		else
		{
			players[1].setObjective(players[6].x, y_area_gol_max + 17);
			players[2].setObjective(players[6].x, y_area_gol_min - 17);
		}
	}

//	//for (int i = 1; i < 3; i++)
//	//{
//		//objective.x = players[6].x;
//		
//		//if (((side == 1) && (players[i].y < meio_campo.y)) || ((side == 0) && (players[i].y < meio_campo.y)))
//		//{
//
//		
//			/*for (int j = 1; j < 3; j++)
//			{
//				if (players[i].y < players[j].y)
//				{
//					objective.y = y_area_gol_min - 17;
//				}
//				else
//				{
//					objective.y = y_area_gol_max + 17;
//				}
//			}*/
//		//}
//		//else if (((side == 1) && (players[i].y > meio_campo.y)) || ((side == 0) && (players[i].y > meio_campo.y)))
//		////{
//		//	for (int j = 1; j < 3; j++)
//		//	{
//		//		if (players[i].y < players[j].y)
//		//		{
//		//			objective.y = y_area_gol_min - 17;
//		//		}
//		//		else
//		//		{
//		//			objective.y = y_area_gol_max + 17;
//		//		}
//		//	}
//
//		//	objective.y = y_area_gol_max + 17;
//		//}
//
//
//		//if (players[i].comportamento == 4)
//			players[i].setObjective(objective);
//	}
//}
}


// -----------------OBSTACULOS----------------//

void estrategia::obstacles(void)
{
	field = 0;
	for (int i = 0; i < 3; i++)
	{
		if (players[i + 3].x != -1)
		{
			cv::circle(field, cvPoint((int)players[i + 3].x / 4, (int)players[i + 3].y / 4), 7, 128, -1);
			//cout << "Adicionado um obstaculo em (" << (int)players[i + 3].x << ", " <<  (int)players[i + 3].y << ")" << endl;
		}
	}

	//cout << "Adicionado um obstaculo em (" << (int)players[6].x << ", " <<  (int)players[6].y << ")" << endl;
	if (players[6].x != -1)
	{
		cv::circle(field, cvPoint(((int)players[6].x - 20) / 4, (int)players[6].y / 4), 9, 128, -1);
		cv::circle(field, cvPoint((int)players[6].x / 4, (int)players[6].y / 4), 5, 0, -1);
	}
	cv::circle(field, cvPoint(25, 25), 10, 20, -1);

}

void estrategia::obstaclesI(int n)
{
	fieldI[n] = 10;

	RotatedRect zona_leve(Point(width / 2, height / 2), Size(width, height*0.8), 0);
	cv::ellipse(fieldI[n], zona_leve, 0, -1);

	// ============= Peso dos Jogadores =========//

	for (int i = 0; i < 3; i++)
	{
		if (players[i].x != -1)
		{
			if (i != n) 
				cv::circle(fieldI[n], cvPoint((int)players[i].x * reduce, (int)players[i].y * reduce), adv_radius, adv_weight * reduce, -1);
		}
	}
	for (int i = 3; i < 6; i++)
	{
		if (players[i].x != -1)
		{
			if (!players[n].posse)
				cv::circle(fieldI[n], cvPoint((int)players[i].x * reduce, (int)players[i].y * reduce), adv_radius, adv_weight * reduce, -1);
		}
	}



	float angle;

	if (players[6].x != -1)
	{
		// ============= Peso da bola =========//
		if (!players[n].posse)
		{
			
			angle = atan2(ponto_gol_adv.y - players[6].y, ponto_gol_adv.x - players[6].x);
			cv::circle(fieldI[n], cvPoint((int)(players[6].x + 0 * cos(angle)) *reduce, (int)(players[6].y + 0 * sin(angle)) *reduce), ball_radius * reduce, ball_weight* reduce, -1);
			//RotatedRect bola(Point((int)(players[6].x - 10 * cos(angle)) * reduce, (int)(players[6].y - 10 * sin(angle)) * reduce), Size(ball_w, ball_h), angle*(180 / M_PI));
			//cv::ellipse(fieldI[n], bola, 0, -1);

			cv::Point end_line ((players[6].x - cos(angle)*ball_radius) *reduce, (players[6].y - sin(angle)*ball_radius) *reduce);

			//cout << "End_line: " << end_line.y << endl;
			//cout << "Height: " << height << endl;
			if (end_line.y <= 0)
				end_line.y = players[6].y * reduce;
			else if (end_line.y > height)
				end_line.y = players[6].y * reduce;


			cv::line(fieldI[n], cv::Point(players[6].x *reduce, players[6].y *reduce), end_line, 0, 2);
			//cv::circle(fieldI[n], cvPoint((int)players[6].x * reduce, (int)players[6].y *reduce), 3, 0, -1);
		}

		// ============= Peso do lateral com posse de bola =========//
		else
		{
			angle = atan2(players[6].y - players[n].y, players[6].x - players[n].x);

			cv::circle(fieldI[n], cvPoint((int)(players[n].x - 0 * cos(angle)) *reduce, (int)(players[n].y - 0 * sin(angle)) *reduce), player_radius * reduce, ball_weight * reduce, -1);
			RotatedRect bola(Point((int)(players[n].x + 10 * cos(angle)) * reduce, (int)(players[n].y + 10 * sin(angle)) * reduce), Size(9, 3), angle*(180 / M_PI));
			cv::ellipse(fieldI[n], bola, 0, -1);
			//angle += M_PI / 2;
			//cv::circle(fieldI[n], cvPoint((int)(players[6].x - 10 * cos(angle)) *reduce, (int)(players[6].y - 10 * sin(angle)) *reduce), 1, 90, -1);
			//angle = M_PI;
			//cv::circle(fieldI[n], cvPoint((int)(players[6].x - 10 * cos(angle)) *reduce, (int)(players[6].y - 10 * sin(angle)) *reduce), 1, 90, -1);

			//cv::circle(fieldI[n], cvPoint((int)(players[n].x - 20 * cos(angle)) *reduce, (int)(players[n].y - 20 * sin(angle)) *reduce), 8, 50, -1);
			//cv::circle(fieldI[n], cvPoint((int)players[n].x * reduce, (int)players[n].y *reduce), 3, 0, -1);
		}
	}
	

	// ============= Peso de dentro do gol =========//

	cv::rectangle(fieldI[n], cv::Rect(0, 0, x_gol_min * reduce, height+1), 30, -1);
	cv::rectangle(fieldI[n], cv::Rect(x_gol_max * reduce, 0, width, height + 1), 30, -1);
	//cv::circle(fieldI[n], cvPoint((int)players[n].x * reduce, (int)players[n].y *reduce), 4, 0, -1);
}

// ----------------ROTAS---------------------//

void estrategia::routing(robot init, robot goal)
{
	int cost = 1;
	std::vector<robot> open;
	way.clear();
	init.g_value = 0;
	int step, nstep;
	robot next;
	float x, y, x2, y2, x3, y3;
	int g, g2;

	closed = 0;
	closed.at<int>(init.y, init.x) = 1;

	expand = -1;
	step = 0;



	open.push_back(init);

	int delta[8][3] = { { 0, -1 }, // go up
						{ -1, -1}, // go northwest
						{ -1, 0 }, // go left
						{ 1, -1}, // go southwest
						{ 0, 1 }, // go down
						{ 1, 1}, // go southeast
						{ 1, 0 }, // go right
						{ -1, 1 }}; // go norheast


	bool found = false;
	bool resign = false;

	while (!found && !resign)
	{
		if (open.size() == 0)
		{
			cout << "Resign" << endl;
			resign = true;
			break;
		}
		else
		{

			open = sort(open);
			reverse(open.begin(), open.end());
			next = open.back();
			open.pop_back();

			x = next.x;
			y = next.y;
			g = next.g_value;

			expand.at<int>(y, x) = step;
			step += 1;
			

			if ((x == goal.x) && (y == goal.y))
			{
				found = true;
				cout << "Found" << endl;
			}
			else
			{
				for (int i = 0; i < 8; i++)
				{
					x2 = x + delta[i][0];
					y2 = y + delta[i][1];
					if ((x2 >= 0) && (x2 < width) && (y2 >= 0) && (y2 < height))
					{
						if ((closed.at<int>(y2, x2) == 0) && (field.at<uchar>(y2, x2) == 0)) // 
						{
							g2 = g + cost;
							open.push_back(robot(x2, y2, g2));
							closed.at<int>(y2, x2) = 1;
						}
					}
				}
			}
		}
	}
	for (int a = 0; a < g; a++)
	{
		nstep = expand.at<int>(y, x);
		for (int b = 0; b < 8; b++)
		{
			x2 = x + delta[b][0];
			y2 = y + delta[b][1];
			if ((0 <= x2) && (x2 < width) && (0 <= y2) && (y2 < height))
			{
				if ((nstep > expand.at<int>(y2, x2)) && (expand.at<int>(y2, x2) != -1))
				{
					nstep = expand.at<int>(y2, x2);
					way.push_back(robot(x2, y2));
					x3 = x2;
					y3 = y2;
				}
			}
		}
		x = x3;
		y = y3;
	}
}

void estrategia::aStar(robot init, robot goal)
{
	int cost = 1;
	std::vector<robot> open;
	way.clear();
	init.g_value = 0;
	init.h_value = distance(init, goal);
	int step, nstep;
	robot next;
	float x, y, x2, y2, x3, y3;
	int g, g2;
	float h, h2;

	closed = 0;
	closed.at<int>(init.y, init.x) = 1;

	expand = -1;
	step = 0;


	open.push_back(init);

	int delta[8][3] = { { 0, -1 }, // go up
		//{ -1, -1 }, // go northwest
	{ -1, 0 }, // go left
	//{ 1, -1 }, // go southwest
	{ 0, 1 }, // go down
	//{ 1, 1 }, // go southeast
	{ 1, 0 } }; // go right
	//{ -1, 1 } }; // go norheast


	bool found = false;
	bool resign = false;

	while (!found && !resign)
	{
		if (open.size() == 0)
		{
			//cout << "Resign" << endl;
			resign = true;
			way.clear();
			break;
		}
		else
		{

			open = sortA(open);
			reverse(open.begin(), open.end());
			next = open.back();
			//for (int i = 0; i < open.size(); i++)
			//	cout << "(" << open[i].x << ", " << open[i].y << ", " << (float)open[i].h_value << ") ; ";
			//cout << endl;

			open.pop_back();



			x = next.x;
			y = next.y;
			g = next.g_value;
			h = next.h_value;


			//cout << "(" << x << ", " << y << ", " << h << ")" << endl;
			expand.at<int>(y, x) = step;
			step += 1;


			if ((x == goal.x) && (y == goal.y))
			{
				found = true;
				//cout << "Found" << endl;
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					x2 = x + delta[i][0];
					y2 = y + delta[i][1];
					if ((x2 >= 0) && (x2 < width) && (y2 >= 0) && (y2 < height))
					{
						if ((closed.at<int>(y2, x2) == 0) && (field.at<uchar>(y2, x2) == 0)) // 
						{
							g2 = g + cost;
							h2 = distance(robot(x2, y2), goal);
							open.push_back(robot(x2, y2, g2, h2));
							closed.at<int>(y2, x2) = 1;
						}
					}
				}
			}
		}
	}
	cout << "Step: " << step << endl;
	cout << "g value: " << g << endl;

	way.push_back(goal);
	if (resign == false)
	{
		for (int a = 0; a < g; a++)
		{
			nstep = expand.at<int>(y, x);
			for (int b = 0; b < 8; b++)
			{
				x2 = x + delta[b][0];
				y2 = y + delta[b][1];
				if ((0 <= x2) && (x2 < width) && (0 <= y2) && (y2 < height))
				{
					if ((nstep > expand.at<int>(y2, x2)) && (expand.at<int>(y2, x2) != -1))
					{
						nstep = expand.at<int>(y2, x2);
						way.push_back(robot(x2, y2));
						x3 = x2;
						y3 = y2;
					}
				}
			}
			x = x3;
			y = y3;
		}
	}
	//way.push_back(init);

	//cout << "[ ";
	//for (int i = 0; i < width; i++)
	//{
	//	for (int j = 0; j < height; j++)
	//		cout << expand.at<int>(i, j) << ", ";
	//	cout << "]" << endl;
	//}
}

void estrategia::weightAStar(robot init, robot goal)
{
	int cost = 1;
	std::vector<robot> open;
	way.clear();
	init.g_value = 0;
	init.h_value = distance(init, goal);
	int step, nstep;
	robot next;
	float x, y, x2, y2, x3, y3;
	int g, g2;
	float h, h2;

	closed = 0;
	closed.at<int>(init.y, init.x) = 1;

	expand = -1;
	step = 0;


	open.push_back(init);

	int delta[4][3] = { { 0, -1 }, // go up
		//{ -1, -1 }, // go northwest
	{ -1, 0 }, // go left
	//{ 1, -1 }, // go southwest
	{ 0, 1 }, // go down
	//{ 1, 1 }, // go southeast
	{ 1, 0 } }; // go right
	//{ -1, 1 } }; // go norheast


	bool found = false;
	bool resign = false;

	while (!found && !resign)
	{
		if (open.size() == 0)
		{
			cout << "Resign" << endl;
			resign = true;
			way.clear();
			break;
		}
		else
		{

			open = sortA(open);
			reverse(open.begin(), open.end());
			next = open.back();
			//for (int i = 0; i < open.size(); i++)
			//	cout << "(" << open[i].x << ", " << open[i].y << ", " << open[i].g_value << ", " << (float)open[i].h_value << ") ; ";
			//cout << endl;

			open.pop_back();

			x = next.x;
			y = next.y;
			g = next.g_value;
			h = next.h_value;


			//cout << "(" << x << ", " << y << ", " << g << ", " << h << ")" << endl;
			expand.at<int>(y, x) = step;
			step += 1;


			if ((x == goal.x) && (y == goal.y))
			{
				found = true;
				cout << "Found" << endl;
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					x2 = x + delta[i][0];
					y2 = y + delta[i][1];
					if ((x2 >= 0) && (x2 < width) && (y2 >= 0) && (y2 < height))
					{
						if ((closed.at<int>(y2, x2) == 0)) // 
						{
							g2 = g + field.at<uchar>(y2, x2);
							h2 = g2 + distance(robot(x2, y2), goal);
							open.push_back(robot(x2, y2, g2, h2));
							closed.at<int>(y2, x2) = 1;
						}
					}
				}
			}
		}
	}

	way.push_back(goal);
	if (resign == false)
	{
		for (int a = 0; a < (step-1); a++)
		{
			nstep = expand.at<int>(y, x);
			for (int b = 0; b < 8; b++)
			{
				x2 = x + delta[b][0];
				y2 = y + delta[b][1];
				if ((0 <= x2) && (x2 < width) && (0 <= y2) && (y2 < height))
				{
					if ((nstep > expand.at<int>(y2, x2)) && (expand.at<int>(y2, x2) != -1))
					{
						nstep = expand.at<int>(y2, x2);
						way.push_back(robot(x2, y2));
						x3 = x2;
						y3 = y2;
					}
				}
			}
			x = x3;
			y = y3;
		}
	}
	//way.push_back(init);

	//cout << "[ ";
	//for (int i = 0; i < width; i++)
	//{
	//	for (int j = 0; j < height; j++)
	//		cout << expand.at<int>(i, j) << ", ";
	//	cout << "]" << endl;
	//}
}

void estrategia::weightAStarI(int n)
{
	robot init = robot((int) (players[n].x * reduce), (int) (players[n].y * reduce));
	robot goal = robot((int)(players[n].objective_x * reduce), (int)(players[n].objective_y * reduce));

	//cout << "Carrinho " << n << ": " << players[n].x << ", " << players[n].y << " -->" << players[n].objective_x << ", " << players[n].objective_y << endl;


	int cost = 1;
	std::vector<robot> open;
	path[n].clear();
	init.g_value = 0;
	init.h_value = distance(init, goal);
	int step, nstep;
	robot next;
	float x, y, x2, y2, x3, y3;
	int g, g2;
	float h, h2;

	closedI[n] = 0;
	closedI[n].at<int>(init.y, init.x) = 1;

	expandI[n] = -1;
	step = 0;


	open.push_back(init);

	int delta[4][3] = { { 0, -1 }, // go up
		//{ -1, -1 }, // go northwest
	{ -1, 0 }, // go left
	//{ 1, -1 }, // go southwest
	{ 0, 1 }, // go down
	//{ 1, 1 }, // go southeast
	{ 1, 0 } }; // go right
	//{ -1, 1 } }; // go norheast


	bool found = false;
	bool resign = false;


	//cout << players[n].x << ", " << players[n].y << endl;
	if ((players[n].x < 0) || (width / reduce < players[n].x) || (players[n].y < 0) || (height / reduce < players[n].y))
		resign = true;

	while (!found && !resign)
	{
		if (open.size() == 0)
		{
			cout << "Resign: " << n << endl;
			resign = true;
			path[n].clear();
			break;
		}
		else
		{

			open = sortA(open);
			reverse(open.begin(), open.end());
			next = open.back();
			//for (int i = 0; i < open.size(); i++)
			//	cout << "(" << open[i].x << ", " << open[i].y << ", " << open[i].g_value << ", " << (float)open[i].h_value << ") ; ";
			//cout << endl;

			open.pop_back();

			x = next.x;
			y = next.y;
			g = next.g_value;
			h = next.h_value;


			//cout << "(" << x << ", " << y << ", " << g << ", " << h << ")" << endl;
			expandI[n].at<int>(y, x) = step;
			step += 1;


			if ((x == goal.x) && (y == goal.y))
			{
				found = true;
				//cout << "Found: " << n << endl;
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					x2 = x + delta[i][0];
					y2 = y + delta[i][1];
					if ((x2 >= 0) && (x2 < width) && (y2 >= 0) && (y2 < height))
					{
						//cout << "( " << x2 << ", " << y2 << ")" << endl;
						if ((closedI[n].at<int>(y2, x2) == 0)) // 
						{
							g2 = g + fieldI[n].at<uchar>(y2, x2);
							h2 = g2 + 10 * distance(robot(x2, y2), goal);
							open.push_back(robot(x2, y2, g2, h2));
							closedI[n].at<int>(y2, x2) = 1;
						}
					}
				}
			}
		}
	}

	path[n].push_back(robot(goal.x /reduce, goal.y / reduce));
	if (resign == false)
	{
		for (int a = 0; a < (step - 1); a++)
		{
			nstep = expandI[n].at<int>(y, x);
			for (int b = 0; b < 8; b++)
			{
				x2 = x + delta[b][0];
				y2 = y + delta[b][1];
				if ((0 <= x2) && (x2 < width) && (0 <= y2) && (y2 < height))
				{
					if ((nstep > expandI[n].at<int>(y2, x2)) && (expandI[n].at<int>(y2, x2) != -1))
					{
						nstep = expandI[n].at<int>(y2, x2);
						path[n].push_back(robot(x2 / reduce, y2 / reduce));
						x3 = x2;
						y3 = y2;
					}
				}
			}
			x = x3;
			y = y3;
		}
	}
	//way.push_back(init);


	//cout << "N: " << n << endl;
	//cout << "[ ";
	//for (int i = 0; i < width; i++)
	//{
	//	for (int j = 0; j < height; j++)
	//		cout << closedI[n].at<int>(i, j) << ", ";
	//	cout << "]" << endl;
	//}
}

void estrategia::minimizeRoute(void)
{
	vector<robot> copy = way;
	way.clear();

	for (int i = 0; i < copy.size(); i++)
	{
		if ((i == 0) || (i == copy.size()) || ((i % 2) == 0))
			way.push_back(copy[i]);			
	}
}

void estrategia::minimizeRouteI(int n)
{
	vector<robot> copy = path[n];
	path[n].clear();

	for (int i = 0; i < copy.size(); i++)
	{
		if ((i == 0) || (i == copy.size()) || ((i % 4) == 0))
			path[n].push_back(copy[i]);
	}
}

void estrategia::smoothPath(void)
{
	float weight_data = 0.99;
	float weight_smooth = 0.01;
	float tolerance = 0.0001;
	float change;
	float aux;

	vector<robot> newpath = way;

	change = tolerance;
	while (change >= tolerance)
	{
		change = 0;
		for (int i = 1; i < (way.size() - 1); i++)
		{
			aux = newpath[i].x;
			newpath[i].x += weight_data * (way[i].x - newpath[i].x);
			newpath[i].x += weight_smooth * (newpath[i + 1].x + newpath[i-1].x - 2*newpath[i].x);
			change += abs(newpath[i].x - aux);

			aux = newpath[i].y;
			newpath[i].y += weight_data * (way[i].y - newpath[i].y);
			newpath[i].y += weight_smooth * (newpath[i + 1].y + newpath[i - 1].y - 2 * newpath[i].y);
			change += abs(newpath[i].y - aux);
		}
		//cout << "change: " << change << endl;
	}
	way = newpath;
}

void estrategia::smoothPathI(int n)
{
	float tolerance = 0.1;
	float change;
	float aux;

	vector<robot> newpath = path[n];

	change = tolerance;
	while (change >= tolerance)
	{
		change = 0;
		for (int i = 1; i < (path[n].size() - 1); i++)
		{
			aux = newpath[i].x;
			newpath[i].x += weight_data * (path[n][i].x - newpath[i].x);
			newpath[i].x += weight_smooth * (newpath[i + 1].x + newpath[i - 1].x - 2 * newpath[i].x);
			change += abs(newpath[i].x - aux);

			aux = newpath[i].y;
			newpath[i].y += weight_data * (path[n][i].y - newpath[i].y);
			newpath[i].y += weight_smooth * (newpath[i + 1].y + newpath[i - 1].y - 2 * newpath[i].y);
			change += abs(newpath[i].y - aux);
		}
		//cout << "change: " << change << endl;
	}
	path[n] = newpath;
}

void estrategia::nextGoal(void)
{
	if (distance(players[0], robot(way.back().x*4, way.back().y*4)) > 7)
		players[0].setGoal(way.back().x * 4, way.back().y * 4);
	else
	{
		way.pop_back();
		players[0].setGoal(way.back().x * 4, way.back().y * 4);
	}
}

void estrategia::nextGoalI(int n)
{
	float dist = 0;
	if (path[n].size() > 0)
		dist = distance(players[n], robot(path[n].back().x, path[n].back().y));
	while ((dist < 10) && (path[n].size() > 0))
	{
		path[n].pop_back();
		if (path[n].size() > 0)
			dist = distance(players[n], robot(path[n].back().x, path[n].back().y));
	}
	if (path[n].size() > 0)
		players[n].setGoal(path[n].back().x, path[n].back().y);
	else
		players[n].setGoal(players[n].x, players[n].y);

	//if (distance(players[n], robot(path[n].back().x, path[n].back().y)) > 7)
	//	players[n].setGoal(path[n].back().x, path[n].back().y);
	//else
	//{
	//	path[n].pop_back();
	//	players[n].setGoal(path[n].back().x, path[n].back().y);
	//}
}

void estrategia::dijkstraI(int n)
{
	robot sec1, sec2;
	aresta aresta1, aresta2;
	aresta initial;
	vector<aresta> temp;
	vector<aresta> descoberto;

	draw.clear();
	vertices.clear();
	paths.clear();

	pilha[n].clear();

	float a, b, dist2vert, d2;
	float a2, b2, d3, x0, y0;

	robot init = robot((int)(players[n].x), (int)(players[n].y));
	robot goal = robot((int)(players[n].objective_x), (int)(players[n].objective_y));

	float angle2goal = atan2(goal.y - init.y, goal.x - init.x);
	float secRadius = 20;
	float minCusto = 5000;
	int indMin, ind;

	bool possible;
	bool found = false;
	bool giveup = false;


	// Criando os pontos do grafo
	if (init.x == goal.x && init.y == goal.y)
	{
		pilha[n].push_back(goal);
		return;
	}
	if (init.x != -1 && init.y != -1)
	{
		initial = aresta(init.x, init.y, n);
		vertices.push_back(initial);
	}
	else {
		return;
	}

	for (int i = 0; i < 6; i++)
	{
		if (i != n)
		{
			if (players[i].x != -1 && players[i].y != -1)
			{
				sec1 = robot(players[i].x + secRadius*sin(angle2goal), players[i].y - secRadius*cos(angle2goal));
				sec2 = robot(players[i].x - secRadius*sin(angle2goal), players[i].y + secRadius*cos(angle2goal));

				players[i].setSec1(sec1);
				players[i].setSec2(sec2);

				aresta1 = aresta(players[i].x + secRadius*sin(angle2goal), players[i].y - secRadius*cos(angle2goal), i);
				aresta2 = aresta(players[i].x - secRadius*sin(angle2goal), players[i].y + secRadius*cos(angle2goal), i);

				vertices.push_back(aresta1);
				vertices.push_back(aresta2);
			}

		}
	}
	if (goal.x != -1 && goal.y != -1)
	{
		aresta1 = aresta(goal.x, goal.y, -1); // -1 para goal
		vertices.push_back(aresta1);
	}

	//Criando os caminhos
	for (vector<aresta>::iterator next = vertices.begin(); next != vertices.end(); ++next)
	{
		for (vector<aresta>::iterator it = next + 1; it != vertices.end(); ++it)
		{
			if (next->x != it->x || next->y != it->y)
			{
				possible = true;

				a = (it->y - next->y) / (it->x - next->x);
				b = next->y - a*next->x;

				dist2vert = sqrt(pow(next->x - it->x, 2) + pow(next->y - it->y, 2));
				next->distance = dist2vert;

				if (it->x != -1 && next->x != -1)
				{
					for (int i = 0; i < 6; i++)
					{
						if (i != it->o && i != next->o)
						{
							if (players[i].x != -1 && players[i].y != -1)
							{
								if (a != 0)
								{
									a2 = -1 / a;
									b2 = players[i].y - a2 * players[i].x;

									x0 = (b2 - b) / (a - a2);
									y0 = a*x0 + b;
								}
								else
								{
									x0 = players[i].x;
									y0 = it->y;
								}

								d3 = sqrt(pow(x0 - players[i].x, 2) + pow(y0 - players[i].y, 2));
								d2 = sqrt(pow(it->x - players[i].x, 2) + pow(it->y - players[i].y, 2));


								if (d3 < (secRadius - 5) && (min(it->x, next->x) <= x0) && (x0 <= max(it->x, next->x)) && (min(it->y, next->y) <= y0) && (y0 <= max(it->y, next->y)))
								{
									possible = false;
								}
								if (d2 < (secRadius - 5))
								{
									possible = false;
								}
								if (next->o == it->o)
								{
									possible = false;
								}
							}
						}
					}
				}
				else { possible = false; }
			}
			else { possible = false; }

			if (possible)
			{
				aresta1 = aresta(next->x, next->y, next->o);
				aresta1.setNext(it->x, it->y, it->o);
				aresta1.distance = sqrt(pow(next->x - it->x, 2) + pow(next->y - it->y, 2));
				paths.push_back(aresta1);
			}
		}
	}

	// Calculando melhor caminho

	// Para todos os vertices, o custo é definido como 5000, pois ainda nao se sabe como chegar neles.
	for (vector<aresta>::iterator next = vertices.begin(); next != vertices.end(); ++next)
	{
		next->custoTotal = 5000;
		next->x_ant = -1;
		next->y_ant = -1;
	}

	if (vertices.size() > 0)
	{
		vertices.at(0).custoTotal = 0;
		vertices.at(0).x_ant = vertices.at(0).x;
		vertices.at(0).y_ant = vertices.at(0).y;
	}
	


	temp = vertices;
	//cout << "==============Vertices==============" << endl;
	//for (vector<aresta>::iterator next = temp.begin(); next != temp.end(); ++next)
	//{
	//	cout << "( " << next->x << ", " << next->y << " ) , Custo total: " << next->custoTotal << endl;
	//}

	//cout << "==============Caminhos==============" << endl;
	//for (vector<aresta>::iterator next = paths.begin(); next != paths.end(); ++next)
	//{
	//	cout << "( " << next->x << ", " << next->y << " ) ---> ( " << next->nx << ", " << next->ny << " ) - custo: " << next->distance  << endl;
	//}

	while (!found && !giveup)
	{
		// Procurando o vertice com menor custo de chegada. O primeiro a ser encontrado é sempre o vértice de inicio, pois possui custo zero.
		ind = 0;
		indMin = -1;
		minCusto = 5001;
		for (vector<aresta>::iterator next = temp.begin(); next != temp.end(); ++next)
		{
			if (next->custoTotal < minCusto)
			{
				minCusto = next->custoTotal;
				indMin = ind;
			}
			ind++;
		}
		if (indMin != -1)
		{
			descoberto.push_back(temp.at(indMin));
			temp.erase(temp.begin() + indMin);
		}


		// Procurando todos os segmentos que partem do vertice de menor custo.
		for (vector<aresta>::iterator it = paths.begin(); it != paths.end(); ++it)
		{
			if (it->x == descoberto.back().x && it->y == descoberto.back().y)
			{
				for (vector<aresta>::iterator next = temp.begin(); next != temp.end(); ++next)
				{
					if (next->x == it->nx && next->y == it->ny)
					{
						if (next->custoTotal > descoberto.back().custoTotal + it->distance)
						{
							next->custoTotal = descoberto.back().custoTotal + it->distance;
							//cout << next->custoTotal << " = " << descoberto.back().custoTotal << " + " << it->distance << endl;
							next->x_ant = descoberto.back().x;
							next->y_ant = descoberto.back().y;
						}
					}
				}
			}
			if (it->nx == descoberto.back().x && it->ny == descoberto.back().y)
			{
				for (vector<aresta>::iterator next = temp.begin(); next != temp.end(); ++next)
				{
					if (next->x == it->x && next->y == it->y)
					{
						if (next->custoTotal > descoberto.back().custoTotal + it->distance)
						{
							next->custoTotal = descoberto.back().custoTotal + it->distance;
							//cout << next->custoTotal << " = " << descoberto.back().custoTotal << " + " << it->distance << endl;
							next->x_ant = descoberto.back().x;
							next->y_ant = descoberto.back().y;
						}
					}
				}
			}
		}

		if (temp.size() == 0)
		{
			for (vector<aresta>::iterator next = descoberto.begin(); next != descoberto.end(); ++next)
			{
				if (next->o == -1 && next->custoTotal < 5000)
				{
					found = true;
					//cout << "Caminho encontrado: " << next-> custoTotal << endl;
				}
			}
			if (!found)
			{
				giveup = true;
			}
		}
	}
		
	if (found)
	{
		found = false;
		giveup = false;

		pilha[n].push_back(robot(vertices.back().x, vertices.back().y));
		while (!found && !giveup)
		{
			sec1 = pilha[n].back();
			for (vector<aresta>::iterator next = descoberto.begin(); next != descoberto.end(); ++next)
			{
				if (next->x == sec1.x && next->y == sec1.y)
				{
					pilha[n].push_back(robot(next->x_ant, next->y_ant));
				}
			}

			if (pilha[n].back().x == init.x && pilha[n].back().y == init.y)
			{
				found = true;
			}

			if (pilha[n].size() > 50)
				giveup = true;
		}
	}




}

//---------RECOLHIMENTO DE INFORMAÇÕES--------//

void estrategia::Cria_distancia(void)
{
	for (int i = 0; i < 3; i++)
	{
		players[i].setDistancia_bola(distance(players[i], players[6]));
		players[i].setDistancia_gol(distance(players[i], ponto_gol));
	}
}

void estrategia::Posse(void)
{
	
	float centro_xmin, centro_xmax, centro_ymin, centro_ymax, d_PB1, d_PB2;

	for (int i = 0; i < 3; i++)
	{
		centro_xmin = players[i].x - raio * cos(players[i].theta);
		centro_xmax = players[i].x + raio * cos(players[i].theta);
		centro_ymin = players[i].y - raio * sin(players[i].theta);
		centro_ymax = players[i].y + raio * sin(players[i].theta);
		
		d_PB1 = sqrt(pow(players[6].x - centro_xmin, 2) + pow(players[6].y - centro_ymin, 2));  //distancia de um lado (mais proximo do 0) do carrinho ate a bola
		d_PB2 = sqrt(pow(players[6].x - centro_xmax, 2) + pow(players[6].y - centro_ymax, 2));  //distancia do outro lado do carrinho ate a bola

		if (i == 0)
		{
			circulo1 = robot(centro_xmin, centro_ymin);
			circulo2 = robot(centro_xmax, centro_ymax);
		}
		if (d_PB1 < raio || d_PB2 < raio)
		{
			players[i].posse = true;
		}
		else
		{
			players[i].posse = false;
		}
	}
}

//-------FUNÇÕES AUXILIARES ----------------//

vector<robot> estrategia::sort(vector<robot> new_open)
{
	robot aux;
	int k = new_open.size() - 1;
	for (int i = 0; i < new_open.size(); i++)
	{
		int j = 0;
		while (j < k)
		{
			if (new_open[j].g_value > new_open[j + 1].g_value)
			{
				aux = new_open[j];
				new_open[j] = new_open[j + 1];
				new_open[j + 1] = aux;
			}
			j += 1;
		}
		k -= 1;
	}
	return new_open;
}

vector<robot> estrategia::sortA(vector<robot> new_open)
{
	robot aux;
	int k = new_open.size() - 1;
	for (int i = 0; i < new_open.size(); i++)
	{
		int j = 0;
		while (j < k)
		{
			if (new_open[j].h_value > new_open[j + 1].h_value)
			{
				aux = new_open[j];
				new_open[j] = new_open[j + 1];
				new_open[j + 1] = aux;
			}
			j += 1;
		}
		k -= 1;
	}
	return new_open;
}

vector<robot> estrategia::reverseVector(vector<robot> new_open)
{
	vector<robot>::size_type sz = new_open.size();

	for (unsigned i = 0; i < sz / 2; i++)
	{
		robot temp;
		temp = new_open[sz - 1 - i];
		new_open[sz - 1 - i] = new_open[i];
		new_open[i] = temp;
	}

	return new_open;
}

float estrategia::distance(robot one, robot two)
{
	return sqrt(pow(two.x - one.x, 2) + pow(two.y - one.y, 2));
}

void estrategia::generalRouting(void)
{
	int j = 3;

	std::vector < std::thread> threads;

	obstacles();
	
	//for (int i = 0; i < j; i++)
	//	threads.push_back(std::thread(&estrategia::obstaclesI, this, i));
	//for (auto& th : threads) th.join();

	//threads.clear();

	for (int i = 0; i < j; i++)
		threads.push_back(std::thread(&estrategia::weightAStarI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();

	for (int i = 0; i < j; i++)
		threads.push_back(std::thread(&estrategia::minimizeRouteI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();

	for (int i = 0; i < 3; i++)
		threads.push_back(std::thread(&estrategia::smoothPathI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();

	for (int i = 0; i < j; i++)
		threads.push_back(std::thread(&estrategia::nextGoalI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();
}

void estrategia::generalRouting2(void)
{
	for (int i = 0; i < 3; i++)
		dijkstraI(i);
}

bool estrategia::bola_na_area(void)
{

	//cout << "X1: " << x_area_gol1 << endl;
	//cout << "X2: " << x_area_gol2 << endl;
	//cout << "Y1: " << y_area_gol_min << endl;
	//cout << "Y2: " << y_area_gol_max << endl;

	if ((side == 1) && (players[6].x < x_area_gol1) && (players[6].y >= y_area_gol_min) && (players[6].y <= y_area_gol_max))
	{
		return 1;
	}
	else if ((side == 0) && (players[6].x > x_area_gol2) && (players[6].y >= y_area_gol_min) && (players[6].y <= y_area_gol_max))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}