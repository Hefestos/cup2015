#define _CRT_SECURE_NO_DEPRECATE

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
	cv::Mat pesos;
	IplImage* pesos3;
	pesos3 = cvCreateImage(cvSize(1, 1), 8, 3);
	// Config Debug
	clock_t visionI, visionF, estraI, estraF, controlI, controlF, commI, commF, totalI, totalF, debugI, debugF, start;
	int p = 0;
	float counter = 0;
	char char_pressed;
	int time = 1;
	int side = 1;
	bool paused = true;


	// Config Vision

	CvRect ROI = cvRect(20, 69, 271, 164);
	robot canto_area1 = robot(54, 36);
	robot canto_area2 = robot(224, 123);
	robot canto_gol1 = robot(19, 48);
	robot canto_gol2 = robot(257, 113);
	int amin = 23;
	int amax = 173;
	int radius = 20;
	int min_distance = 12;
	int parametro[36] = { 88, 139, 80, 121, 253, 255,
		21, 62, 114, 60, 243, 255,
		138, 59, 73, 163, 131, 255,
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
	
	//float param0[12] = { 15.17, 0.29, 14.33, 0, 0, 0, 2.51, 0.09, 0.36, 2.37, 0.08, 0.31 };
	//float param0[12] = { 5.03, 0.85, 14.48, 4.97, 0.37, 16, 5.09, 0, 0.07, 5.37, 0, 0.07 };
	float param0[12] = { 20.21, 0.97, 7.24, 19.72, 0.32, 7.38, 4.48, 0.14, 1.47, 9.95, 0.19, 1.45 };
	float param1[12] = { 10, 0, 8.93, 10, 0, 10, 5.09, 0, 0.07, 5.37, 0, 0.07 };
	float param2[12] = { 10, 0.07, 8.93, 10, 0.07, 10, 5.09, 0, 0.07, 5.37, 0, 0.07 };


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
	
	// Inicialização do Controle
	//controle Controle00((float)0.2, (float)0.007, (float)6, (float)0.3, (float)0.000001, (float)4);
	controle Controle00(param0);
	controle Controle01(param1);
	controle Controle02(param2);
	Controle00.distance_lim = 50;
	Controle01.distance_lim = 50;
	Controle02.distance_lim = 50;

	
	// Inicialização do Serial
	Serial* SP = new Serial("COM5"); 
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


		// =========================== ESTRATEGIA =========================== //

		estraI = clock();

		//Atribuição de comportamentos
		Estrategia.update(Camera.players);

		Estrategia.comportamento3();
		Estrategia.Posse();
		Estrategia.players[1].comportamento = 3;
		Estrategia.players[2].comportamento = 2;

		// Atribuição de objetivos
		Estrategia.goleiro_gago();
		Estrategia.lateral();
		Estrategia.atacante();


		Estrategia.generalRouting();
		//Estrategia.players[0].setGoal(50, 98);
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
			SP->send(0, Controle00.v.x, Controle00.v.y);
			SP->send(1, Controle01.v.x, Controle01.v.y);
			SP->send(2, Controle02.v.x, Controle02.v.y);
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
		


		debugF  = clock();
		totalF = clock();
		//system("cls");

		//if (paused)
		//	cout << "Paused" << endl;
		//else
		//	cout << "Running" << endl;
		//cout << "Tempo de analise de imagem: " << (visionF - visionI) << endl;
		//cout << "Tempo da Estrategia: " << estraF - estraI << endl;
		//cout << "Tempo do Controle: " << controlF - controlI << endl;
		//cout << "Tempo da Comunicacao: " << commF - commI << endl;
		//cout << "Tempo do Debug:" << debugF - debugI<< endl;
		//cout << "Tempo Total: " << (totalF - totalI) << endl;
		//cout << "Frame Rate: " << 1000 * counter/ (totalF - start) << endl;

		//cout << counter << endl;
		//system("cls");
	}
}
