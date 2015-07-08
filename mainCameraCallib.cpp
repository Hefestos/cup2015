#define _CRT_SECURE_NO_DEPRECATE

#include"vision\vision.h"
#include"robot\robot.h"
#include <time.h>


//NameSpaces
using namespace cvb;
using namespace cv;
using namespace std;

Mat image;

int ponto_area = 0;
bool canto1 = true;
robot canto_gol1 = robot(-1, -1), canto_gol2 = robot(-1, -1);
robot canto_area1 = robot(-1, -1), canto_area2 = robot(-1, -1);

int mode = 1;
bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
CvRect selection;
robot clicked(0, 0);
robot mouse(0,0);
robot mouse2(0, 0);

//callback function
void mouseEvent(int evt, int x, int y, int flags, void* param){
	mouse.x = x;
	mouse.y = y;

	if (evt == CV_EVENT_LBUTTONDOWN){
		clicked.x = x;
		clicked.y = y;
	}

	if (selectObject)
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);

		//selection &= Rect(0, 0, image.cols, image.rows);
	}


	switch (evt)
	{
	case CV_EVENT_LBUTTONDOWN:
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		selectObject = true;
		break;
	case CV_EVENT_LBUTTONUP:
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
			trackObject = -1;
		break;
	}
}


void mouseEvent2(int evt, int x, int y, int flags, void* param){
	mouse2.x = x;
	mouse2.y = y;

	if (mode == 2)
	{
		if (evt == CV_EVENT_LBUTTONDOWN)
		{
			if (canto1)
			{
				canto1 = false;
				canto_gol1 = robot(x, y);
			}
			else
			{
				canto1 = true;
				canto_gol2 = robot(x, y);
			}
		}
	}
	if (mode == 3)
	{
		if (evt == CV_EVENT_LBUTTONDOWN)
		{
			if (ponto_area == 1)
			{
				ponto_area++;
				canto_area1 = robot(x, y);
			}
			else
			{
				ponto_area = 1;
				canto_area2 = robot(x, y);
			}
		}
	}
}



