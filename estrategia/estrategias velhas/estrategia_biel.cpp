#include "estrategia.h"


estrategia::estrategia()
{
	field = cvCreateImage(cvSize(270,102),IPL_DEPTH_8U,1);
	cvSet(field,cvScalar(0,0,0));
	cvCircle(field, cvPoint(270/2, 102/2), 10, cvScalar(128), -1);
	cvCircle(field, cvPoint(270/2 + 5, 102/2 + 5), 10, cvScalar(128), -1);
}

estrategia::~estrategia()
{



}

//robot ponto1, robot ponto2, robot ponto_f
float estrategia::distancia(robot ponto1, robot ponto2)
{
    float distancia = sqrt(pow(ponto1.x - ponto2.x, 2) + pow(ponto1.y - ponto2.y, 2));

    return distancia;
}

//robot ponto_bola, robot carro_1, robot carro_2, robot carro_3, robot carro_I_1, robot carro_I_2, robot carro_I_3
int estrategia::comportamento()
{
	///////////////////////////  SETANDO AS DISTANCIAS  //////////////////////////////////////

	/////////////////////////////////  CARRO_1 ///////////////////////////////////////////////
	distancia(robot carro_1, robot ponto_bola, robot robot ponto3);
	robot D1 = distancia_c;       // distancia carro_1 a bola e ao ponto que o goleiro devia estar

	/////////////////////////////////  CARRO_2 ///////////////////////////////////////////////
	distancia(robot carro_2, robot ponto_bola, robot robot ponto3);
	robot D2 = distancia_c;       // distancia carro_2 a bola e ao ponto que o goleiro devia estar

	/////////////////////////////////  CARRO_3 ///////////////////////////////////////////////
	distancia(robot carro_3, robot ponto_bola, robot robot ponto3);
	robot D3 = distancia_c;       // distancia carro_3 a bola e ao ponto que o goleiro devia estar

	/////////////////////////////////  CARRO_1 ADVERSARIO ///////////////////////////////////////////////
	distancia(robot carro_I_1, robot ponto_bola, robot robot ponto3);
	robot DA1 = distancia_c;       // distancia carro_1 adversario a bola e ao ponto que o goleiro devia estar

	/////////////////////////////////  CARRO_2 ADVERSARIO  ///////////////////////////////////////////////
	distancia(robot carro_I_2, robot ponto_bola, robot robot ponto3);
	robot DA2 = distancia_c;       // distancia carro_2 adversario a bola e ao ponto que o goleiro devia estar

	/////////////////////////////////  CARRO_3 ADVERSARIO  ///////////////////////////////////////////////
	distancia(robot carro_I_3, robot ponto_bola, robot robot ponto3);
	robot DA3 = distancia_c;       // distancia carro_3 adversario a bola e ao ponto que o goleiro devia estar

	///////////////////////////////////  DISTANCIAS SETADAS  /////////////////////////////////

	//Comportamento = 1 --> Goleiro
	//Comportamento = 2 --> Atacante
	//Comportamento = 3 --> Lateral

	/////////////////////////////////  SEM POSSE DE BOLA  /////////////////////////////////////////////////////
	if ((D2.y < D1.y) && (D1.y < D3.y) && (carro_1.posse != 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D3.y) && (D3.y < D1.y) && (carro_1.posse != 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D1.y) && (D1.y < D2.y) && (carro_1.posse != 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D2.y) && (D2.y < D1.y) && (carro_1.posse != 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	////////////////////////////////  COM POSSE DE BOLA  ///////////////////////////////////////////////////////
	else if ((D1.y < D2.y) && (D2.y < D3.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D1.y < D2.y) && (D2.y < D3.y) && (carro_1.posse = 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D1.y < D2.y) && (D2.y < D3.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D1.y < D3.y) && (D3.y < D2.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D1.y < D3.y) && (D3.y < D2.y) && (carro_1.posse = 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D1.y < D3.y) && (D3.y < D2.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D1.y) && (D1.y < D3.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D1.y) && (D1.y < D3.y) && (carro_1.posse != 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D1.y) && (D1.y < D3.y) && (carro_1.posse = 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D1.y) && (D1.y < D3.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D3.y) && (D3.y < D1.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D3.y) && (D3.y < D1.y) && (carro_1.posse != 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D3.y) && (D3.y < D1.y) && (carro_1.posse != 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D3.y) && (D3.y < D1.y) && (carro_1.posse = 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D2.y < D3.y) && (D3.y < D1.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D1.y) && (D1.y < D2.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D1.y) && (D1.y < D2.y) && (carro_1.posse != 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D1.y) && (D1.y < D2.y) && (carro_1.posse = 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D1.y) && (D1.y < D2.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D2.y) && (D2.y < D1.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D2.y) && (D2.y < D1.y) && (carro_1.posse != 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D2.y) && (D2.y < D1.y) && (carro_1.posse != 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D2.y) && (D2.y < D1.y) && (carro_1.posse = 1) && (carro_2.posse = 1) && (carro_3.posse != 1))
	{
		carro_3.comportamento = 1;
		if ((D1.x < D2.x))
		{
			carro_1.comportamento = 2;
			carro_2.comportamento = 3;
		}
		else
		{
			carro_2.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else if ((D3.y < D2.y) && (D2.y < D1.y) && (carro_1.posse = 1) && (carro_2.posse != 1) && (carro_3.posse = 1))
	{
		carro_2.comportamento = 1;
		if ((D1.x < D3.x))
		{
			carro_1.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_1.comportamento = 3;
		}
	}
	else
	{
		carro_1.comportamento = 1;
		if ((D2.x < D3.x))
		{
			carro_2.comportamento = 2;
			carro_3.comportamento = 3;
		}
		else
		{
			carro_3.comportamento = 2;
			carro_2.comportamento = 3;
		}
	}
}

//float x_goleiro, robot ponto_f
robot estrategia::goleiro(float x_goleiro)
{

	ponto_f.x = x_goleiro;
	float delta_x = x_goleiro - ponto_bola.x;

	if((delta_x < "MEIO DO CAMPO") && (ponto_bola.y >= 148))
	{
	    ponto_f.y = 148;
	}
	else if((delta_x < "MEIO DO CAMPO") && (ponto_bola.y < 148) && (ponto_bola.y > 64))
	{
	    ponto_f.y = ponto_bola.y;
	}
	else if((delta_x < "MEIO DO CAMPO") && (ponto_bola.y <= 64))
	{
	    ponto_f.y = 64;
	}
	else if((delta_x > "MEIO DO CAMPO") && (ponto_bola.y < 148) && (ponto_bola.y > 64))
	{
	    ponto_f.y = ponto_bola.y;
	}
	else
	{
	    ponto_f.y = 106;
	}

	return ponto_f;
}

//robot ponto_bola, float y_meio_de_campo, robot carro_a, float x_gol_I
robot estrategia::atacante()
{
	ponto_f.y = y_meio_de_campo;
	if ((side == 1) && carro_a.posse != 1)
	{
		ponto_f.x = ponto_bola.x - 10;
	}
	else if ((side == 0) && carro_a.posse != 1)
	{
		ponto_f.x = ponto_bola.x + 10;
	}
	else
	{
		ponto_f.x = x_gol_I;
	}

	return ponto_f;
}

//robot ponto_bola, robot carro_l, robot carro_a, robot meio_campo
robot estrategia::lateral()
{
	if ((carro_L.posse != 1) && (carro_a.posse != 1))
	{
		ponto_f = ponto_bola;
	}
	else if ((carro_L.posse != 1) && (carro_a.posse = 1))
	{
		ponto_f = meio_campo;
	}
	else
	{
		ponto_f = ponto_bola;
	}
}