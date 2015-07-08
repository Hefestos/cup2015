#pragma once

#include "../cvblobs/cvblob.h"
#include "../robot/robot.h"
#include <opencv/cv.h> 
#include <opencv/highgui.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <thread>
#define M_PI 3.14159265358979323846

#ifndef VISION_H
#define VISION_H

class vision
{
public:
	vision(void);

	~vision(void);
	
	void configParam(int*, int, int, int, int);
	void configParam(int*, int, int, int, int, CvRect);
	
	void createCamera(int);
	
	bool getImage(CvRect);
	bool getImage(void);
	bool getImage2(void);
	
	void filterImage(void);
	void filterImageI(int);

	void findBlob(void);
	void findBlobI(int);
	
	void findCircles_old(void);
	void findCircles(void);
	void findCirclesI(int);

	void ballPrediction(void);

	void findCar_old(void);
	void findCar(robot, robot, int);

	void analizeBlob(void);

	void dataAcquire(void);

	float distance(robot, robot);


	int time;

	std::vector<robot> blue;
	std::vector<robot> yellow;
	std::vector<robot> green;
	std::vector<robot> purple;
	std::vector<robot> brown;
	std::vector<robot> orange;
	
	robot players[7];

	IplImage *original;
	IplImage *frame;
	IplImage *fram;
	IplImage *hsvframe;

	IplImage *labelImg[6];
	IplImage *thresh[6];
	cvb::CvBlobs blobs[6];
	

	CvCapture *capture;
	
	CvPoint circlePos[6];
	CvPoint bluePos;
	CvPoint yellowPos;
	CvPoint3D32f one_car;

	CvRect select;

	robot ball_old;
	int h, w;
	int cores[6][6];
	int radius;
	int area_min, area_max;
};

#endif