int main(int argc, const char** argv)
{
	clock_t visionI, visionF, estraI, estraF, controlI, controlF, commI, commF, totalI, totalF, debugI, debugF, start;

	float counter = 0;
	int p=0;
	char char_pressed;
	int time = 1;


	// ========== Casa de Goias =========///
	//int amin = 11;
	//int amax = 238;
	//int radius = 20;
	//int min_distance = 13;
	//int parametro[36] = { 88, 139, 80, 121, 253, 255,
	//	22, 59, 107, 55, 204, 255,
	//	126, 43, 81, 141, 171, 247,
	//	52, 82, 94, 97, 168, 210,
	//	0, 80, 60, 14, 118, 166,
	//	0, 121, 142, 18, 255, 255 };


	//CvRect ROI = cvRect(20, 69, 271, 164);
	//robot canto_area1 = robot(54, 36);
	//robot canto_area2 = robot(224, 123);
	//robot canto_gol1 = robot(19, 48);
	//robot canto_gol2 = robot(257, 113);
	int amin = 23;
	int amax = 173;
	int radius = 20;
	int min_distance = 12;
	int parametro[36] = { 88, 139, 80, 121, 253, 255,
		21, 62, 114, 60, 243, 255,
		160, 101, 153, 180, 146, 255,
		55, 189, 91, 94, 229, 255,
		3, 102, 83, 17, 199, 229,
		0, 85, 135, 21, 255, 255 };



	int debug_colors[7][3] = {{128,	0,		128},		// Roxo
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

	CvFont fonte;
	cvInitFont(&fonte, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 2, 8);
	

	
	cvNamedWindow("Live", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Cropped", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Level");
	cvResizeWindow("Level", 600, 650);
	cvNamedWindow("Filtro Atual");


	// Inicialização da Visão
	vision Camera;
	Camera.createCamera(0);


	start = clock();
	while (1)
	{

		counter++;
		totalI = clock();
		cvCreateTrackbar("Cor", "Level", &p, 5, NULL);
		cvCreateTrackbar("Hmin", "Level", &parametro[0 + 6 * p], 180, NULL);
		cvCreateTrackbar("Smin", "Level", &parametro[1 + 6 * p], 255, NULL);
		cvCreateTrackbar("Vmin", "Level", &parametro[2 + 6 * p], 255, NULL);
		cvCreateTrackbar("Hmax", "Level", &parametro[3 + 6 * p], 180, NULL);
		cvCreateTrackbar("Smax", "Level", &parametro[4 + 6 * p], 255, NULL);
		cvCreateTrackbar("Vmax", "Level", &parametro[5 + 6 * p], 255, NULL);
		cvCreateTrackbar("Amin", "Level", &amin, 1000, NULL);
		cvCreateTrackbar("Amax", "Level", &amax, 1000, NULL);
		cvCreateTrackbar("MinDist", "Level", &min_distance, 500, NULL);


		Camera.configParam(parametro, amin, amax, time, min_distance);


		// ------------------VISÃO ------------------//
		visionI = clock();
		Camera.getImage(selection);
		Camera.dataAcquire();
		visionF = clock();
		//cout << "Tempo de analise de imagem: " << (visionF - visionI) << endl;

		// Debug em prompt de comando
		debugI = clock();
		//system("cls");
		//cout << clicked.x << ", " << clicked.y << endl;
		//cout << selection.x << ", " << selection.y << ", " << selection.width << ", " << selection.height << endl;


		// Debug em janela

		cvSetMouseCallback("Live", mouseEvent, 0);

		cvSetMouseCallback("Cropped", mouseEvent2, 0);



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

		cvLine(Camera.fram, cvPoint(mouse.x, 0), cvPoint(mouse.x, Camera.fram->height), CV_RGB(0, 0, 0), 1);
		cvLine(Camera.fram, cvPoint(0, mouse.y), cvPoint(Camera.fram->width, mouse.y), CV_RGB(0, 0, 0), 1);

		cvLine(Camera.frame, cvPoint(mouse2.x, 0), cvPoint(mouse2.x, Camera.frame->height), CV_RGB(0, 0, 0), 1);
		cvLine(Camera.frame, cvPoint(0, mouse2.y), cvPoint(Camera.frame->width, mouse2.y), CV_RGB(0, 0, 0), 1);

		if (mode == 2)
		{
			cvLine(Camera.frame, cvPoint(canto_gol1.x, 0), cvPoint(canto_gol1.x, Camera.frame->height), CV_RGB(255, 255, 255), 2);
			cvLine(Camera.frame, cvPoint(0, canto_gol1.y), cvPoint(Camera.frame->width, canto_gol1.y), CV_RGB(255, 255, 255), 2);

			cvLine(Camera.frame, cvPoint(canto_gol2.x, 0), cvPoint(canto_gol2.x, Camera.frame->height), CV_RGB(255, 255, 255), 2);
			cvLine(Camera.frame, cvPoint(0, canto_gol2.y), cvPoint(Camera.frame->width, canto_gol2.y), CV_RGB(255, 255, 255), 2);
		}
		if (mode == 3)
		{
			if ((canto_area1.x != -1) && (canto_area2.x != -1))
			{
				cvLine(Camera.frame, cvPoint(0, canto_area1.y), cvPoint(canto_area1.x, canto_area1.y), CV_RGB(255, 255, 255), 2);
				cvLine(Camera.frame, cvPoint(canto_area1.x, canto_area1.y), cvPoint(canto_area1.x, canto_area2.y), CV_RGB(255, 255, 255), 2);
				cvLine(Camera.frame, cvPoint(0, canto_area2.y), cvPoint(canto_area1.x, canto_area2.y), CV_RGB(255, 255, 255), 2);

				cvLine(Camera.frame, cvPoint(Camera.frame->width, canto_area1.y), cvPoint(canto_area2.x, canto_area1.y), CV_RGB(255, 255, 255), 2);
				cvLine(Camera.frame, cvPoint(canto_area2.x, canto_area1.y), cvPoint(canto_area2.x, canto_area2.y), CV_RGB(255, 255, 255), 2);
				cvLine(Camera.frame, cvPoint(canto_area2.x, canto_area2.y), cvPoint(Camera.frame->width, canto_area2.y), CV_RGB(255, 255, 255), 2);
			}
		}



		cvShowImage("Live", Camera.fram);
		cvShowImage("Cropped", Camera.frame);
		cvShowImage("Filtro Atual", Camera.thresh[p]);

		//Escape Sequence
		char_pressed = cvWaitKey(1);
		if (char_pressed == 27)
			break;
		if (char_pressed == 112)
		{
			system("cls");
			cout << "CvRect ROI = cvRect(" << selection.x << ", " << selection.y << ", " << selection.width << ", " << selection.height << ");" << endl;


			cout << "robot canto_area1 = robot(" << canto_area1.x << ", " << canto_area1.y << ");" << endl;
			cout << "robot canto_area2 = robot(" << canto_area2.x << ", " << canto_area2.y << ");" << endl;

			cout << "robot canto_gol1 = robot(" << canto_gol1.x << ", " << canto_gol1.y << ");" << endl;
			cout << "robot canto_gol2 = robot(" << canto_gol2.x << ", " << canto_gol2.y << ");" << endl;

			cout << "int amin = " << amin << ";" << endl;
			cout << "int amax = " << amax << ";" << endl;
			cout << "int radius = " << radius << ";" << endl;
			cout << "int min_distance = " << min_distance << ";" << endl;

			cout << "int parametro[36] = { ";
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					cout << parametro[i * 6 + j];
					if ((i * 6 + j) != 35)
						cout << ", ";
				}
				if (i < 5)
					cout << endl << "    ";
			}
			cout << "};" << endl;
		}
		if (char_pressed == 49)
			mode = 1;
		if (char_pressed == 50)
		{
			mode = 2;
			canto1 = true;
		}
		if (char_pressed == 51)
		{
			mode = 3;
			ponto_area = 1;
		}


		debugF  = clock();
		totalF = clock();
		//cout << "Tempo do Debug:" << debugF - debugI<< endl;
		//cout << "Tempo Total: " << (totalF - totalI) << endl;
		//cout << "Frame Rate: " << 1000 * counter/ (totalF - start) << endl;

		//cout << counter << endl;
	}
}
