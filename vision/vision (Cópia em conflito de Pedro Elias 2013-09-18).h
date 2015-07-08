#pragma once

#include "../cvblobs/cvblob.h"
#include "../robot/robot.h"
#include<opencv/cv.h> 
#include<highgui.h>
#include<stdio.h>
#include<vector>
#include <cmath>
#define M_PI 3.14159265358979323846

#ifndef VISION_H
#define VISION_H

class vision
{
public:
	vision(void);

	~vision(void);
	
	void configParam(int*, int, int, int, int);
	
	void createCamera(int);
	
	bool getImage(void);
	
	void filterImage(void);
	
	void findBlob(void);
	
	void findCircles_old(void);
	void findCircles(void);

	void findCar_old(void);
	void findCar(circle, circle, int);

	void analizeBlob();

	float distance(circle, circle);


	int time;

	std::vector<circle> blue;
	std::vector<circle> yellow;
	std::vector<circle> green;
	std::vector<circle> purple;
	std::vector<circle> brown;
	std::vector<circle> orange;
	robot players[7];


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


	int cores[6][6];
	int radius;
	int area_min, area_max;
};

#endif