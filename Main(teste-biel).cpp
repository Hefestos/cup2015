#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string>
#include"estrategia\estrategia.h"
#include <vector>
#include "control\control.h"
#include"serial\serial.h"
#include"vision\vision.h"
#include"robot\robot.h"
#include <time.h>
#include "Windows.h"
//#include "controle_novo\controle_novo.h"
//#include "controle_bili\controle_bili.h"

robot clicked;




//NameSpaces
using namespace cvb;
using namespace std;


//callback function
void mouseEvent(int evt, int x, int y, int flags, void* param){

	if (evt == CV_EVENT_LBUTTONDOWN){
		printf("%d %d\n", x, y);
		clicked.x = x;
		clicked.y = y;
		clicked.stop = FALSE;
	}if (evt == CV_EVENT_RBUTTONDOWN){
		clicked.stop = TRUE;
	}
}




int main()
{



	FILE * xFile;
	xFile = fopen("motor1x.csv", "w");

	FILE * yFile;
	yFile = fopen("motor1y.csv", "w");

	FILE * thetaFile;
	thetaFile = fopen("motor1theta.csv", "w");

	FILE * refFile;
	refFile = fopen("ref.csv", "w");

	FILE * f1File;
	f1File = fopen("f1.csv", "w");

	FILE * f2File;
	f2File = fopen("f2.csv", "w");

	FILE * gFile;
	gFile = fopen("total.csv", "w");




	cv::Mat pesos;
	IplImage* pesos3;
	pesos3 = cvCreateImage(cvSize(1, 1), 8, 3);
	// Config Debug
	clock_t visionI, visionF, estraI, estraF, controlI, controlF, commI, commF, totalI, totalF, debugI, debugF, start;
	vector<robot> whishPath, followedPath;
	int p = 0;
	float counter = 0;
	char char_pressed;
	int time = 1;
	int side = 1;
	bool paused = true;
	CvRect ROI = cvRect(0, 0, 0, 0);
	robot canto_area1 = robot(-1, -1);
	robot canto_area2 = robot(-1, -1);
	robot canto_gol1 = robot(-1, -1);
	robot canto_gol2 = robot(-1, -1);
	int amin = 21;
	int amax = 84;
	int radius = 20;
	int min_distance = 54;
	int parametro[36] = { 94, 143, 80, 121, 253, 25,
		23, 134, 126, 52, 255, 255,
		33, 81, 154, 180, 255, 255,
		55, 189, 91, 94, 229, 255,
		180, 255, 255, 0, 0, 0,
		4, 156, 102, 14, 255, 255 };





























	//CvRect ROI = cvRect(36, 46, 266, 162);

	//robot canto_area1 = robot(65, 67);
	//robot canto_area2 = robot(244, 167);
	//robot canto_gol1 = robot(43, 66);
	//robot canto_gol2 = robot(282, 173);

	//int amin = 20;
	//int amax = 250;
	//int radius = 20;
	//int min_distance = 50;
	//int parametro[36] = { 88, 139, 80, 121, 253, 255,
	//	22, 59, 141, 55, 204, 255,
	//	117, 58, 113, 141, 113, 255,
	//	52, 82, 89, 97, 168, 169,
	//	0, 79, 123, 19, 149, 191,
	//	0, 129, 182, 18, 251, 255 };


	int debug_colors[7][3] = { { 128, 0, 128 },		// Roxo
	{ 0, 64, 0 },									// Verde
	{ 117, 58, 0 },									// Marrom
	{ 255, 255, 0 },								// Amarelo
	{ 255, 255, 0 },								// Amarelo
	{ 255, 255, 0 },								// Amarelo
	{ 255, 128, 0 } };								// Laranja

	if (time == 1)
	{
		for (int k = 0; k < 3; k++)
		{
			debug_colors[3 + k][0] = 0;			// Azul
			debug_colors[3 + k][1] = 0;			// Azul
			debug_colors[3 + k][2] = 255;		// Azul
		}
	}


	// Config Controle

	// Parametros do PID na seguinte ordem: PF, IF, DF, PT, IT, DT, PFR, IFR, DFR, PTR, ITR, DTR

	float param0[12] = { 11.45, 1.49, 26.9, 13.59, 1.63, 25.86, 1.03, 0.14, 9.36, 1.4, 0.23, 9.44 };
	float param1[12] = { 25.72, 1.01, 20.07, 22.34, 0.94, 21.1, 10, 0.64, 4.92, 10, 0.61, 6.92 };
	float param2[12] = { 21.86, 1.06, 18, 22, 1.03, 25.03, 3.59, 0.32, 4.94, 2.9, 0.89, 3.79 };

	CvFont fonte;
	cvInitFont(&fonte, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8);


	cvNamedWindow("Live", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("Filtro Atual");
	//cvNamedWindow("Estrategia");

	// Inicialização da Visão
	vision Camera;
	Camera.createCamera(0);
	Camera.getImage(ROI);
	Camera.configParam(parametro, amin, amax, time, min_distance, ROI);

	// Inicialização da Estratégia
	estrategia Estrategia(Camera.h, Camera.w, side, canto_gol1, canto_gol2, canto_area1, canto_area2);
	Estrategia.Config(Camera.h, Camera.w);
	pesos = cvCreateImage(cvGetSize(Camera.frame), Camera.frame->depth, 1);
	Estrategia.ConfigAreas(canto_gol1, canto_gol2, canto_area1, canto_area2);

	// Inicialização do Controle
	

	controle_bili Controle_bili;

	// Inicialização do Serial
	Serial* SP = new Serial("COM4");
	if (SP->IsConnected())
		printf("We're connected\n");




	//============================== VARIÁVEIS  DE TESTE ============================//

	int contador = 0;
	float t;
	float ref = 0.0, T = 40000.0, f1 = 0.0, f2 = 0.0, torque = 0.0, fx_B = 0.0, d = 3.75, KP_w = 0.0, KI_w = 0.0, flag = 0.0, K, KP_u, R = 55.0, a, dist = 0.0;

	//float x = Camera.players[0].x,e =0.0;
	//float y = Camera.players[0].y;
	Camera.players[0].theta_acumulated = 0;
	Camera.players[0].theta_old = 0;
	//float theta = Camera.players[0].theta;
	//float t = visionF - start;
	//float v1 = 15;
	//float v2 = -15;
	float c, alpha = 0.0, biel = 0.0, v1_send = 0.0, v2_send = 0.0;
	float dx0, dy0, Kp, /*v1 = 0.0, v2 = 0.0,*/ theta2 = 0.0, thetad = 0.0, Kt, refaux = 0.0, i = 0.0, o = 0.0, ang_error = 0.0;
	float ct, st, erro_angulo = 0.0, x_bola = 0.0, y_bola = 0.0, thetad_old = 0.0, u_i = 0.0, KI_u = 0.0;
	float w = 0.0, u = 0.0, w_i = 0.0, w_p = 0.0, h = 0.0, wc = 0.0, uc = 7.0, f = 300.0,  u_p = 0.0, x_0 = 155, y_0 = 116;
	h = 1.0 / 12.0;
	int xd = 0, yd = 0;
	char temp[2];
	char tempread[2];
	//char msg[12];
	//byte sum;
	float xd_old = 0.0, yd_old = 0.0;
	float n_xd = Camera.players[0].x, n_yd = Camera.players[0].y;
	start = clock();
	Camera.players[0].refaux_acumulated = 0.0;
	Camera.players[0].refaux_old = 0.0;
	Camera.players[0].refaux = 0.0;
	int contar = 0, inicio_voltas = 0.0;

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 256;
	int readResult = 0;

	double t1 = 0;
	double tChecksum_IN;
	double tChecksum_OUT;
	//double tRecieve_IN;
	double tRecieve_OUT;
	int loop = 0;

	byte sum = 0;
	byte recievedCheckSum = 0;
	int left = 0;
	int right = 0;
	int recievedValue = 0;
	float vADC = 0;
	float voltagem = 0;
	int ze = 255;
	int newV1 = 0;
	int newV2 = 0;
	//	int loop;

	int change_loop = 1;	// o valor dessea variavel fica entre 1 e 6, ela escolhe qual loop estaremos usando
	char point		= 'i';	// i-> ponto inicial do teste, f-> ponto final do teste
	char param		= '0';  // 0-> proporcional, 1-> integral, 2-> derivativo
	int step_flag	= 1;	// 1-> passo alto, 2-> passo medio, 3-> passo baixo, 4-> achamos o valor "ótimo";
	int analise		= 0;	// 0->análise do ts, 1->análise do overshoot, 2->análise do upertime

	//=========================== INÍCIO DO LOOP ==============================//
	while (true)
	{
		//====================Leitura Mensagem================================ //
		readResult = SP->ReadData(incomingData, 11);

		/*for (int i = 0; i<11; i++)
		{
		cout << dec << i << ": ";
		cout << hex << (int)incomingData[i] << endl;
		//cout << "haha" << endl;
		//system("pause");

		}*/

		//O endereço do xbee de onde veio a mensagem é incomingData[4] e incomingData[5]
		if (incomingData[0] == 0x7e && incomingData[1] == 0x00 && incomingData[2] == 0x07 && incomingData[3] == -127 && incomingData[4] == -86 && incomingData[5] == -86 && incomingData[7] == 0)
		{
			//cout << "_-_-_-_-_-_" << endl;

			sum = 0;
			recievedCheckSum = 0;
			left = 0;
			right = 0;
			recievedValue = 0;
			voltagem = 0;
			vADC = 0;
			//loop = 0;

			for (int i = 0; i < 11; i++)
			{
				if (i>2 && i < 10)
				{
					sum = sum + incomingData[i];
				}
			}
			sum = 0xFF - sum;
			sum = 0xFF & sum;

			if (incomingData[10] & 0x111111)
			{
				recievedCheckSum = (int)(incomingData[10] - 0xffffff00);
				//cout << "HAHA" << endl;
			}
			else
			{
				recievedCheckSum = (int)(incomingData[10]);
			}

			if (recievedCheckSum == sum)
			{
				//cout << "Mensagem Valida" << endl;
				//system("cls");
				//cout << "left: " << hex << (int)incomingData[8] << endl;
				//cout << "right: " << hex << (int)incomingData[9] << endl;
				//cout << "check: " << dec << ((int)incomingData[9]) << endl;
				if (incomingData[8] & 0x1111111)
				{
					//cout << "caso 1" << endl;
					left = (int)(incomingData[8] - 0xffffff00);
					left = left - 256;

				}
				else if (incomingData[8] & 0x111111)
				{
					//cout << "caso 2" << endl;
					left = (int)(incomingData[8] - 0xffffff00);
					//left = left -256;
				}
				else
				{
					//cout << "caso 3" << endl;
					left = (int)(incomingData[8]);

				}


				//cout << dec << (int)(incomingData[9]) << endl;
				if (incomingData[9] & 0x1111111)
				{
					right = (int)(incomingData[9] - 0xffffff00);
					if ((int)incomingData[9] > 0)
					{
						right = right - 256;
					}
					//right = right - 256;
					//cout << "caso 1" << endl;

				}
				else if (incomingData[9] & 0x111111)
				{
					right = (int)(incomingData[9] - 0xffffff00);
					//left = left -256;
					//cout << "caso 2" << endl;
				}
				else
				{
					right = (int)(incomingData[9]);
					//cout << "caso 3" << endl;

				}


				/*if (incomingData[9] & 0x111111)
				{
				right = (int)(incomingData[9] - 0xffffff00);
				}
				else
				{
				right = (int)(incomingData[9]);
				}*/

				//cout <<"esquerda: " << left<< endl;
				//cout << "direita: " << right << endl;
				recievedValue = 768 + right;
				vADC = recievedValue * 0.00489;
				voltagem = 0.013414862*recievedValue;

				/*
				cout << "Total: " << dec << recievedValue << endl;
				cout << "ADC: " << dec << vADC << endl;
				printf("Voltagem: %0.2f V \n\n", voltagem);
				cout << loop++ << endl;
				*/

				//cout << "Tempo receber: " << clock() - t1 << "ms" << endl;
				//cout << "Recieve: " << tRecieve_OUT - t1 << "ms" << endl;
				//cout << "Checksum: " << tChecksum_IN - tChecksum_IN << "ms" << endl;
				//cout << loop++ << endl;


				/*if (recievedValue < 1024)
				{
				system("cls");
				voltagem = 0.0135*recievedValue;
				cout << "Total: " << dec << recievedValue << endl;
				printf("Voltagem: %0.2f V \n\n", voltagem);
				cout << "Tempo receber: " << clock() - t1 << "ms" <<endl;
				cout << "Recieve: " << tRecieve_OUT - t1 << "ms" << endl;
				cout << "Checksum: " << tChecksum_IN - tChecksum_IN << "ms" << endl;
				cout << loop++ << endl;
				}*/

				//cout << "Voltagem: " << dec << voltagem << endl;

			}
		}
		else //=== Mensagem Inválida 
		{

			char incomingData[256] = "";
		}

		/*for (int i = 0; i < 11; i++)
		{
		cout << i << ": " << hex << (int)incomingData[i] << endl;
		}*/


		//cout << endl << endl << endl << endl;

		//cout << char(incomingData[6]) << endl;

		//std::string test(incomingData);


		//printf("%s", incomingData);
		//test = "";



		//==================================================================== //
		
		
		
		
		
		//cout << "theta  inicio = " << Camera.players[0].theta << endl;

		counter++;
		totalI = clock();



		// =========================== VISÃO =========================== //
		visionI = clock();

		Camera.getImage();
		Camera.dataAcquire();
		Camera.players[1] = robot(-1, -1);
		Camera.players[2] = robot(-1, -1);


		visionF = clock();



		// =========================== ESTRATEGIA =========================== //

		estraI = clock();

		//Atribuição de comportamentos
//		Estrategia.update(Camera.players);

//		Estrategia.comportamento3();
//		Estrategia.Posse();
		//Estrategia.players[0].comportamento = 3;
		//Estrategia.players[1].comportamento = 3;
		//Estrategia.players[2].comportamento = 2;

		// Atribuição de objetivos
//		Estrategia.goleiro4();
//		Estrategia.lateral();
//		Estrategia.atacante2();
//		Estrategia.zagueiro();


		//Estrategia.generalRouting();


		//Estrategia.players[0].setGoal(23, 98);
		//Estrategia.players[1].setGoal(23, 98);


		estraF = clock();



		//  =========================== CONTROLE =========================== //
		/*controlI = clock();
		Controle00.controla(Camera.players[0], robot(Estrategia.players[0].goal_x, Estrategia.players[0].goal_y), robot(Estrategia.players[0].objective_x, Estrategia.players[0].objective_y));
		Controle01.controla(Camera.players[1], robot(Estrategia.players[1].goal_x, Estrategia.players[1].goal_y), robot(Estrategia.players[1].objective_x, Estrategia.players[1].objective_y));
		Controle02.controla(Camera.players[2], robot(Estrategia.players[2].goal_x, Estrategia.players[2].goal_y), robot(Estrategia.players[2].objective_x, Estrategia.players[2].objective_y));
		controlF = clock();*/

		// =========================== COMUNICAÇÃO =========================== //
		commI = clock();


		


		//if (!clicked.stop)
		//{
		//	xd = clicked.x  -x_0;
		//	yd = -(clicked.y - y_0);
		//}

		//if (clicked.stop)
		//{
		//	xd = Camera.players[0].x - x_0;
		//	yd = -(Camera.players[0].y- y_0);
		//}
		
		


		
		//xd = Camera.players[6].x - x_0;
		//yd = -(Camera.players[6].y - y_0);
		//yd = sin((6.28 / T) * t)*R;
		//xd = cos((6.28 / T) * t)*R;

		/*if (Camera.players[6].x == -1){
			xd = xd_old;
			yd = yd_old;
		}
		else{
			xd_old = xd;
			yd_old = yd;
		}*/


		///// -------------------------------------- MODIFICAÇÃO BIEL -----------------------------------------------------/////

		/* 
		VERIFICAR PRECISAO DAS VARIAVEIS DE REFERÊNCIA -> PARA SABER SE AS CONDIÇÕES SERÃO SATISFEITAS
		VERIFICAR QUAIS SÃO AS VARIAVEIS DE REFERÊNCIA
		*/

		// Apartir daqui procuraremos os ganho para o ajuste angular
		if (change_loop == 1)
		{
			/*Loop para fazer o carro ir ate o ponto inicial do teste*/
			xd = 120;	//valor de x de um extremo do campo
			yd = -9;	//valor de y de um extremo do campo

			if ((Camera.players[0].x <= xd + 12) && (Camera.players[0].x >= xd - 12) && (Camera.players[0].y <= yd + 12) && (Camera.players[0].y <= yd + 12))
			{
				thetad = 0.0;
				if ((Camera.players[0].theta <= thetad + 0.09) && (Camera.players[0].theta >= thetad - 0.09))
				{
					change_loop = 2;
					point = 'f';
				}
			}
		}
		else if (change_loop == 2)
		{
			if (point == 'f')
			{
				/*Loop para fazer o teste com os valores inicias dos parametros do PID*/
				thetad = 60.0;

				// Nesse caso testaremos os ganhos de rotação
				Camera.players[0].gain[0] = param0[3];
				Camera.players[0].gain[1] = param0[4];
				Camera.players[0].gain[2] = param0[5];

				Camera.players[0].group_theta.push_back(Camera.players[0].theta);

				if ((Camera.players[0].theta <= thetad + 0.05) && (Camera.players[0].theta <= thetad + 0.05))
				{
					Camera.players[0].condition_theta[0] = Controle_bili.find_ts('t', Camera.players[0], Camera.players[0].group_theta.size());
					Camera.players[0].condition_theta[1] = Controle_bili.find_overshoot('t', Camera.players[0], Camera.players[0].group_theta.size());
					Camera.players[0].condition_theta[2] = Controle_bili.find_upertime('t', Camera.players[0], Camera.players[0].group_theta.size());
					point = 'i';
				}
			}
			else if (point == 'i')
			{
				thetad = 0.0;
				if ((Camera.players[0].x <= thetad + 0.09) && (Camera.players[0].x >= thetad - 0.09))
				{
					change_loop = 3;
					point = 'f';

					// O teste começara com o proporcional rotacional
					param0[3] += 0.5;
					Camera.players[0].group_theta.clear();

					// Armazenamos os ganhos atuais, nos ganhos anteriores e atualizamos os ganhos atuais
					Camera.players[0].old_gain[0] = Camera.players[0].gain[0];
					Camera.players[0].old_gain[1] = Camera.players[0].gain[1];
					Camera.players[0].old_gain[2] = Camera.players[0].gain[2];

					Camera.players[0].gain[0] = param0[3];
					Camera.players[0].gain[1] = param0[4];
					Camera.players[0].gain[2] = param0[5];
				}
			}
		}
		else if (change_loop == 3)
		{
			if (point == 'f')
			{
				/*Loop para fazer o teste com os valores inicias dos parametros do PID*/
				thetad = 0.0;
				Camera.players[0].group_theta.push_back(Camera.players[0].theta);

				if ((Camera.players[0].theta <= thetad + 0.09) && (Camera.players[0].theta <= thetad + 0.09))
				{
					// Apartir daqui atribuiremos os valores anteriores de ts, overshoot e upertime
					Camera.players[0].condition_theta[3] = Camera.players[0].condition_theta[0];
					Camera.players[0].condition_theta[4] = Camera.players[0].condition_theta[1];
					Camera.players[0].condition_theta[5] = Camera.players[0].condition_theta[2];
					// Término das atribuições

					//Começamos a medir os novos valores de  ts, overshoot e upertime
					Camera.players[0].condition_theta[0] = Controle_bili.find_ts('t', Camera.players[0], Camera.players[0].group_theta.size());
					Camera.players[0].condition_theta[1] = Controle_bili.find_overshoot('t', Camera.players[0], Camera.players[0].group_theta.size());
					Camera.players[0].condition_theta[2] = Controle_bili.find_upertime('t', Camera.players[0], Camera.players[0].group_theta.size());
					// Término das novas atribuições

					float wanted_ts = 50.0;	// Não sei o valor de ts,  chutei como 50
					float wanted_overshoot = 50.0;	// Não sei o valor de overshoot,  chutei como 50
					float wanted_upertime = 50.0;	// Não sei o valor de upertime,  chutei como 50

					switch (param)					// O switch é para sabermos qual ganho estamos analisando no momento
					{
					case '0':
						// Ganho proporcional
						if (analise == 0)		//se igual a zero fazemos a analise de ts
						{
							step_flag = Controle_bili.improve_p_ts(Camera.players[0], 't', wanted_ts, step_flag);
						}
						else if (analise == 1)	//se igual a 1 fazemos a anlise de overshoot
						{
							step_flag = Controle_bili.improve_p_overshoot(Camera.players[0], 't', wanted_overshoot, step_flag);
						}
						else if (analise == 2)	// se igual a 2 fazemos a analise do tempo de subida (upertime)
						{
							step_flag = Controle_bili.improve_p_upertime(Camera.players[0], 't', wanted_upertime, step_flag);
						}
						if (step_flag == 4)
						{
							param = 1;
						}
						else
						{
							point = 'i';
						}
						param0[3] = Camera.players[0].gain[0];
						break;
					case '1':
						// Ganho integral
						if (analise == 0)		//se igual a zero fazemos a analise de ts
						{
							step_flag = Controle_bili.improve_i_ts(Camera.players[0], 't', wanted_ts, step_flag);
						}
						else if (analise == 1)	//se igual a 1 fazemos a anlise de overshoot
						{
							step_flag = Controle_bili.improve_i_overshoot(Camera.players[0], 't', wanted_overshoot, step_flag);
						}
						else if (analise == 2)	// se igual a 2 fazemos a analise do tempo de subida (upertime)
						{
							step_flag = Controle_bili.improve_i_upertime(Camera.players[0], 't', wanted_upertime, step_flag);
						}
						if (step_flag == 4)
						{
							param = 2;
						}
						else
						{
							point = 'i';
						}
						param0[4] = Camera.players[0].gain[1];
						break;
					case '2':
						// Ganho derivativo
						step_flag = Controle_bili.improve_d_ts(Camera.players[0], 't', wanted_ts, step_flag);
						if ((step_flag == 4) && (analise == 0))
						{
							param = 1;
							analise = 1;
						}
						else if ((step_flag == 4) && (analise == 1))
						{
							param = 1;
							analise = 2;
						}
						else if ((step_flag == 4) && (analise == 2))
						{
							change_loop = 4;
						}
						else
						{
							point = 'i';
						}
						param0[5] = Camera.players[0].gain[2];
						break;
					}
					point = 'i';
				}
			}
			else if (point == 'i')
			{
				thetad = 0.0;
				if ((Camera.players[0].theta <= thetad + 0.09) && (Camera.players[0].theta >= thetad - 0.09))
				{
					point = 'f';
					Camera.players[0].group_theta.clear();

					// Armazenamos os ganhos atuais, nos ganhos anteriores e atualizamos os ganhos atuais
					Camera.players[0].old_gain[0] = Camera.players[0].gain[0];
					Camera.players[0].old_gain[1] = Camera.players[0].gain[1];
					Camera.players[0].old_gain[2] = Camera.players[0].gain[2];

					Camera.players[0].gain[0] = param0[0];
					Camera.players[0].gain[1] = param0[1];
					Camera.players[0].gain[2] = param0[2];
				}
			}
		}

		// Apartir daqui procuraremos os ganho para o ajuste linear
		// O AJUSTE SERÁ FEITO POR UMA MOVIMENTAÇÃO NO EIXO X LOGO ALTERAREMOS OS GANHOS BASEADOS NESSE EIXO
		else if (change_loop == 4)
		{
			/*Loop para fazer o carro ir ate o ponto inicial do teste*/
			xd = 120;	//valor de x de um extremo do campo
			yd = -9;	//valor de y de um extremo do campo

			if ((Camera.players[0].x <= xd + 12) && (Camera.players[0].x >= xd - 12) && (Camera.players[0].y <= yd + 12) && (Camera.players[0].y <= yd + 12))
			{
				thetad = 0.0;  
				if ((Camera.players[0].theta <= thetad + 0.09) && (Camera.players[0].theta >= thetad - 0.09))
				{
					change_loop = 5;
					point = 'f';
				}
			}
		}
		else if (change_loop == 5)
		{
			if (point == 'f')
			{
				/*Loop para fazer o teste com os valores inicias dos parametros do PID*/
				xd = 155;	//valor de x do centro do campo
				yd = -116;	//valor de y do centro do campo
			
				// Nesse caso testaremos os ganhos de deslocamento linear
				Camera.players[0].gain[0] = param0[0];
				Camera.players[0].gain[1] = param0[1];
				Camera.players[0].gain[2] = param0[2];
				
				Camera.players[0].group_x.push_back(Camera.players[0].x);
				//Camera.players[0].group_y.push_back(Camera.players[0].y);

				if ((Camera.players[0].x <= xd + 12) && (Camera.players[0].x >= xd - 12) && (Camera.players[0].y <= yd + 12) && (Camera.players[0].y <= yd + 12))
				{
					Camera.players[0].condition_x[0] = Controle_bili.find_ts('x', Camera.players[0], Camera.players[0].group_x.size());
					//Camera.players[0].condition_y[0]	 = control.find_ts('y', Camera.players[0], Camera.players[0].group_y.size());

					Camera.players[0].condition_x[1] = Controle_bili.find_overshoot('x', Camera.players[0], Camera.players[0].group_x.size());
					//Camera.players[0].condition_y[1]	 = control.find_overshoot('y', Camera.players[0], Camera.players[0].group_y.size());

					Camera.players[0].condition_x[2] = Controle_bili.find_upertime('x', Camera.players[0], Camera.players[0].group_x.size());
					//Camera.players[0].condition_y[2]	 = control.find_upertime('y', Camera.players[0], Camera.players[0].group_y.size());
					point = 'i';
				}
			}
			else if (point == 'i')
			{
				xd = 120;	//valor de x de um extremo do campo
				yd = -9;	//valor de y de um extremo do campo

				if ((Camera.players[0].x <= xd + 12) && (Camera.players[0].x >= xd - 12) && (Camera.players[0].y <= yd + 12) && (Camera.players[0].y <= yd + 12))
				{
					thetad = 0.0;
					if ((Camera.players[0].theta <= thetad + 0.09) && (Camera.players[0].theta >= thetad - 0.09))
					{
						change_loop = 6;
						point = 'f';
						
						// O teste começara com o proporcional linear
						param0[0] += 0.5;
						Camera.players[0].group_x.clear();
						//Camera.players[0].group_y.clear();

						// Armazenamos os ganhos atuais, nos ganhos anteriores e atualizamos os ganhos atuais
						Camera.players[0].old_gain[0] = Camera.players[0].gain[0];
						Camera.players[0].old_gain[1] = Camera.players[0].gain[1];
						Camera.players[0].old_gain[2] = Camera.players[0].gain[2];

						Camera.players[0].gain[0] = param0[0];
						Camera.players[0].gain[1] = param0[1];
						Camera.players[0].gain[2] = param0[2];
					}
				}
			}
		}
		else if (change_loop == 6)
		{
			if (point == 'f')
			{
				/*Loop para fazer o teste com os valores inicias dos parametros do PID*/
				xd = 155;	//valor de x do centro do campo
				yd = -116;	//valor de y do centro do campo
				Camera.players[0].group_x.push_back(Camera.players[0].x);
				//Camera.players[0].group_y.push_back(Camera.players[0].y);

				if ((Camera.players[0].x <= xd + 12) && (Camera.players[0].x >= xd - 12) && (Camera.players[0].y <= yd + 12) && (Camera.players[0].y <= yd + 12))
				{
					// Apartir daqui atribuiremos os valores anteriores de ts, overshoot e upertime
					Camera.players[0].condition_x[3]	 = Camera.players[0].condition_x[0];
					//Camera.players[0].condition_y[3]	 = Camera.players[0].condition_y[0];

					Camera.players[0].condition_x[4]	 = Camera.players[0].condition_x[1];
					//Camera.players[0].condition_y[4]	 = Camera.players[0].condition_y[1];

					Camera.players[0].condition_x[5]	 = Camera.players[0].condition_x[2];
					//Camera.players[0].condition_y[5]	 = Camera.players[0].condition_y[2];
					// Término das atribuições

					//Começamos a medir os novos valores de  ts, overshoot e upertime
					Camera.players[0].condition_x[0] = Controle_bili.find_ts('x', Camera.players[0], Camera.players[0].group_x.size());
					//Camera.players[0].condition_y[0]	 = control.find_ts('y', Camera.players[0], Camera.players[0].group_y.size());

					Camera.players[0].condition_x[1] = Controle_bili.find_overshoot('x', Camera.players[0], Camera.players[0].group_x.size());
					//Camera.players[0].condition_y[1]	 = control.find_overshoot('y', Camera.players[0], Camera.players[0].group_y.size());

					Camera.players[0].condition_x[2] = Controle_bili.find_upertime('x', Camera.players[0], Camera.players[0].group_x.size());
					//Camera.players[0].condition_y[2]	 = control.find_upertime('y', Camera.players[0], Camera.players[0].group_y.size());
					// Término das novas atribuições

					float wanted_ts = 50.0;	// Não sei o valor de ts,  chutei como 50
					float wanted_overshoot = 50.0;
					float wanted_upertime = 50.0;
					
					// OBS: COMO FAZER PARA ACHAR GANHOS DE X E Y E JUNTA-LOS NUM MOVIMENTO LINEAR?????????????

					switch (param)// O switch é para sabermos qual ganho estamos analisando no momento
					{
					case '0':
						// Ganho proporcional
						if (analise == 0)		//se igual a zero fazemos a analise de ts
						{
							step_flag = Controle_bili.improve_p_ts(Camera.players[0], 'X', wanted_ts, step_flag);
						}
						else if (analise == 1)	//se igual a 1 fazemos a anlise de overshoot
						{
							step_flag = Controle_bili.improve_p_overshoot(Camera.players[0], 'X', wanted_overshoot, step_flag);
						}
						else if (analise == 2)	// se igual a 2 fazemos a analise do tempo de subida (upertime)
						{
							step_flag = Controle_bili.improve_p_upertime(Camera.players[0], 'X', wanted_upertime, step_flag);
						}
						if (step_flag == 4)
						{
							param = 1;
						}
						else
						{
							point = 'i';
						}
						param0[0] = Camera.players[0].gain[0];
						break;
					case '1':
						// Ganho integral
						if (analise == 0)		//se igual a zero fazemos a analise de ts
						{
							step_flag = Controle_bili.improve_i_ts(Camera.players[0], 'X', wanted_ts, step_flag);
						}
						else if (analise == 1)	//se igual a 1 fazemos a anlise de overshoot
						{
							step_flag = Controle_bili.improve_i_overshoot(Camera.players[0], 'X', wanted_overshoot, step_flag);
						}
						else if (analise == 2)	// se igual a 2 fazemos a analise do tempo de subida (upertime)
						{
							step_flag = Controle_bili.improve_i_upertime(Camera.players[0], 'X', wanted_upertime, step_flag);
						}
						if (step_flag == 4)
						{
							param = 2;
						}
						else
						{
							point = 'i';
						}
						param0[1] = Camera.players[0].gain[1];
						break;
					case '2':
						// Ganho derivativo
						step_flag = Controle_bili.improve_d_ts(Camera.players[0], 'x', wanted_ts, step_flag);
						if ((step_flag == 4) && (analise == 0))
						{
							param = 1;
							analise = 1;
						}
						else if ((step_flag == 4) && (analise == 1))
						{
							param = 1;
							analise = 2;
						}
						else if ((step_flag == 4) && (analise == 2))
						{
							change_loop = 7;
						}
						else
						{
							point = 'i';
						}
						break;
					}

					point = 'i';
				}
			}
			else if (point == 'i')
			{
				xd = 120;	//valor de x de um extremo do campo
				yd = -9;	//valor de y de um extremo do campo

				if ((Camera.players[0].x <= xd + 12) && (Camera.players[0].x >= xd - 12) && (Camera.players[0].y <= yd + 12) && (Camera.players[0].y <= yd + 12))
				{
					thetad = 0.0;
					if ((Camera.players[0].theta <= thetad + 0.09) && (Camera.players[0].theta >= thetad - 0.09))
					{
						point = 'f';
						Camera.players[0].group_x.clear();
						//Camera.players[0].group_y.clear();

						// Armazenamos os ganhos atuais, nos ganhos anteriores e atualizamos os ganhos atuais
						Camera.players[0].old_gain[0] = Camera.players[0].gain[0];
						Camera.players[0].old_gain[1] = Camera.players[0].gain[1];
						Camera.players[0].old_gain[2] = Camera.players[0].gain[2];

						Camera.players[0].gain[0] = param0[0];
						Camera.players[0].gain[1] = param0[1];
						Camera.players[0].gain[2] = param0[2];
					}
				}
			}
		}
		else if (change_loop == 7)
		{
			cout << "Os ganhos são :" << endl;
			cout << "GPL " << param0[0] << endl;
			cout << "GIL " << param0[1] << endl;
			cout << "GDL " << param0[2] << endl;
			cout << "GPR " << param0[3] << endl;
			cout << "GIR " << param0[4] << endl;
			cout << "GDR " << param0[5] << endl;
		}


			
		///// -------------------------------------- TÉRMINO DA MODIFICAÇÃO BIEL -----------------------------------------------------/////


		Controle_bili.rotina_controle(0, 0, &Camera, xd, yd);
		newV2 = (0.000001947*pow(recievedValue, 2) - 0.004673*recievedValue + 3.495)*abs(Controle_bili.v2 * 255);
		newV1 = (0.000001947*pow(recievedValue, 2) - 0.004673*recievedValue + 3.495)*abs(Controle_bili.v1 * 255);
		//SP->sendApi(0, Controle_bili.o, abs(Controle_bili.v2 * 255), Controle_bili.i, abs(Controle_bili.v1 * 255));
		SP->sendApi(0, Controle_bili.o, newV2, Controle_bili.i, newV1);



		if (Camera.players[0].x != -1)
			fprintf(gFile, "%f,%f,%f, %i, %f, %f,%f,%f,%f,%f,%f, %f\n", Controle_bili.x*1.3, Controle_bili.y*1.3, Controle_bili.theta, int(visionF - start), Controle_bili.thetad, Controle_bili.v1, Controle_bili.v2, w, xd*1.3, yd*1.3, u, Controle_bili.refaux);

		//SP->sendApi(0, 1, 84, 0, 0);
		//=================== DISPLAY DOS DADOS DO ROBO ============//

		cout << "thetad = " << Controle_bili.thetad*180/M_PI << endl;
		cout << "theta = " << Controle_bili.theta * 180 / M_PI << endl;
		cout << "ang_error = " << Controle_bili.ang_error * 180 / M_PI << endl;
		//cout << "ypc = " << dy0 << endl;
		cout << "v1 = " << Controle_bili.v1 << endl;
		cout << "v2 = " << Controle_bili.v2 << endl;
		//cout << "u_p = " << Controle_bili.u_p << endl;
		
		

		
		//======================== ENVIO DAS VELOCIDADES ====================//

		if (!paused)
		{
			//SP->send(0, v2, v1);
			//SP->send(v2, v1);
			//SP->send(0, Controle00.v.x, Controle00.v.y);
			// whishPath.push_back(robot(Estrategia.players[0].goal_x, Estrategia.players[0].goal_y));
			//sfollowedPath.push_back(Camera.players[0]);
			//SP->send(1, Controle01.v.x, Controle01.v.y);
			//SP->send(2, Controle02.v.x, Controle02.v.y);
		}
		else
		{
			/*
			SP->send(0, 0, 0);
			SP->send(1, 0, 0);
			SP->send(2, 0, 0);
			*/
		}
		commF = clock();


		// Debug em prompt de comando
		debugI = clock();

		//system("cls");
		//cout << "Roxo: " << Estrategia.players[0].x << ", " << Estrategia.players[0].y << " --> " << Estrategia.players[0].goal_x << ", " << Estrategia.players[0].goal_y << endl;
		//cout << "Verde: " << Estrategia.players[1].x << ", " << Estrategia.players[1].y << " --> " << Estrategia.players[1].goal_x << ", " << Estrategia.players[1].goal_y << endl;
		//cout << "Marrom: " << Estrategia.players[2].x << ", " << Estrategia.players[2].y << " --> " << Estrategia.players[2].goal_x << ", " << Estrategia.players[2].goal_y << endl;

		//cout << "Carrinho: (" << Camera.players[0].x << ", " << Camera.players[0].y << ", " << Camera.players[0].theta *(180/M_PI) << ")" << endl;
		//cout << "Objetivo: (" << (int) Estrategia.ponto_f.x << ", " << (int) Estrategia.ponto_f.y << ")" << endl;
		//cout << "Velocidade (d, e): " << Controle00.v.x << ", "<< Controle00.v.y << ")" <<endl;



		// Debug em janela

		cvSetMouseCallback("Live", mouseEvent, 0);

		// Desenhando nas janelas
//		cv::resize(Estrategia.fieldI[1], pesos, cv::Size(Camera.frame->width, Camera.frame->height), 0, 0);
//		cvReleaseImage(&pesos3);

		pesos3 = cvCreateImage(cvGetSize(Camera.frame), Camera.frame->depth, Camera.frame->nChannels);
		cvMerge(&(IplImage)pesos, &(IplImage)pesos, &(IplImage)pesos, NULL, pesos3);
		//cvAddWeighted(pesos3, 0.9, Camera.frame, 0.8, 80, Camera.frame);

		for (int i = 0; i < 7; i++)
		{
			if (Camera.players[i].x != -1)
			{
				cvCircle(Camera.frame, cvPoint((int)Camera.players[i].x, (int)Camera.players[i].y), radius, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
				cvLine(Camera.frame, cvPoint((int)Camera.players[i].x, (int)Camera.players[i].y), cvPoint((int)Camera.players[i].x + radius*cos(Camera.players[i].theta),
					(int)Camera.players[i].y + radius*sin(Camera.players[i].theta)), CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
			}
		}



		cout << "nxd: " << xd << " nyd: " << yd << endl;
		cvCircle(Camera.frame, cvPoint(xd + 155, -yd + 116), 5, CV_RGB(0, 255, 0), -1);

		
		cvShowImage("Live", Camera.frame);



		//Escape Sequence
		char_pressed = cvWaitKey(1);
		if (char_pressed == 27) //esc
		{

			temp[0] = 0x80;
			temp[1] = 0x80;
			SP->WriteData(temp, 2);
			SP->sendApi(0, 0, 0, 0, 0);
			SP->sendApi(0, 0, 0, 0, 0);
			SP->sendApi(0, 0, 0, 0, 0);

			break;


		}
		if (char_pressed == 112) //p
		{
			if (paused)
				paused = false;
			else{
				paused = true;
				flag = 1.0;
				//thetad = alpha;
			}
		}
		if (char_pressed == 99) //c
		{
			whishPath.clear();
			followedPath.clear();
		}
		if (char_pressed == 105) //i
		{

			//e += 1;

		}
		if (char_pressed == 111) //o
		{

			//e -= 1;

		}

		if (char_pressed == 117) //u
		{
			wc = 0.0;


			temp[0] = 0x80;
			temp[1] = 0x80;
			SP->WriteData(temp, 2);

		}
		if (char_pressed == 107) //k
		{



			R += 1;

		}
		if (char_pressed == 108) //l
		{

			R -= 1;
		}

		if (char_pressed == 113) //q
		{
			f += 5;

		}

		if (char_pressed == 119) //w
		{
			f -= 5;

		}


		if (char_pressed == 109) // m
		{
			xd += 10;

		}


		if (char_pressed == 98) // b
		{
			xd -= 10;

		}

		if (char_pressed == 104) // h
		{
			yd += 10;

		}

		if (char_pressed == 110) // n
		{
			yd -= 10;

		}

		debugF = clock();
		totalF = clock();
		//system("cls");

		//if (paused)
		//	cout << "Paused" << endl;
		//else
		system("cls");
		//cout << "Running" << endl;
		//cout << "Tempo de analise de imagem: " << (visionF - visionI) << endl;
		//cout << "Tempo da Estrategia: " << estraF - estraI << endl;
		//cout << "Tempo do Controle: " << controlF - controlI << endl;
		//cout << "posição: " << camera.players[0].x << " , " << camera.players[0].y << endl;
		//cout << "Tempo da Comunicacao: " << commF - commI << endl;
		//cout << "Tempo do Debug:" << debugF - debugI<< endl;
		//cout << "Tempo Total: " << (totalF - totalI) << endl;
		//cout << "Frame Rate: " << 1000 * counter/ (totalF - start) << endl;
		//cout << "Frame Rate Instantanea: " << 1000 / (totalF - visionI) << endl;
		//cout << "F1: " << f1 << endl;
		//cout << "F2: " << f2 << endl;
		//cout << counter << endl;
		//cout << "tempread1 = " << (int)tempread[1] << endl;
		//cout << "tempread2 = " << (int)tempread[0] << endl;
		//cout << "temp = " << (int)temp[1] << endl;
		//cout << "temp = " << (int)temp[0] << endl;
		//cout << "PERÍODO = " << T << endl;
		//cout << "Soma: " << (int)sum << endl;



		//Sleep(100);
	}
}
