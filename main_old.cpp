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
/*void mouseEvent(int evt, int x, int y, int flags, void* param){
	robot clicked;
	if(evt==CV_EVENT_LBUTTONDOWN){
        printf("%d %d\n",x,y);
		robot.x = x;
		robot.y = y;
    }
}*/

int main()
{
	robot* clicked;
	clock_t end, start;
	float counter = 0;
	int amin = 36;
	int amax = 160;
	int p=0;
	int radius = 20;
	int min_distance = 50;
	char char_pressed;
	int time = 0;

	int flag_pontof = 0;
	int flag_pid = 0;

	int pp_f = 1000;
	int pi1_f = 7;
	int pd_f = 893;
	int pp_t = 1000;
	int pi1_t = 7;
	int pd_t = 1000;

	int pp_r_f = 1000;
	int pi1_r_f = 7;
	int pd_r_f = 893;
	int pp_r_t = 1000;
	int pi1_r_t = 7;
	int pd_r_t = 1000;

	int parametro[36] =	{	88, 139, 80, 121, 253, 255,		// Azul
							22, 59, 141, 55, 204, 255,			// Amarelo
							120, 82, 127, 149, 204, 255,		// Roxo
							63, 103, 105, 97, 126, 114,			// Verde
							0, 85, 107, 12, 143, 170,			// Marrom
							0, 85, 91, 12, 255, 255};			// Laranja


	int debug_colors[7][3] = {	{128,	0,		128},		// Roxo
							{0,		64,		0},				// Verde
							{117,	58,		0},				// Marrom
							{255,	255,	0},				// Amarelo
							{255,	255,	0},				// Amarelo
							{255,	255,	0},				// Amarelo
							{255,	128 ,	0}};			// Laranja

	if (time == 1)
	{
		for (int k = 0; k < 3; k++)
		{
			debug_colors[3+k][0] = 0;			// Azul
			debug_colors[3+k][1] = 0;			// Azul
			debug_colors[3+k][2] = 255;			// Azul
		}
	}

	IplImage* live = cvCreateImage(cvSize(2*270,2*102),8,3);	
	cvNamedWindow("Live");
	cvNamedWindow("Level");
	cvResizeWindow("Level", 600, 650);
	cvNamedWindow("Filtro Atual");
	cvNamedWindow("Estrategia");
	cvNamedWindow("PID");
	cvResizeWindow("PID", 1000, 600);

	// Inicialização da Visão
	vision Camera;
	Camera.createCamera(0);

	// Inicialização da Estratégia
	estrategia Estrategia;
	
	// Inicialização do Controle
	//controle Controle00((float)0.2, (float)0.007, (float)6, (float)0.3, (float)0.000001, (float)4);
	controle Controle00((pp_f/100.0),(pi1_f/100.0), (pd_f/100.0), (pp_t/100.0), (pi1_t/100.0), (pd_t/100.0));

	// Inicialização do Serial
	Serial* SP = new Serial("COM5"); 
	if (SP->IsConnected())
		printf("We're connected\n");

	start = clock();
	while(1)
	{
		//cout << "PP Frente = " << pp_f/100.0 << endl;
		//cout << "PI1 Frente = " << pi1_f/100.0 << endl;
		//cout << "PD Frente = " << pd_f/100.0 << endl;
		//cout << "PP Tras = " << pp_t/100.0 << endl;
		//cout << "PI1 Tras = " << pi1_t/100.0 << endl;
		//cout << "PD Tras = " << pd_t/100.0 << endl;
		//cout << "================================================="<< endl;

//		cvSetMouseCallback("Live", mouseEvent, 0);

		// Visão
		cvCreateTrackbar("Cor","Level", &p, 5, NULL);
		cvCreateTrackbar("Hmin", "Level", &parametro[0+6*p], 180, NULL);
		cvCreateTrackbar("Smin", "Level", &parametro[1+6*p], 255, NULL);
		cvCreateTrackbar("Vmin", "Level", &parametro[2+6*p], 255, NULL);
		cvCreateTrackbar("Hmax", "Level", &parametro[3+6*p], 180, NULL);
		cvCreateTrackbar("Smax", "Level", &parametro[4+6*p], 255, NULL);
		cvCreateTrackbar("Vmax", "Level", &parametro[5+6*p], 255, NULL);
		cvCreateTrackbar("Amin", "Level", &amin, 150, NULL);
		cvCreateTrackbar("Amax", "Level", &amax, 150, NULL);
		cvCreateTrackbar("MinDist", "Level", &min_distance, 200, NULL);
		cvCreateTrackbar("FlagPID", "Level", &flag_pid, 1, NULL);
		cvCreateTrackbar("Flag Pontof", "Level", &flag_pontof, 3, NULL);

		
		cvCreateTrackbar("PP Frente", "PID", &pp_f, 1000, NULL);
		cvCreateTrackbar("PI Frente", "PID", &pi1_f, 1000, NULL);
		cvCreateTrackbar("PD Frente", "PID", &pd_f, 1000, NULL);
		cvCreateTrackbar("PP Tras", "PID", &pp_t, 1000, NULL);
		cvCreateTrackbar("PI Tras", "PID", &pi1_t, 1000, NULL);
		cvCreateTrackbar("PD Tras", "PID", &pd_t, 1000, NULL);

		cvCreateTrackbar("PP R Frente", "PID", &pp_r_f, 1000, NULL);
		cvCreateTrackbar("PI R Frente", "PID", &pi1_r_f, 1000, NULL);
		cvCreateTrackbar("PD R Frente", "PID", &pd_r_f, 1000, NULL);
		cvCreateTrackbar("PP R Tras", "PID", &pp_r_t, 1000, NULL);
		cvCreateTrackbar("PI R Tras", "PID", &pi1_r_t, 1000, NULL);
		cvCreateTrackbar("PD R Tras", "PID", &pd_r_t, 1000, NULL);
		
		Camera.configParam(parametro, amin, amax, time, min_distance);
		

		Camera.getImage();
		Camera.filterImage();
		Camera.findBlob();
		Camera.findCircles();
		Camera.analizeBlob();

			//cvSetImageROI(Camera.frame, cvRect(100, 110, 200, 200));

		if (flag_pid == 1) {			
			cout << "%%%%%%%%%%%%%%%%%% PID WINDOW ATIVA %%%%%%%%%%%%%%%%%%"<< endl;
			
			Controle00.setParametros_pid(pp_f/100.0, pi1_f/100.0, pd_f/100.0, pp_t/100.0, pi1_t/100.0, pd_t/100.0);
    		Controle00.setParametros_pid_rotacao(pp_r_f/100.0, pi1_r_f/100.0, pd_r_f/100.0, pp_r_t/100.0, pi1_r_t/100.0, pd_r_t/100.0);

			flag_pid = 0;
		}

		robot ponto_f;
		if (flag_pontof == 0) {
			// bola
			ponto_f.x = Camera.players[6].x;
			ponto_f.y = Camera.players[6].y;
		} else if (flag_pontof == 1) {
			// centro
			ponto_f.x = 263.0;
			ponto_f.y = 103.0;
		} else if (flag_pontof == 2) {
			//canto
			/*ponto_f.x = 476.0;
			ponto_f.y = 195.0;*/
			ponto_f.x = Estrategia.goleiro(Camera.players[6], 60).x;
			ponto_f.y = Estrategia.goleiro(Camera.players[6], 60).y;
		}


		//Estrategia

		// Controle
		Controle00.pid(Camera.players[0], ponto_f);


		// Comunicação
		SP->send(0,Controle00.v.y, Controle00.v.x);


		// Debug em prompt de comando
		//cout << "Carrinho: (" << Camera.players[0].x << ", " << Camera.players[0].y << ", " << Camera.players[0].theta *(180/M_PI) << ")" << endl;
		
		//cout << "Velocidade (d, e): " << Controle00.v.x << ", "<< Controle00.v.y << ")" <<endl;
		
		

		// Debug em janela
		for (int i = 0; i < 7; i++)
		{
			if (Camera.players[i].x != -1)
			{
			cvCircle( Camera.frame, cvPoint((int)Camera.players[i].x, (int)Camera.players[i].y), radius, CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
			cvLine(Camera.frame, cvPoint((int)Camera.players[i].x, (int)Camera.players[i].y), cvPoint((int)Camera.players[i].x + radius*cos(Camera.players[i].theta),
								(int)Camera.players[i].y + radius*sin(Camera.players[i].theta)), CV_RGB(debug_colors[i][0], debug_colors[i][1], debug_colors[i][2]), 2);
			}
		}
		cvResize(Camera.frame, live, CV_INTER_AREA );
		cvLine(live,cvPoint((int)Camera.players[0].x, (int)Camera.players[0].y), cvPoint((int)ponto_f.x, (int)ponto_f.y), CV_RGB(255, 255, 255), 2);
		cvShowImage("Live", live);
		cvShowImage("Filtro Atual", Camera.thresh[p]);
		cvShowImage("Estrategia", Estrategia.field);

		//Escape Sequence
		char_pressed = cvWaitKey(1);
		if(char_pressed == 27)
		{
			SP->send(0, 0, 0);
			break;
		}
		end = clock();
		counter ++;
		cout << "Frame Rate: " << (counter*1000)/(end-start)  << endl;
	}
}
