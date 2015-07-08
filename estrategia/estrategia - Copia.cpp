												ude "estrategia.h"


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

	y_min = 0;
	y_max = height-1;
	y_gol_min = 62;
	y_gol_max = 138;

	x_min = 0;
	x_max = width-1;
	x_gol_min = 20;
	x_gol_max = 301;

	side = 0;
}

estrategia::~estrategia()
{


}

void estrategia::Config(int nh, int nw)
{
	//cout << "Entre com seu lado do campo" << endl;
	//cout << "OBS: Lado = 1 --> x próximo de 0 é o seu gol" << endl;
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

	//cout << "Entre com o y_area_gol_min maximo do campo" << endl;
	//cin >> y_area_gol_min;
	//cout << "Entre com o y_area_gol_max maximo do campo" << endl;
	//cin >> y_area_gol_max;

	height = (nh *reduce);
	width = (nw * reduce);

	x_limite1 = 3 * (x_gol_max + x_gol_min) / 4;
	x_limite2 = (x_gol_max + x_gol_min) / 4;

	x_area_gol1 = 5 * (x_gol_max + x_gol_min) / 6;
	x_area_gol2 = (x_gol_max + x_gol_min) / 6;
	y_area_gol_min -= 15;
	y_area_gol_max += 15;

	meio_campo.x = (x_max + x_min) / 2;
	meio_campo.y = (y_max + y_min) / 2;

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

void estrategia::update(robot* new_players)
{
	for (int i = 0; i < 7; i++)
		players[i] = new_players[i];
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

	players[0].comportamento = 1;  
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
	else if ((flag_bola_area == 1) && (players[1].distancia_bola < players[2].distancia_bola))
	{
		players[1].comportamento = 3;
		players[2].comportamento = 2;
		//cout << "caso2" << endl;
	}
	else if ((flag_bola_area == 1) && (players[2].distancia_bola < players[1].distancia_bola))
	{
		players[1].comportamento = 2;
		players[2].comportamento = 3;
		//cout << "caso3" << endl;
	}
	else if ((players[6].x > x_limite1) && (players[1].distancia_bola > 80.0) && (side == 1) && (players[2].distancia_bola <= 60.0))
	{
		players[1].comportamento = 3;
		players[2].comportamento = 2;
		//cout << "caso4" << endl;
	}
	else if ((players[6].x < x_limite2) && (players[1].distancia_bola > 80.0) && (side == 0) && (players[2].distancia_bola <= 60.0))
	{
		players[1].comportamento = 3;
		players[2].comportamento = 2;
		//cout << "caso5" << endl;
	}
	else
	{
		players[1].comportamento = 2;
		players[2].comportamento = 3;
		//cout << "caso6" << endl;
	}
	flag_bola_area = bola_na_area();
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

void estrategia::lateral(void)
{
	robot objective;
	int lateral, atacante;
	//atacante = 1;
	for (int i = 0; i < 3; ++i)
	{
		if (players[i].comportamento == 3)
		{
			lateral = i;
			if (players[2].comportamento == 3)
			{
				atacante = 1;
			}
			else
			{
				atacante = 2;
			}
			/*for (int j = 0; j < 3; ++j)
			{
				if (players[j].comportamento == 2)
				{
					atacante = j;
				}
			}*/
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
			objective.y = ponto_gol_adv.y;
			//cout << "Leva bola pro gol" << endl;
		}
		else
		{
			objective = players[6];
			//cout << "Bola, bola bola" << endl;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (players[i].comportamento == 3)
			players[i].setObjective(objective);
	}
}

float estrategia::faz_gol(void)
{
	float gol;

	for (int i = 3; i < 6; ++i)
	{
		if ((side == 1) && (players[i].x <= x_limite1) && (players[i].y >= y_gol_min) && (players[i].y <= y_gol_max))
		{
			dist_inf = players[i].y - y_gol_min;
			dist_sup = y_gol_max - players[i].y;
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
			dist_inf = players[i].y - y_gol_min;
			dist_sup = y_gol_max - players[i].y;
			if (dist_inf < dist_sup)
			{
				gol = y_gol_max;
			}
			else
			{
				gol = y_gol_min;
			}
		}
	}

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
	for (int i = 0; i < 3; i++)
	{
		robot objective;
		objective.x = players[6].x;
		if ((side == 1) && (players[i].y < meio_campo.y) || (side == 0) && (players[i].y < meio_campo.y))
		{
			objective.y = y_area_gol_min - 2;
		}
		else if ((side == 1) && (players[i].y > meio_campo.y) || (side == 0) && (players[i].y > meio_campo.y))
		{
			objective.y = y_area_gol_max + 2;
		}

		if (players[i].comportamento == 4)
			players[i].setObjective(objective);
	}
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

	for (int i = 0; i < 6; i++)
	{
		if (players[i].x != -1)
		{
			if (i != n)
				cv::circle(fieldI[n], cvPoint((int)players[i].x * reduce, (int)players[i].y * reduce), 7, 10, -1);
				//cout << "Adicionado um obstaculo em (" << (int)players[i + 3].x << ", " <<  (int)players[i + 3].y << ")" << endl;
		}
	}

	//cout << "Adicionado um obstaculo em (" << (int)players[6].x << ", " <<  (int)players[6].y << ")" << endl;

	float angle = atan2(ponto_gol_adv.y - players[6].y, ponto_gol_adv.x - players[6].x);
	if (players[6].x != -1)
	{
		cv::circle(fieldI[n], cvPoint((int)(players[6].x + 20 * cos(angle)) *reduce, (int)(players[6].y + 20 * sin(angle)) *reduce), 9, 40, -1);
		cv::circle(fieldI[n], cvPoint((int)players[6].x * reduce, (int)players[6].y *reduce), 5, 0, -1);
	}
	//cv::circle(fieldI[n], cvPoint(25, 25), 10, 4, -1);
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

	if ((goal.x < 0) && (width < goal.x) && (goal.y < 0) && (height < goal.y))
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
		if ((i == 0) || (i == copy.size()) || ((i % 3) == 0))
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
	float weight_data = 0.3;
	float weight_smooth = 0.4;
	float tolerance = 0.0001;
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
	if (distance(players[n], robot(path[n].back().x / reduce, path[n].back().y / reduce)) > 7)
		players[n].setGoal(path[n].back().x / reduce, path[n].back().y /reduce);
	else
	{
		path[n].pop_back();
		players[n].setGoal(path[n].back().x / reduce, path[n].back().y /reduce);
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
	float centro_xmin, centro_xmax, centro_ymin, centro_ymax, raio, d_PB1, d_PB2;

	for (int i = 0; i < 3; i++)
	{
		raio = 13;
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

	for (int i = 0; i < j; i++)
		threads.push_back(std::thread(&estrategia::obstaclesI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();

	for (int i = 0; i < j; i++)
		threads.push_back(std::thread(&estrategia::weightAStarI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();

	for (int i = 0; i < j; i++)
		threads.push_back(std::thread(&estrategia::minimizeRouteI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();

	//for (int i = 0; i < 3; i++)
	//	threads.push_back(std::thread(&estrategia::smoothPathI, this, i));
	//for (auto& th : threads) th.join();

	threads.clear();

	for (int i = 0; i < j; i++)
		threads.push_back(std::thread(&estrategia::nextGoalI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();
}

bool bola_na_area()
{
	if ((side == 1) && (players[6].x < x_area_gol2) && (players[6].y >= y_area_gol_min) && (players[6].y <= y_area_gol_max))
	{
		return 1;
	}
	else if ((side == 0) && (players[6].x > x_area_gol1) && (players[6].y >= y_area_gol_min) && (players[6].y <= y_area_gol_max))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}