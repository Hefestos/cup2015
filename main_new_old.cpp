#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <string>
#include"estrategia\estrategia.h"
#include"controle\controle.h"
#include"serial\serial.h"
#include"vision\vision.h"
#include"robot\robot.h"
#include <time.h>


//NameSpaces
using namespace cvb;
using namespace std;


//callback function
void mouseEvent(int evt, int x, int y, int flags, void* param){
	robot clicked;
	if(evt==CV_EVENT_LBUTTONDOWN){
        printf("%d %d\n",x,y);
		clicked.x = x;
		clicked.y = y;
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

	FILE * gFile;
	xFile = fopen("geral.csv", "w");

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


	// Config Vision

	CvRect ROI = cvRect(25, 49, 295, 184);
	robot canto_area1 = robot(49, 35);
	robot canto_area2 = robot(244, 146);
	robot canto_gol1 = robot(23, 59);
	robot canto_gol2 = robot(276, 126);
	int amin = 23;
	int amax = 173;
	int radius = 20;
	int min_distance = 12;
	int parametro[36] = { 88, 139, 80, 121, 253, 255,
		21, 62, 114, 60, 243, 255,
		146, 98, 164, 180, 162, 255,
		55, 189, 91, 94, 229, 255,
		3, 102, 83, 17, 199, 229,
		0, 85, 135, 21, 255, 255 };











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
	{ 0, 64, 0 },				// Verde
	{ 117, 58, 0 },				// Marrom
	{ 255, 255, 0 },				// Amarelo
	{ 255, 255, 0 },				// Amarelo
	{ 255, 255, 0 },				// Amarelo
	{ 255, 128, 0 } };			// Laranja

	if (time == 1)
	{
		for (int k = 0; k < 3; k++)
		{
			debug_colors[3 + k][0] = 0;			// Azul
			debug_colors[3 + k][1] = 0;			// Azul
			debug_colors[3 + k][2] = 255;			// Azul
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
	Camera.createCamera(1);
	Camera.getImage(ROI);
	Camera.configParam(parametro, amin, amax, time, min_distance, ROI);

	// Inicialização da Estratégia
	estrategia Estrategia(Camera.h, Camera.w, side, canto_gol1, canto_gol2, canto_area1, canto_area2);
	Estrategia.Config(Camera.h, Camera.w);
	pesos = cvCreateImage(cvGetSize(Camera.frame), Camera.frame->depth, 1);
	Estrategia.ConfigAreas(canto_gol1, canto_gol2, canto_area1, canto_area2);
	
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


	start = clock();
	while (1)
	{

		counter++;
		totalI = clock();




		// =========================== VISÃO =========================== //
		visionI = clock();

		Camera.getImage();
		Camera.dataAcquire();
		Camera.players[1] = robot(-1, -1);
		Camera.players[2] = robot(-1, -1);

		visionF = clock();

		//if (Camera.players[0].x != -1)
		//	fprintf(xFile, "%f, %i\n", Camera.players[0].x * (184 / ROI.width), int(visionF - start));
		//if (Camera.players[0].y != -1)
		//	fprintf(yFile, "%f, %i\n", Camera.players[0].y * (95 / ROI.height), int(visionF - start));
		//if (Camera.players[0].x != -1)
		//	fprintf(thetaFile, "%f, %i\n", Camera.players[0].theta, int(visionF - start));
		//if (Camera.players[0].x != -1)
		//	fprintf(gFile, "%f,%f,%f %i\n", Camera.players[0].x * (184 / ROI.width), Camera.players[0].y * (95 / ROI.height), Camera.players[0].theta, int(visionF - start));

		// =========================== ESTRATEGIA =========================== //

		estraI = clock();

		//Atribuição de comportamentos
		Estrategia.update(Camera.players);

		Estrategia.comportamento3();
		Estrategia.Posse();
		//Estrategia.players[0].comportamento = 3;
		//Estrategia.players[1].comportamento = 3;
		//Estrategia.players[2].comportamento = 2;

		// Atribuição de objetivos
		Estrategia.goleiro4();
		Estrategia.lateral();
		Estrategia.atacante2();
		Estrategia.zagueiro();


		Estrategia.generalRouting();
		
		
		//Estrategia.players[0].setGoal(23, 98);
		//Estrategia.players[1].setGoal(23, 98);


		estraF = clock();



		//  =========================== CONTROLE =========================== //
		controlI = clock();
		Controle00.controla(Camera.players[0], robot(Estrategia.players[0].goal_x, Estrategia.players[0].goal_y), robot(Estrategia.players[0].objective_x, Estrategia.players[0].objective_y));
		Controle01.controla(Camera.players[1], robot(Estrategia.players[1].goal_x, Estrategia.players[1].goal_y), robot(Estrategia.players[1].objective_x, Estrategia.players[1].objective_y));
		Controle02.controla(Camera.players[2], robot(Estrategia.players[2].goal_x, Estrategia.players[2].goal_y), robot(Estrategia.players[2].objective_x, Estrategia.players[2].objective_y));
		controlF = clock();

		// =========================== COMUNICAÇÃO =========================== //
		commI = clock();

		if (!paused)
		{
			//SP->send(0, 0, 15);
			SP->send(0, Controle00.v.x, Controle00.v.y);
			whishPath.push_back(robot(Estrategia.players[0].goal_x, Estrategia.players[0].goal_y));
			followedPath.push_back(Camera.players[0]);
			//SP->send(1, Controle01.v.x, Controle01.v.y);
			//SP->send(2, Controle02.v.x, Controle02.v.y);
		}
		else
		{
			SP->send(0, 0, 0);
			SP->send(1, 0, 0);
			SP->send(2, 0, 0);
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
		cv::resize(Estrategia.fieldI[1], pesos, cv::Size(Camera.frame->width, Camera.frame->height), 0, 0);
		cvReleaseImage(&pesos3);

		pesos3 = cvCreateImage(cvGetSize(Camera.frame), Camera.frame->depth, Camera.frame->nChannels);
		cvMerge(&(IplImage)pesos, &(IplImage)pesos, &(IplImage)pesos, NULL, pesos3);
		cvAddWeighted(pesos3, 0.9, Camera.frame, 0.8, 80, Camera.frame);

		for (int i = 0; i < 7; i++)
		{
			if (Camera.players[i].x != -1)
			{
				cvCircle(Camera.frame, cvPoint((int)Camera.players[i].x, (int)Camera.players[i].y), radius, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
				cvLine(Camera.frame, cvPoint((int)Camera.players[i].x, (int)Camera.players[i].y), cvPoint((int)Camera.players[i].x + radius*cos(Camera.players[i].theta),
					(int)Camera.players[i].y + radius*sin(Camera.players[i].theta)), CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (Estrategia.players[i].x != -1)
				cvLine(Camera.frame, cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), cvPoint((int)Estrategia.players[i].goal_x, (int)Estrategia.players[i].goal_y), CV_RGB(255, 255, 255), 3);
		}

		for (int n = 0; n < 3; n++)
		{
			if (Estrategia.path[n].size() > 1)
			{
				for (int i = 0; i < (Estrategia.path[n].size() - 1); i++)
				{
					//cout << "( " << Estrategia.path[n][i].x << ", " << Estrategia.path[n][i].y << " ) ; " << endl;
					cvLine(Camera.frame, cvPoint((int)Estrategia.path[n][i].x, (int)Estrategia.path[n][i].y), cvPoint((int)Estrategia.path[n][i + 1].x, (int)Estrategia.path[n][i + 1].y), CV_RGB(debug_colors[n][0], debug_colors[n][1], debug_colors[n][2]), 2);
				}
			}
		}
		for (int i = 0; i < 3; i++)
		{
			if (Estrategia.players[i].x != -1)
			{
				int comportamento = Estrategia.players[i].comportamento;
				switch (comportamento)
				{
				case(1) :
					cvPutText(Camera.frame, "G", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				//case(2) :
				//	cvPutText(Camera.frame, "A", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				//case(3) :
				//	cvPutText(Camera.frame, "L", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				//case(4) :
				//	cvPutText(Camera.frame, "Z", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				}
			}
		}

		if (whishPath.size() > 1)
		{
			for (int i = 0; i < (whishPath.size() - 1); i++)
			{
				//cout << "( " << Estrategia.path[n][i].x << ", " << Estrategia.path[n][i].y << " ) ; " << endl;
				cvLine(Camera.frame, cvPoint((int)whishPath[i].x, (int)whishPath[i].y), cvPoint((int)whishPath[i + 1].x, (int)whishPath[i + 1].y), CV_RGB(221, 0, 0, 2));			//Vermelho
				cvLine(Camera.frame, cvPoint((int)followedPath[i].x, (int)followedPath[i].y), cvPoint((int)followedPath[i + 1].x, (int)followedPath[i + 1].y), CV_RGB(94, 83, 138, 2));		//Roxo
			}
		}

		cvShowImage("Live", Camera.frame);



		//Escape Sequence
		char_pressed = cvWaitKey(1);
		if (char_pressed == 27)
		{
			SP->send(0, 0, 0);
			SP->send(1, 0, 0);
			SP->send(2, 0, 0);
			break;
		}
		if (char_pressed == 112)
		{
			if (paused)
				paused = false;
			else
				paused = true;
		}
		if (char_pressed == 99)
		{
			whishPath.clear();
			followedPath.clear();
		}
		


		debugF  = clock();
		totalF = clock();
		system("cls");

		//if (paused)
		//	cout << "Paused" << endl;
		//else
			cout << "Running" << endl;
		cout << "Tempo de analise de imagem: " << (visionF - visionI) << endl;
		cout << "Tempo da Estrategia: " << estraF - estraI << endl;
		cout << "Tempo do Controle: " << controlF - controlI << endl;
		cout << "Tempo da Comunicacao: " << commF - commI << endl;
		cout << "Tempo do Debug:" << debugF - debugI<< endl;
		cout << "Tempo Total: " << (totalF - totalI) << endl;
		cout << "Frame Rate: " << 1000 * counter/ (totalF - start) << endl;
		//cout << "posicao: "<< camera.players[0].x << "," << camera.players[0].y <<endl;
		//cout << counter << endl;
		//system("cls");
	}
}
