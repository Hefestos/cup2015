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


vector<robot> newpath;

//callback function
void mouseEvent(int evt, int x, int y, int flags, void* param){
	robot clicked;
	if(evt==CV_EVENT_LBUTTONDOWN){
		clicked.x = x;
		clicked.y = y;
		newpath.insert(newpath.begin(), clicked);
    }
	if (evt == CV_EVENT_RBUTTONDOWN){
		newpath.clear();
	}
}



int main()
{
	// Config Debug
	clock_t visionI, visionF, estraI, estraF, controlI, controlF, commI, commF, totalI, totalF, debugI, debugF, start;
	int p = 0;
	float counter = 0;
	char char_pressed;
	int carrinho = 0;
	int time = 1;


	// Config Vision

	//CvRect ROI = cvRect(36, 46, 266, 162);

	//int amin = 20;
	//int amax = 250;
	//int radius = 20;
	//int min_distance = 50;
	//int time = 0;

	//int parametro[36] = { 88, 139, 80, 121, 253, 255,		// Azul
	//	22, 59, 141, 55, 204, 255,			// Amarelo
	//	117, 58, 113, 141, 113, 255,		// Roxo
	//	52, 82, 89, 97, 168, 169,			// Verde
	//	0, 79, 123, 19, 149, 191,			// Marrom
	//	0, 129, 182, 18, 251, 255 };			// Laranja


	//CvRect ROI = cvRect(12, 1, 305, 234);
	//robot canto_area1 = robot(46, 56);
	//robot canto_area2 = robot(259, 179);
	//robot canto_gol1 = robot(19, 81);
	//robot canto_gol2 = robot(286, 153);
	//int amin = 11;
	//int amax = 238;
	//int radius = 20;
	//int min_distance = 13;
	//int parametro[36] = { 88, 139, 80, 121, 253, 255,
	//	22, 59, 141, 55, 204, 255,
	//	120, 63, 155, 152, 168, 255,
	//	52, 82, 94, 97, 168, 210,
	//	0, 85, 100, 19, 185, 227,
	//	8, 129, 182, 18, 255, 255 };


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
	//float param0[12] = { 24.14, 0.94, 18.14, 30, 0.99, 11.72, 5.09, 0, 0.07, 6.92, 0, 0.07 };
	//float param1[12] = { 14.28, 0.09, 9.52, 14.69, 0.09, 9.79, 5.09, 0, 0.07, 5.37, 0, 0.07 };
	//float param2[12] = { 18.83, 1.22, 14.41, 19.17, 0.3, 14.34, 3.03, 0.09, 2.71, 3.50, 0.08, 2.31 };

	//int param0[12] = { 1924, 92, 1676, 1903, 99, 1717, 503, 14, 618, 364, 19, 743 };
	//ROXO-- - { 1869, 103, 724, 1972, 103, 738, 1000, 14, 147, 995, 19, 145 };

	//int param0[12] = { 1517, 92, 2469, 1538, 108, 2503, 145, 25, 683, 238, 37, 692 };

	////int param0[12] = { 1517, 92, 2469, 1538, 108, 2503, 269, 14, 611, 318, 19, 743 };
	//int param1[12] = { 1428, 9, 952, 1469, 9, 979, 509, 0, 7, 537, 0, 7 };
	//int param2[12] = { 1517, 92, 2469, 1538, 108, 2503, 269, 14, 611, 318, 19, 743 };


	// ========Casa de Goias ========//
	//int param0[12] = { 1924, 106, 2600, 1662, 94, 2503, 917, 25, 683, 1444, 33, 780 };
	//int param1[12] = { 2572, 101, 2007, 2234, 94, 2110, 1000, 64, 159, 1000, 61, 290 };
	//int param2[12] = { 2186, 106, 1800, 2200, 103, 2503, 359, 32, 494, 318, 89, 509 };

	// ===== UNIFOR ========//
	//int param0[12] = { 1517, 92, 2469, 1538, 108, 2503, 145, 25, 683, 238, 37, 692 };
	//int param1[12] = { 1331, 9, 1903, 1469, 9, 0, 982, 0, 0, 1000, 0, 0 };
	//int param2[12] = { 2186, 106, 1800, 2200, 103, 2503, 359, 32, 494, 290, 89, 379 };
	//int param0[12] = { 1621, 106, 2690, 1662, 94, 2503, 497, 14, 869, 393, 33, 879 };
	//int param1[12] = { 2572, 101, 2007, 2234, 94, 2110, 1000, 64, 492, 1000, 61, 692 };
	//int param2[12] = { 2186, 106, 1800, 1552, 103, 2800, 359, 32, 494, 318, 89, 509 };

	// Testes no LabRob
	int param0[12] = { 1145, 149, 2690, 1359, 163, 2586, 103, 14, 936, 140, 23, 944 };
	int param1[12] = { 2572, 101, 2007, 2234, 94, 2110, 1000, 64, 492, 1000, 61, 692 };
	int param2[12] = { 2186, 106, 1800, 2200, 103, 2503, 359, 32, 494, 290, 89, 379 };

	//float param0[12] = { 11.45, 1.49, 26.9, 13.59, 1.63, 25.86, 1.03, 0.14, 9.36, 1.4, 0.23, 9.44 };
	//float param1[12] = { 25.72, 1.01, 20.07, 22.34, 0.94, 21.1, 10, 0.64, 4.92, 10, 0.61, 6.92 };
	//float param2[12] = { 21.86, 1.06, 18, 22, 1.03, 25.03, 3.59, 0.32, 4.94, 2.9, 0.89, 3.79 };


	CvFont fonte;
	cvInitFont(&fonte, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 2, 8);

	cvNamedWindow("Live", CV_WINDOW_AUTOSIZE);

	cvNamedWindow("PID");
	cvResizeWindow("PID", 600, 800);

	// Inicialização da Visão
	vision Camera;
	Camera.createCamera(0);
	Camera.getImage(ROI);
	Camera.configParam(parametro, amin, amax, time, min_distance, ROI);

	// Inicialização da Estratégia
	estrategia Estrategia(Camera.h, Camera.w);
	Estrategia.Config(Camera.h, Camera.w);
	
	// Inicialização do Controle
	controle Controle00(param0);
	controle Controle01(param1);
	controle Controle02(param2);
	Controle00.distance_lim = 50;
	
	// Inicialização do Serial
	Serial* SP = new Serial("COM5"); 
	if (SP->IsConnected())
		printf("We're connected\n");


	start = clock();
	while (1)
	{
		
		counter++;
		totalI = clock();

		switch (carrinho)
		{
		case 0:
				{
				cvCreateTrackbar("Carrinho", "PID", &carrinho, 2, NULL);
				cvCreateTrackbar("PP Frente", "PID", &param0[0], 3000, NULL);
				cvCreateTrackbar("PI Frente", "PID", &param0[1], 1000, NULL);
				cvCreateTrackbar("PD Frente", "PID", &param0[2], 3000, NULL);
				cvCreateTrackbar("PP Tras", "PID", &param0[3], 3000, NULL);
				cvCreateTrackbar("PI Tras", "PID", &param0[4], 1000, NULL);
				cvCreateTrackbar("PD Tras", "PID", &param0[5], 3000, NULL);
				cvCreateTrackbar("PP R Frente", "PID", &param0[6], 3000, NULL);
				cvCreateTrackbar("PI R Frente", "PID", &param0[7], 1000, NULL);
				cvCreateTrackbar("PD R Frente", "PID", &param0[8], 1000, NULL);
				cvCreateTrackbar("PP R Tras", "PID", &param0[9], 3000, NULL);
				cvCreateTrackbar("PI R Tras", "PID", &param0[10], 1000, NULL);
				cvCreateTrackbar("PD R Tras", "PID", &param0[11], 1000, NULL);
				}
				break;
		case 1:
			{
				cvCreateTrackbar("Carrinho", "PID", &carrinho, 2, NULL);

				cvCreateTrackbar("PP Frente", "PID", &param1[0], 3000, NULL);
				cvCreateTrackbar("PI Frente", "PID", &param1[1], 1000, NULL);
				cvCreateTrackbar("PD Frente", "PID", &param1[2], 3000, NULL);
				cvCreateTrackbar("PP Tras", "PID", &param1[3], 3000, NULL);
				cvCreateTrackbar("PI Tras", "PID", &param1[4], 1000, NULL);
				cvCreateTrackbar("PD Tras", "PID", &param1[5], 3000, NULL);
				cvCreateTrackbar("PP R Frente", "PID", &param1[6], 1000, NULL);
				cvCreateTrackbar("PI R Frente", "PID", &param1[7], 1000, NULL);
				cvCreateTrackbar("PD R Frente", "PID", &param1[8], 1000, NULL);
				cvCreateTrackbar("PP R Tras", "PID", &param1[9], 1000, NULL);
				cvCreateTrackbar("PI R Tras", "PID", &param1[10], 1000, NULL);
				cvCreateTrackbar("PD R Tras", "PID", &param1[11], 1000, NULL);

				}
				break;
		case 2:
			{
				cvCreateTrackbar("Carrinho", "PID", &carrinho, 2, NULL);

				cvCreateTrackbar("PP Frente", "PID", &param2[0], 3000, NULL);
				cvCreateTrackbar("PI Frente", "PID", &param2[1], 1000, NULL);
				cvCreateTrackbar("PD Frente", "PID", &param2[2], 3000, NULL);
				cvCreateTrackbar("PP Tras", "PID", &param2[3], 3000, NULL);
				cvCreateTrackbar("PI Tras", "PID", &param2[4], 1000, NULL);
				cvCreateTrackbar("PD Tras", "PID", &param2[5], 3000, NULL);
				cvCreateTrackbar("PP R Frente", "PID", &param2[6], 1000, NULL);
				cvCreateTrackbar("PI R Frente", "PID", &param2[7], 1000, NULL);
				cvCreateTrackbar("PD R Frente", "PID", &param2[8], 1000, NULL);
				cvCreateTrackbar("PP R Tras", "PID", &param2[9], 1000, NULL);
				cvCreateTrackbar("PI R Tras", "PID", &param2[10], 1000, NULL);
				cvCreateTrackbar("PD R Tras", "PID", &param2[11], 1000, NULL);

				}
				break;
		}
		
		Controle00.setParam(param0);
		Controle01.setParam(param1);
		Controle02.setParam(param2);



		// ------------------VISÃO ------------------//
		visionI = clock();
		Camera.getImage();
		Camera.dataAcquire();
		visionF = clock();
		//cout << "Tempo de analise de imagem: " << (visionF - visionI) << endl;

		//----------------ESTRATEGIA---------------//

		estraI = clock();
		
		//Atribuição de comportamentos
		Estrategia.update(Camera.players);
		//Estrategia.players[carrinho].setPos(10, 10);

		Estrategia.path[carrinho] = newpath;

		for (int i = 0; i < 3; i++)
		{
			if (i != carrinho)
				Estrategia.path[i].clear();
		}

		if (newpath.size() > 0)
			Estrategia.players[carrinho].setObjective(Estrategia.path[carrinho].front());

		Estrategia.nextGoalI(carrinho);

		for (int i = 0; i < 3; i++)
		{
			if (i != carrinho)
				Estrategia.players[i].setGoal(Estrategia.players[i]);
		}

		newpath = Estrategia.path[carrinho];


		estraF = clock();
		//cout << "Tempo da Estrategia: " << estraF - estraI << endl;
		

		// ---------------------CONTROLE ---------------//
		controlI = clock();
		Controle00.controla(Camera.players[0], robot(Estrategia.players[0].goal_x, Estrategia.players[0].goal_y), robot(Estrategia.players[0].objective_x, Estrategia.players[0].objective_y));
		Controle01.controla(Camera.players[1], robot(Estrategia.players[1].goal_x, Estrategia.players[1].goal_y), robot(Estrategia.players[1].objective_x, Estrategia.players[1].objective_y));
		Controle02.controla(Camera.players[2], robot(Estrategia.players[2].goal_x, Estrategia.players[2].goal_y), robot(Estrategia.players[2].objective_x, Estrategia.players[2].objective_y));
		controlF = clock();
		//cout << "Tempo do Controle: " << controlF - controlI<< endl;

		// -----------------COMUNICAÇÃO -----------------//
			
		//if ((abs(Controle00.v.y) > 0) && (abs(Controle00.v.y) < 3)){
		//	if (Controle00.v.y > 0){
		//		Controle00.v.y = 3;
		//	}
		//	else {
		//		Controle00.v.y = -3;
		//	}
		//}

		//if ((abs(Controle00.v.x) > 0) && (abs(Controle00.v.x) < 3)){
		//	if (Controle00.v.x > 0){
		//		Controle00.v.x = 3;
		//	}
		//	else {
		//		Controle00.v.x = -3;
		//	}
		//}




		commI = clock();
		SP->send(0, Controle00.v.x, Controle00.v.y);
		SP->send(1, Controle01.v.x, Controle01.v.y);
		SP->send(2, Controle02.v.x, Controle02.v.y);
		commF = clock();
		//cout << "Tempo da Comunicacao: " << commF - commI << endl;


		// Debug em prompt de comando
		debugI = clock();
		


		// Debug em janela

		cvSetMouseCallback("Live", mouseEvent, 0);

		// Desenhando nas janelas
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
			if ((Estrategia.players[i].x != -1) && (i == carrinho))
				cvLine(Camera.frame, cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), cvPoint((int)Estrategia.players[i].goal_x, (int)Estrategia.players[i].goal_y), CV_RGB(255, 255, 255), 3);
		}

		for (int n = 0; n < 3; n++)
		{
			if (Estrategia.path[n].size() > 0)
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
				case(2) :
					cvPutText(Camera.frame, "A", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				case(3) :
					cvPutText(Camera.frame, "L", cvPoint((int)Estrategia.players[i].x, (int)Estrategia.players[i].y), &fonte, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]));
				}
			}
		}


		cvShowImage("Live", Camera.frame);

		 //Escape Sequence
		char_pressed = cvWaitKey(1);
		if(char_pressed == 27)
		{
			SP->send(0, 0, 0);
			SP->send(1, 0, 0);
			SP->send(2, 0, 0);
			break;
		}

		if (char_pressed == 112)
		{
			system("cls");
			cout << "int param0[12] = {";
			for (int i = 0; i < 12; i++)
			{
				cout << param0[i];
				if (i != 11)
					cout << ", ";
			}
			cout << "};" << endl;

			cout << "int param1[12] = {";
			for (int i = 0; i < 12; i++)
			{
				cout << param1[i];
				if (i != 11)
					cout << ", ";
			}
			cout << "};" << endl;

			cout << "int param2[12] = {";
			for (int i = 0; i < 12; i++)
			{
				cout << param2[i];
				if (i != 11)
					cout << ", ";
			}
			cout << "};" << endl;

			cout << "float param0[12] = {";
			for (int i = 0; i < 12; i++)
			{
				cout << (float)param0[i] / 100;
				if (i != 11)
					cout << ", ";
			}
			cout << "};" << endl;

			cout << "float param1[12] = {";
			for (int i = 0; i < 12; i++)
			{
				cout << (float)param1[i] / 100;
				if (i != 11)
					cout << ", ";
			}
			cout << "};" << endl;

			cout << "float param2[12] = {";
			for (int i = 0; i < 12; i++)
			{
				cout << (float) param2[i] / 100;
				if (i != 11)
					cout << ", ";
			}
			cout << "};" << endl;

		}

		debugF  = clock();
		totalF = clock();
		
		//system("cls");
		//cout << "Tempo do Debug:" << debugF - debugI<< endl;
		//cout << "Tempo Total: " << (totalF - totalI) << endl;
		cout << "Frame Rate: " << 1000 * counter / (totalF - start) << endl;
		//cout << "Velocidade do carrinho: esq = " << Controle00.v.x << ", dir = " << Controle00.v.y << endl;
		//cout << "flagrot = " << Controle00.flagrot << endl;
		//cout << counter << endl;

		//cout << "Velocidade do carrinho: flagdir = " << Controle00.flagdir << endl;
		//cout << "Velocidade do carrinho: velocidade = " << Controle00.velocidade << endl;
	}
}
