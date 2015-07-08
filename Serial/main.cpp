#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string>
#include"estrategia\estrategia.h"
#include"controle\controle.h"
#include"serial\serial.h"
#include"vision\vision.h"
#include"robot\robot.h"
#include <time.h>
#include "Windows.h"
#include "controle_novo\controle_novo.h"

#include <typeinfo>


robot clicked;




//NameSpaces
using namespace cvb;
using namespace std;


//callback function
void mouseEvent(int evt, int x, int y, int flags, void* param){
	
	if(evt==CV_EVENT_LBUTTONDOWN){
        printf("%d %d\n",x,y);
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
	int amin = 16;
	int amax = 153;
	int radius = 20;
	int min_distance = 58;
	int parametro[36] = { 88, 143, 80, 121, 253, 25,
		0, 151, 153, 141, 255, 255,
		122, 68, 112, 152, 169, 255,
		55, 189, 91, 94, 229, 255,
		180, 255, 255, 0, 0, 0,
		2, 157, 90, 8, 251, 246 };

	

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
	//estrategia Estrategia(Camera.h, Camera.w, side, canto_gol1, canto_gol2, canto_area1, canto_area2);
	//Estrategia.Config(Camera.h, Camera.w);
	//pesos = cvCreateImage(cvGetSize(Camera.frame), Camera.frame->depth, 1);
	//Estrategia.ConfigAreas(canto_gol1, canto_gol2, canto_area1, canto_area2);
	 
	// Inicialização do Controle
	//controle Controle00((float)0.2, (float)0.007, (float)6, (float)0.3, (float)0.000001, (float)4);
	controle Controle00(param0);
	controle Controle01(param1);
	controle Controle02(param2);
	Controle00.distance_lim = 50;
	Controle01.distance_lim = 50;
	Controle02.distance_lim = 50;

	
	// Inicialização do Serial
	Serial* SP = new Serial("COM3"); 
	if (SP->IsConnected())
		printf("We're connected\n");

	




	//============================== VARIÁVEIS  DE TESTE ============================//

	int contador = 0;
	float t;
	float ref = 0.0, T = 3000.0, f1 = 0.0, f2 = 0.0, torque = 0.0, fx_B = 0.0, d = 3.75, KP_w = 0.0, KI_w = 0.0, flag = 0.0, K, KP_u, R = 55.0, a, dist = 0.0;
	
	float x = Camera.players[0].x;
	float y = Camera.players[0].y;
	Camera.players[0].theta_acumulated = 0;
	Camera.players[0].theta_old = 0;
	float theta = Camera.players[0].theta;
	//float t = visionF - start;
	//float v1 = 15;
	//float v2 = -15;
	float c, alpha = 0.0, biel = 0.0, v1_send = 0.0, v2_send = 0.0;
	float dx0, dy0, Kp,  v1 = 0.0, v2 = 0.0, theta2 = 0.0, thetad = 0.0, Kt, refaux = 0.0, i = 0.0,o = 0.0, ang_error = 0.0;
	float ct, st, erro_angulo = 0.0, x_bola = 0.0, y_bola = 0.0, thetad_old = 0.0, u_i = 0.0, KI_u= 0.0;
	float w = 0.0, u = 0.0, w_i = 0.0, w_p = 0.0, h = 0.0, wc = 0.0, uc = 7.0, f = 300.0, xd = 0.0, yd = 0.0, u_p = 0.0, x_0 = 155, y_0 = 116;
	h = 1.0 / 12.0;
	char temp[2]; 
	char tempread[2];
	//char msg[12];
	//byte sum;
	xd = 0;
	yd = 0;
	float n_xd, n_yd;
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
	int recievedValueA = 0;
	int recievedValueB = 0;
	int recievedValueC = 0;
	float vADC = 0;
	float voltagem = 0;
	//int ze = 255;
	int ze = 0;
	int newVelA = 0;
	int newVelB = 0;
	int newVelC = 0;
//	int loop;
	


	//=========================== INÍCIO DO LOOP ==============================//
	while (1)
	{
		
		//====================Leitura Mensagem================================ //
		readResult = SP->ReadData(incomingData, 11);
		//system("cls");
		/*if (incomingData[0] == 0x7e)
		{
			for (int i = 0; i<11; i++)
			{
			cout << dec << i << ": ";
			cout << dec << (int)incomingData[i] << endl;
			//cout << "haha" << endl;
			//system("pause");

			}
		}*/
		/*for (int i = 0; i<11; i++)
		{
		cout << dec << i << ": ";
		cout << hex << (int)incomingData[i] << endl;
		//cout << "haha" << endl;
		//system("pause");

		}*/

		
		//O endereço do xbee de onde veio a mensagem é incomingData[4] e incomingData[5]
		// -86 = AA
		// -84 = AC
		//if (incomingData[0] == 0x7e && incomingData[1] == 0x00 && incomingData[2] == 0x07 && incomingData[3] == -127 && incomingData[4] == -86 && incomingData[5] == -86 && incomingData[7] == 0)
		if (incomingData[0] == 0x7e && incomingData[1] == 0x00 && incomingData[2] == 0x07 && incomingData[3] == -127 && incomingData[7] == 0)
		{
			
			
			/*for (int i = 0; i<11; i++)
			{
				cout << dec << i << ": ";
				cout << hex << (int)incomingData[i] << endl;
				//cout << "haha" << endl;
				//system("pause");

			}*/
			
			//cout << "______-------_-______----____" << endl;
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
				
								
				recievedValue = 768 + right;
				vADC = recievedValue * 0.00489;
				voltagem = 0.013414862*recievedValue;
				
				//cout << "recieved: " << recievedValue << endl;
				cout << "int 4: " <<(int)incomingData[4] << endl;
				if (incomingData[4] == incomingData[5])
				{
					switch (incomingData[4])
					{
					case -84:
						cout << "ACAC" << endl;
						recievedValueC = recievedValue;
						break;
					case -85:
						cout << "ABAB" << endl;
						recievedValueB = recievedValue;
						break;
					case -86:
						cout << "AAAA" << endl;
						recievedValueA = recievedValue;
						break;
					default:
						break;
					}

				}
				


				for (int i = 0; i < 256; i++)
				{
					incomingData[i] = 0;
				
				}
				
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

		//newVel = (0.000001947*pow(recievedValue, 2) - 0.004673*recievedValue + 3.495)*ze;
		newVelA = (0.000001947*pow(recievedValueA, 2) - 0.004673*recievedValueA + 3.495)*ze;
		newVelB = (0.000001947*pow(recievedValueB, 2) - 0.004673*recievedValueB + 3.495)*ze;
		newVelC = (0.000001947*pow(recievedValueC, 2) - 0.004673*recievedValueC + 3.495)*ze;

		SP->sendApi(0, 1, newVelA, 1, newVelA);
		//SP->sendApi(1, 0, newVelB, 1, newVelB);
		//SP->sendApi(2, 1, newVelC, 1, newVelC);
		//SP->sendApi(1, 1, 128, 0, 0);
		system("cls");
		cout << "AAAA: " << recievedValueA << endl;
		cout << "ABAB: " << recievedValueB << endl;
		cout << "ACAC: " << recievedValueC << endl;

		
		//cout << "Total: " << dec << recievedValue << endl;
		//cout << "ADC: " << dec << vADC << endl;
		//printf("Voltagem: %0.2f V \n\n", voltagem);
		//cout << loop++ << endl;
		cout << "ze: " << ze << endl;
		cout << "newVelA: " << newVelA << endl;
		cout << "newVelB: " << newVelB << endl;
		cout << "newVelC: " << newVelC << endl;
		//haha
		
		
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
		//Estrategia.update(Camera.players);

//		Estrategia.comportamento3();
		//Estrategia.Posse();
		//Estrategia.players[0].comportamento = 3;
		//Estrategia.players[1].comportamento = 3;
		//Estrategia.players[2].comportamento = 2;

		// Atribuição de objetivos
		//Estrategia.goleiro4();
		//Estrategia.lateral();
		//Estrategia.atacante2();
		//Estrategia.zagueiro();


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


		//================= POSIÇÂO DO ROBO ==============================//

		//refaux = Camera.players[0].refaux;
		x = Camera.players[0].x - x_0;
		y = -(Camera.players[0].y - y_0);
		theta = -Camera.players[0].theta;
		x_bola = Camera.players[6].x - x_0;
		y_bola = -(Camera.players[6].y - y_0);
		// cout << "x = " << Camera.players[0].x << endl;
		// cout << "y = " << Camera.players[0].y << endl;

		//================ TEMPO ============//

		t = visionF - start;

		//============================== PLOTAGEM EM UMA TABELA DOS VALORES DE REFERENCIA ============//


		// LEMBRAR, NO USO DA ESTRATÉGIA, DE REALOCAR ESSA LINHA
		if (Camera.players[0].x != -1)
			fprintf(gFile, "%f,%f,%f, %i, %f, %f,%f,%f,%f,%f,%f, %f\n", Camera.players[0].x*1.3, Camera.players[0].y*1.3, alpha, int(visionF - start), thetad, v1, v2, w, xd*1.3, yd*1.3, u, refaux);

		//Camera.players[0].x*1.3

		//======================= REGULAGEM DO THETA =============================//



		if (Camera.players[0].theta < 0)
		{
			Camera.players[0].theta = 6.28 + Camera.players[0].theta;
		}
		else if (Camera.players[0].theta >= 0)
		{
			Camera.players[0].theta = Camera.players[0].theta;
		}


		if ((Camera.players[0].theta - Camera.players[0].theta_old) >= 3.14)
		{
			Camera.players[0].theta_acumulated = Camera.players[0].theta_acumulated - 1;
		}
		else if ((Camera.players[0].theta - Camera.players[0].theta_old) < -3.14)
		{
			Camera.players[0].theta_acumulated = Camera.players[0].theta_acumulated + 1;
		}


		alpha = -(Camera.players[0].theta + Camera.players[0].theta_acumulated*6.28);
		Camera.players[0].theta_old = Camera.players[0].theta;

		if (contar < 2){
			contar += 1;
			if (refaux <= 0)
			{
				refaux = 2 * M_PI + refaux;
			}
			else if (refaux > 0)
			{
				refaux = refaux;
			}
		}
		else
		if (refaux <= 0)
		{
			refaux = 2 * M_PI + refaux;
		}
		else if (refaux > 0)
		{
			refaux = refaux;
		}


		if ((refaux - Camera.players[0].refaux_old) >= M_PI)
		{
			Camera.players[0].refaux_acumulated = Camera.players[0].refaux_acumulated - 1;
		}
		else if ((refaux - Camera.players[0].refaux_old) < -M_PI)
		{
			Camera.players[0].refaux_acumulated = Camera.players[0].refaux_acumulated + 1;
		}





		//thetad = refaux + Camera.players[0].refaux_acumulated*6.24;
		Camera.players[0].refaux_old = refaux;

		//cout << "theta = " << Camera.players[0].theta * 180.0/3.14 << endl;
		//cout << "refaux_old = " << Camera.players[0].refaux_old << endl;
		//cout << "refaux_acumulated = " << Camera.players[0].refaux_acumulated << endl;
		//cout << "alpha = " << alpha* 180.0 / 3.14 << endl;
		//cout << "theta = " << theta* 180.0 / 3.14 << endl;
		//cout << "w = " << w << endl;
		//cout << "u = " << u << endl;




		//==================== UTILIZAÇÃO DA REFERENCIA PARA O ANGULO ===========//
		if (Camera.players[0].x == -1)
		{
			u = w = 0.0;
		}

		if (abs(x - xd) <= 0.1 && abs(y - yd) <= 0.1)
		{
			u = w = 0.0;
		}

		if (inicio_voltas == 0){

			Camera.players[0].refaux_acumulated = 0;
			inicio_voltas += 1;
			
		}

		if (!clicked.stop){
			xd = clicked.x - x_0;
			yd = -(clicked.y - y_0);

		}

		if (clicked.stop){
			xd = x;
			yd = y;

		}

		//xd = x_bola;
		//yd = y_bola;
		//xd = 0;
		//yd = 0;
		xd = cos((6.28 / T) * t)*R;
		yd = sin((6.28 / T) * t)*R;
		
		refaux = atan2(y - yd, x - xd);
		

		if (refaux > 0.0 && refaux <= M_PI / 2.0){

			thetad = refaux + Camera.players[0].refaux_acumulated*6.28 - M_PI;

		}


		if (refaux > M_PI / 2.0 && refaux <= M_PI){

			thetad = refaux + Camera.players[0].refaux_acumulated*6.28 - M_PI;

		}


		if (refaux >= -M_PI  && refaux < -M_PI / 2.0){

			thetad = refaux + Camera.players[0].refaux_acumulated*6.28 + M_PI;

		}


		if (refaux > -M_PI / 2.0 && refaux < 0){

			thetad = refaux + Camera.players[0].refaux_acumulated*6.28 + M_PI;

		}
		
		if (inicio_voltas > 0){
			if (abs(thetad) - abs(thetad_old) <= -2.947){

				thetad = thetad_old; 
				
			}
			else if (abs(thetad) - abs(thetad_old) >= 2.947){
				thetad = thetad_old; 

			}
			else if (thetad > thetad_old  && abs(thetad - thetad_old) <= 0.01){
				thetad = thetad_old;
			}
			else if (thetad < thetad_old  && abs(thetad_old - thetad) <= 0.01){
				thetad = thetad_old;
			}
		}
		 if (thetad > thetad_old && thetad - thetad_old >= M_PI){
			thetad = thetad_old;
			
		}
		 if (thetad < thetad_old && thetad_old - thetad >= M_PI){
			 thetad = thetad_old;
			
		 }
		thetad_old = thetad;
		
		
		//===================== CONTROLE DO ANGULO ===================//
		
	
		
		
		
		
		

		if (abs(thetad - alpha) <= 0.08)
		{
			ang_error = 0.0;
		}
		else
		{
			ang_error = thetad - alpha;
		}

		KP_w = -13;
		KI_w = -1.2;
		w_p = KP_w*(ang_error);
		if (abs (ang_error) <= 0.5){
			w_i = w_i;

		}
		else{
			w_i = w_i + h*KI_w*(ang_error);
		}

		
		w = w_i + w_p;


		//w = w_p;

		//w = 0.0;
		//====================CONTROLE DA POSIÇÃO===================//

		
		/*if (abs(x - xd) && abs(y - yd) < 30)
		{
			KP_u = 0.15;
		}
		else
		{
			KP_u = 0.08;
		}*/
		KI_u = 0.003;
		KP_u = 0.06;
		//KP_u = 0.09;

		dist = sqrt(pow(x - xd, 2) + pow(y - yd, 2));
		if (pow(x - xd, 2) + pow(y - yd, 2) <= 625)
		{
			dist = 0.0;
		}
		u_p = KP_u*dist;
		if (dist <= 10){
			u_i = u_i;

		}
		else{
			u_i = u_i + h*KI_u*dist;
		}

		
		u = u_p + u_i;
		//u = u_p;

		v1 = ((0.5)*u - w*0.5/d) / 10.0;
		v2 = ((0.5)*u + w*0.5 / d) / 10.0;



		//cout << "velocidade 1" << v1 << endl << "velocidade 2" << v2 << endl;

		if (v1 > 1) v1 = 1;
		if (v1 < -1) v1 = -1;
		if (v1 == 0){
			temp[1] = 0x80;
		}
		else{
			temp[1] = (v1 + 1)*127.0;
		}

		if (v2 > 1) v2 = 1;
		if (v2 < -1) v2 = -1;
		if (v2 == 0){
			temp[0] = 0x80;
		}
		else{
			temp[0] = (v2 + 1)*127.0;
		}
		
		//temp[1] = v1;
		//temp[0] = v2;
		//SP->WriteData(temp, 2);
		//SP->ReadData(tempread, 2);

		//================NOVO SERIAL XBEE API ====================
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

		

		//SP->sendApi(0, o, abs(v2 * 255), i, abs(v1 * 255));
		
		
		
		//SP->sendApi(0, 1, biel, 1, biel);
		
		//=================== DISPLAY DOS DADOS DO ROBO ============//
		
		//cout << "theta_ref = " << thetad*180.0 / 3.14 << endl;
		//cout << "refaux = " << refaux << endl;
		//cout << "ypc = " << dy0 << endl;
		//cout << "v1 = " << v1 << endl;
		//cout << "v2 = " << v2 << endl;
		//cout << "u_p = " << u_p << endl;
		//cout << "xd = " << xd << endl;
		//cout << "yd = " << yd << endl;
		//cout << "x = " << x << endl;
		//cout << "y = " << y << endl;
		//cout << "dist = " << dist << endl;
		//cout << "BIEL MUITO PUTO = " << biel << endl;
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
		//cv::resize(Estrategia.fieldI[1], pesos, cv::Size(Camera.frame->width, Camera.frame->height), 0, 0);
		//cvReleaseImage(&pesos3);

		//pesos3 = cvCreateImage(cvGetSize(Camera.frame), Camera.frame->depth, Camera.frame->nChannels);
		//cvMerge(&(IplImage)pesos, &(IplImage)pesos, &(IplImage)pesos, NULL, pesos3);
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

		
		
		//cout << "nxd: " << xd << " nyd: " << yd << endl;
		cvCircle(Camera.frame, cvPoint(xd+155, -yd+116), 5, CV_RGB(0, 255, 0), -1);
		
	//d = -(clicked.y - y_0);

		
		//for (int i = 0; i < 3; i++)
		//{
		//	if (Estrategia.players[i].x != -1)
		//		cvLine(Camera.frame, cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), cvPoint((int)Estrategia.players[i].goal_x, (int)Estrategia.players[i].goal_y), CV_RGB(255, 255, 255), 3);
		//}

		//for (int n = 0; n < 3; n++)
		//{
		//	if (Estrategia.path[n].size() > 1)
		//	{
		//		for (int i = 0; i < (Estrategia.path[n].size() - 1); i++)
		//		{
		//			//cout << "( " << Estrategia.path[n][i].x << ", " << Estrategia.path[n][i].y << " ) ; " << endl;
		//			cvLine(Camera.frame, cvPoint((int)Estrategia.path[n][i].x, (int)Estrategia.path[n][i].y), cvPoint((int)Estrategia.path[n][i + 1].x, (int)Estrategia.path[n][i + 1].y), CV_RGB(debug_colors[n][0], debug_colors[n][1], debug_colors[n][2]), 2);
		//		}
		//	}
		//}
		//for (int i = 0; i < 3; i++)
		//{
		//	if (Estrategia.players[i].x != -1)
		//	{
		//		int comportamento = Estrategia.players[i].comportamento;
		//		switch (comportamento)
		//		{
		//		case(1) :
		//			cvPutText(Camera.frame, "G", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
		//		//case(2) :
		//		//	cvPutText(Camera.frame, "A", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
		//		//case(3) :
		//		//	cvPutText(Camera.frame, "L", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
		//		//case(4) :
		//		//	cvPutText(Camera.frame, "Z", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
		//		}
		//	}
		//}

		//if (whishPath.size() > 1)
		//{
		//	for (int i = 0; i < (whishPath.size() - 1); i++)
		//	{
		//		//cout << "( " << Estrategia.path[n][i].x << ", " << Estrategia.path[n][i].y << " ) ; " << endl;
		//		cvLine(Camera.frame, cvPoint((int)whishPath[i].x, (int)whishPath[i].y), cvPoint((int)whishPath[i + 1].x, (int)whishPath[i + 1].y), CV_RGB(221, 0, 0, 2));			//Vermelho
		//		cvLine(Camera.frame, cvPoint((int)followedPath[i].x, (int)followedPath[i].y), cvPoint((int)followedPath[i + 1].x, (int)followedPath[i + 1].y), CV_RGB(94, 83, 138, 2));		//Roxo
		//	}
		//}

		cvShowImage("Live", Camera.frame);
		

		
		
		//Escape Sequence
		char_pressed = cvWaitKey(1);
		if (char_pressed == 27) //esc
		{
		
			temp[0] = 0x80;
			temp[1] = 0x80;
			//SP->WriteData(temp, 2);
			//SP->sendApi(0, 0, 0, 0, 0);
			//SP->sendApi(0, 0, 0, 0, 0);
			SP->sendApi(0, 0, 0, 0, 0);


			break;
		}
		if (char_pressed == 112) //p
		{
			ze = 0;
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

			biel += 1;
			if (ze <255)
			{
				ze += 1;
			}
			
				
		}
		if (char_pressed == 111) //o
		{

			biel -= 1;
			if (ze > 0)
			{
				ze -= 1;
			}
			
				
		}

		if (char_pressed == 117) //u
		{
			wc = 0.0;
			
			ze = 255;
			temp[0] = 0x80;
			temp[1] = 0x80;
			//SP->WriteData(temp, 2);
		
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

		debugF  = clock();
		totalF = clock();
		//system("cls");

		//if (paused)
		//	cout << "Paused" << endl;
		//else
		//system("cls");
		//cout << "Running" << endl;
		//cout << "Tempo de analise de imagem: " << (visionF - visionI) << endl;
		//cout << "Tempo da Estrategia: " << estraF - estraI << endl;
		//cout << "Tempo do Controle: " << controlF - controlI << endl;
		//cout << "posição: " << camera.players[0].x << " , " << camera.players[0].y << endl;
		//cout << "Tempo da Comunicacao: " << commF - commI << endl;
		//cout << "Tempo do Debug:" << debugF - debugI<< endl;
		//cout << "Tempo Total: " << (totalF - totalI) << endl;
		//cout << "Frame Rate: " << 1000 * counter/ (totalF - start) << endl;
		//cout << "Frame Rate Instantanea: " << 1000  / (totalF - visionI) << endl;
		//cout << "F1: " << f1 << endl;
		//cout << "F2: " << f2 << endl;
		//cout << counter << endl;
		//cout << "tempread1 = " << (int)tempread[1] << endl;
		//cout << "tempread2 = " << (int)tempread[0] << endl;
		//cout << "temp = " << (int)temp[1] << endl;
		//cout << "temp = " << (int)temp[0] << endl;
		//cout << "PERÍODO = " << T << endl;
		//cout << "Soma: " << (int) sum <<endl;
		
		//Sleep(100);
	}
}
