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

robot clicked = robot(-1, -1);
int carrinho = 0;
int simulation = 0;
bool ball_stopped = true;

//callback function
void mouseEvent(int evt, int x, int y, int flags, void* param){
	if(evt == CV_EVENT_LBUTTONDOWN)
	{
		clicked.x = x;
		clicked.y = y;
    }
	if (evt == CV_EVENT_LBUTTONUP)
	{
		clicked.theta = atan2(y - clicked.y, x - clicked.x);
	}
	if (evt == CV_EVENT_RBUTTONDOWN)
	{
		clicked.x = -1;
		clicked.y = -1;
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
	bool paused = true;
	int side = 0;


	// Config Vision
	// ========== Casa de Goias ===========//
	//CvRect ROI = cvRect(14, 0, 308, 235);
	//robot canto_area1 = robot(45, 54);
	//robot canto_area2 = robot(257, 178);
	//robot canto_gol1 = robot(19, 81);
	//robot canto_gol2 = robot(286, 152);
	//int amin = 5;
	//int amax = 238;
	//int radius = 20;
	//int min_distance = 13;
	//int parametro[36] = { 88, 139, 80, 121, 253, 255,
	//	22, 59, 107, 55, 204, 255,
	//	126, 43, 81, 141, 171, 247,
	//	52, 82, 94, 97, 168, 210,
	//	0, 80, 60, 14, 118, 166,
	//	0, 121, 142, 18, 255, 255 };

	// UNIFOR //
	CvRect ROI = cvRect(37, 26, 270, 196);
	robot canto_area1 = robot(46, 34);
	robot canto_area2 = robot(229, 161);
	robot canto_gol1 = robot(28, 31);
	robot canto_gol2 = robot(237, 162);
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
			debug_colors[3 + k][0] = 0;				// Azul
			debug_colors[3 + k][1] = 0;				// Azul
			debug_colors[3 + k][2] = 255;			// Azul
		}
	}


	// Config Controle

	// Parametros do PID na seguinte ordem: PF, IF, DF, PT, IT, DT, PFR, IFR, DFR, PTR, ITR, DTR

	// UNIFOR

	//float param0[12] = { 15.17, 0.92, 24.69, 15.38, 1.08, 25.03, 1.45, 0.25, 6.83, 2.38, 0.37, 6.92 };
	//float param1[12] = { 17.59, 1.13, 12.9, 19.31, 0.9, 18.41, 2.46, 0, 4.53, 3.46, 0, 5.51 };
	//float param2[12] = { 21.86, 1.06, 18, 22, 1.03, 25.03, 3.59, 0.32, 4.94, 2.9, 0.89, 3.79 };

	



	//Casa de goias
	//float param0[12] = { 19.24, 1.06, 26, 16.62, 0.94, 25.03, 9.17, 0.25, 6.83, 14.44, 0.33, 7.8 };
	//float param1[12] = { 25.72, 1.01, 20.07, 22.34, 0.94, 21.1, 10, 0.64, 1.59, 10, 0.61, 2.9 };
	//float param2[12] = { 21.86, 1.06, 18, 22, 1.03, 25.03, 3.59, 0.32, 4.94, 3.18, 0.89, 5.09 };


	float param0[12] = { 16.21, 1.06, 26.9, 18.34, 0.94, 25.86, 4.97, 0.14, 8.69, 3.93, 0.33, 8.79 };
	float param1[12] = { 25.72, 1.01, 20.07, 22.34, 0.94, 21.1, 10, 0.64, 4.92, 10, 0.61, 6.92 };
	float param2[12] = { 21.86, 1.06, 18, 22, 1.03, 25.03, 3.59, 0.32, 4.94, 2.9, 0.89, 3.79 };



	// Config Estrategia
	int weights[10] = { 6, 121, 28, 110, 3, 10, 26, 11, 10, 10 };


	CvFont fonte;
	cvInitFont(&fonte, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8);


	cvNamedWindow("Live", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Weights");
	cvResizeWindow("Weights", 600, 600);

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
	Serial* SP = new Serial("COM5");
	if (SP->IsConnected())
		printf("We're connected\n");


	start = clock();
	while (1)
	{

		counter++;
		totalI = clock();


		cvCreateTrackbar("Raio Adversario", "Weights", &weights[0], 100, NULL);
		cvCreateTrackbar("Peso Adversario", "Weights", &weights[1], 255, NULL);
		cvCreateTrackbar("Raio Bola", "Weights", &weights[2], 100, NULL);
		cvCreateTrackbar("Peso Bola", "Weights", &weights[3], 255, NULL);
		cvCreateTrackbar("Height Bola", "Weights", &weights[4], 100, NULL);
		cvCreateTrackbar("Width Bola", "Weights", &weights[5], 100, NULL);
		cvCreateTrackbar("Raio Jogador", "Weights", &weights[6], 100, NULL);
		cvCreateTrackbar("Weight Data", "Weights", &weights[7], 100, NULL);
		cvCreateTrackbar("Weight Smooth", "Weights", &weights[8], 100, NULL);
		cvCreateTrackbar("Raio da Posse", "Weights", &weights[9], 100, NULL);

		Estrategia.ConfigWeight(weights);


		// =========================== VISÃO =========================== //
		visionI = clock();


		Camera.getImage();
		if (simulation == 0)
		{
			Camera.dataAcquire();
		}
		if (simulation == 1)
		{
			Camera.players[6].stop = ball_stopped;
			Camera.players[carrinho] = clicked;
		}


		//Camera.players[1] = robot(-1, -1);
		//Camera.players[2] = robot(-1, -1);
		visionF = clock();




		// =========================== ESTRATEGIA =========================== //

		estraI = clock();
		system("cls");
		//Atribuição de comportamentos
		Estrategia.update(Camera.players);

		Estrategia.comportamento3();
		Estrategia.Posse();
		//Estrategia.players[2].comportamento = 3;
		//Estrategia.players[1].comportamento = 2;
		// Atribuição de objetivos
		Estrategia.goleiro4();
		Estrategia.lateral();
		Estrategia.atacante2();
		Estrategia.zagueiro();


		Estrategia.generalRouting2();
		//Estrategia.players[0].setGoal(23, 98);
		//Estrategia.players[1].setGoal(23, 98);


		estraF = clock();



		//  =========================== CONTROLE =========================== //
		controlI = clock();
		//Controle00.controla(Camera.players[0], robot(Estrategia.players[0].goal_x, Estrategia.players[0].goal_y), robot(Estrategia.players[0].objective_x, Estrategia.players[0].objective_y));
		//Controle01.controla(Camera.players[1], robot(Estrategia.players[1].goal_x, Estrategia.players[1].goal_y), robot(Estrategia.players[1].objective_x, Estrategia.players[1].objective_y));
		//Controle02.controla(Camera.players[2], robot(Estrategia.players[2].goal_x, Estrategia.players[2].goal_y), robot(Estrategia.players[2].objective_x, Estrategia.players[2].objective_y));

		//Controle00.controla(Camera.players[0], robot(Estrategia.players[0].goal_x, Estrategia.players[0].goal_y), robot(Estrategia.players[0].objective_x, Estrategia.players[0].objective_y));
		//Controle01.controla(Camera.players[1], robot(Estrategia.players[1].goal_x, Estrategia.players[1].goal_y));
		//Controle02.controla(Camera.players[2], robot(Estrategia.players[2].goal_x, Estrategia.players[2].goal_y));
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
		//if (simulation != -1)
		//{
		//	cv::resize(Estrategia.fieldI[1], pesos, cv::Size(Camera.frame->width, Camera.frame->height), 0, 0);
		//	cvReleaseImage(&pesos3);

		//	pesos3 = cvCreateImage(cvGetSize(Camera.frame), Camera.frame->depth, Camera.frame->nChannels);
		//	cvMerge(&(IplImage)pesos, &(IplImage)pesos, &(IplImage)pesos, NULL, pesos3);
		//	cvAddWeighted(pesos3, 0.9, Camera.frame, 0.8, 80, Camera.frame);
		//}

		for (int i = 0; i < 7; i++)
		{
			if (Camera.players[i].x != -1)
			{
				cvCircle(Camera.frame, cvPoint((int)Camera.players[i].x, (int)Camera.players[i].y), radius, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
				cvLine(Camera.frame, cvPoint((int)Camera.players[i].x, (int)Camera.players[i].y), cvPoint((int)Camera.players[i].x + radius*cos(Camera.players[i].theta),
					(int)Camera.players[i].y + radius*sin(Camera.players[i].theta)), CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
			}
		}

		//for (int i = 0; i < 3; i++)
		//{
		//	if (Estrategia.players[i].x != -1)
		//		cvLine(Camera.frame, cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), cvPoint((int)Estrategia.players[i].goal_x, (int)Estrategia.players[i].goal_y), CV_RGB(255, 255, 255), 3);
		//}

		//for (int i = 1; i < 6; i++)
		//{
		//	if (Estrategia.players[i].sec1_x != -1)
		//	{
		//		cvCircle(Camera.frame, cvPoint((int)Estrategia.players[i].sec1_x, (int)Estrategia.players[i].sec1_y), 3, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 3);
		//		cvCircle(Camera.frame, cvPoint((int)Estrategia.players[i].sec2_x, (int)Estrategia.players[i].sec2_y), 3, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 3);
		//	}
		//}
		//for (vector<aresta>::iterator it = Estrategia.vertices.begin(); it != Estrategia.vertices.end(); ++it)
		//{
		//	cvCircle(Camera.frame, cvPoint((int) it-> x, (int)it -> y), 2, CV_RGB(0, 0, 0), 2);
		//}

		//for (vector<robot>::iterator it = Estrategia.draw.begin(); it != Estrategia.draw.end(); ++it)
		//{
		//	cvCircle(Camera.frame, cvPoint((int)it->x, (int)it->y), 2, CV_RGB(255, 0, 0), 2);
		//}

		//cout << "numero de caminhos: " << Estrategia.paths[2].size() << endl;
		//cout << "numero de vertices: " << Estrategia.vertices.size() << endl;
		//for (vector<aresta>::iterator it = Estrategia.paths.begin(); it != Estrategia.paths.end(); ++it)
		//{
		//	cvLine(Camera.frame, cvPoint((int)it->x, (int)it->y), cvPoint((int)it->nx, (int)it->ny), CV_RGB(0, 0, 0), 2);
		//	cout << "( " << it->x << ", " << it->y << ") ---> ( " << it->nx << ", " << it->ny << " )" << endl;
		//}

		for (int i = 0; i < 3; i++)
		{
			if (Estrategia.pilha[i].size() > 0)
			{
				for (vector<robot>::iterator it = Estrategia.pilha[i].begin(); it != Estrategia.pilha[i].end() - 1; ++it)
				{
					cvLine(Camera.frame, cvPoint((int)it->x, (int)it->y), cvPoint((int)(it + 1)->x, (int)(it + 1)->y), CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
					//cout << "====================Rota======================" << endl;
					//cout << "( " << it->x << ", " << it->y << ") ---> ( " << (it + 1)->x << ", " << (it + 1)->y << " )" << endl;
				}
			}
		}

		


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


		cvLine(Camera.frame, cvPoint((int)Estrategia.ponto_gol.x, (int)0), cvPoint((int)Estrategia.ponto_gol.x, (int)Camera.h), CV_RGB(255, 255, 255), 2);
		cvLine(Camera.frame, cvPoint((int)Estrategia.ponto_gol.x, (int)0), cvPoint((int)Estrategia.ponto_gol.x, (int)Camera.h), CV_RGB(255, 255, 255), 2);

		for (int i = 0; i < 3; i++)
		{
			if (Estrategia.players[i].x != -1)
			{
				int comportamento = Estrategia.players[i].comportamento;
				// cout << "Jogador: " << i << ", Comportamento: " << comportamento << endl;
				if (comportamento == 1)
					cvPutText(Camera.frame, "G", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				if (comportamento == 2)
					cvPutText(Camera.frame, "A", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				if (comportamento == 3)
					cvPutText(Camera.frame, "L", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				if (comportamento == 4)
					cvPutText(Camera.frame, "Z", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
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
		if (char_pressed == 115)
		{
			simulation = 1;
			for (int i = 0; i < 7; i++)
				Camera.players[i] = robot(-1, -1);
		}
		if (char_pressed == 114)
			simulation = 0;
		if (char_pressed == 49)
			carrinho = 0;
		if (char_pressed == 50)
			carrinho = 1;
		if (char_pressed == 51)
			carrinho = 2;
		if (char_pressed == 52)
			carrinho = 3;
		if (char_pressed == 53)
			carrinho = 4;
		if (char_pressed == 54)
			carrinho = 5;
		if (char_pressed == 55)
			carrinho = 6;
		if (char_pressed == 97)
		{
			if (ball_stopped)
				ball_stopped = false;
			else
				ball_stopped = true;
		}

		debugF  = clock();
		totalF = clock();
		

		////if (paused)
		////	cout << "Paused" << endl;
		////else
		////	cout << "Running" << endl;

		//cout << "Carrinho 0: " << Camera.players[0].x << ", " << Camera.players[0].y << endl;
		//cout << "Carrinho 1: " << Camera.players[1].x << ", " << Camera.players[1].y << endl;
		//cout << "Carrinho 2: " << Camera.players[2].x << ", " << Camera.players[2].y << endl;
		cout << "Tempo de analise de imagem: " << (visionF - visionI) << endl;
		cout << "Tempo da Estrategia: " << estraF - estraI << endl;
		cout << "Tempo do Controle: " << controlF - controlI << endl;
		cout << "Tempo da Comunicacao: " << commF - commI << endl;
		cout << "Tempo do Debug:" << debugF - debugI<< endl;
		cout << "Tempo Total: " << (totalF - totalI) << endl;
		cout << "Frame Rate: " << 1000 * counter / (totalF - start) << endl;
		//cout << "Sec1: " << Estrategia.players[1].sec1_x << ", " << Estrategia.players[1].sec1_y << endl;

		//cout << "stop: " << Estrategia.players[6].stop << endl;
		//cout << "theta: " << Estrategia.players[6].theta << endl;

		//cout << "side: " << side << endl;

		//cout << counter << endl;
		//system("cls");
		//cout << "Carrinho: " << carrinho << endl;
		//cout << "Posicao: " << clicked.x << ", " << clicked.y << endl;
	}
}
