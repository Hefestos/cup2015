#include "vision.h"

using namespace std;
using namespace cvb;


vision::vision(void)
{
	h = 195;
	w = 320;

	fram = cvCreateImage(cvSize(w, h), 8, 3);   //Original Image
	frame = cvCreateImage(cvSize(w,h),8,3);   //Original Image
	hsvframe = cvCreateImage(cvSize(w,h),8,3);//Image in HSV color space

	for (int i = 0; i < 6; i++)
	{
		thresh[i] = cvCreateImage(cvSize(w,h),8,1);
		labelImg[i] = cvCreateImage(cvSize(w,h), IPL_DEPTH_LABEL, 1);
	}
}

vision::~vision(void)
{
	//Cleanup
	cvReleaseCapture(&capture);
	cvDestroyAllWindows();
}

void vision::configParam(int *new_cor, int new_area_min, int new_area_max, int new_time, int new_radius, CvRect selection)
{
	radius = new_radius;
	time = new_time;
	for(int i=0; i<6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cores[i][j] = new_cor[6*i+j];
		}
	}
	area_min = new_area_min;
	area_max = new_area_max;

	imgROI = selection;
}

void vision::createCamera(int num)
{
	capture=cvCreateCameraCapture(num);
	//printf("Width: %f \n", (float)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH));
	//printf("Height: %f \n", (float)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 320);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
	//printf("Width: %f \n", (float)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH));
	//printf("Height: %f \n", (float)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
}

bool vision::getImage(void)
{
	cvReleaseImage(&frame);
	cvReleaseImage(&hsvframe);
	for (int i = 0; i < 6; i++)
	{
		cvReleaseImage(&thresh[i]);
		cvReleaseImage(&labelImg[i]);
	}

	fram=cvQueryFrame(capture);
	if(!fram)
		return false;
	if ((imgROI.width != 0) && (imgROI.height != 0))
		cvSetImageROI(fram, imgROI);

	frame = cvCreateImage(cvGetSize(fram), fram->depth, fram->nChannels);
	cvCopy(fram, frame, NULL);


	h = frame->height;
	w = frame->width;

	for (int i = 0; i < 6; i++)
	{
		thresh[i] = cvCreateImage(cvGetSize(frame), 8, 1);
		labelImg[i] = cvCreateImage(cvGetSize(frame), IPL_DEPTH_LABEL, 1);
	}

	hsvframe = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);

	cvResetImageROI(fram);

	return true;
	
}

bool vision::getImage(CvRect select)
{
	cvReleaseImage(&frame);
	cvReleaseImage(&hsvframe);
	cvReleaseImage(&fram);
	for (int i = 0; i < 6; i++)
	{
		cvReleaseImage(&thresh[i]);
		cvReleaseImage(&labelImg[i]);
	}

	original = cvQueryFrame(capture);
	if (!original)
		return false;
	
	//fram = cvCreateImage(cvSize((original->width) / 2, (original->height) / 2), original->depth, original->nChannels);

	cvResize(original, fram);

	if ((select.width != 0) && (select.height != 0))
		cvSetImageROI(fram, select);

	frame = cvCreateImage(cvGetSize(fram), fram->depth, fram->nChannels);
	cvCopy(fram, frame, NULL);

	

	h = frame->height;
	w = frame->width;

	for (int i = 0; i < 6; i++)
	{
		thresh[i] = cvCreateImage(cvGetSize(frame), 8, 1);
		labelImg[i] = cvCreateImage(cvGetSize(frame), IPL_DEPTH_LABEL, 1);
	}

	hsvframe = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);

	cvResetImageROI(fram);

	return true;

}

bool vision::getImage2(void)
{
	IplImage *fram = cvQueryFrame(capture);
	if (!fram)
		return false;
	//cvSetImageROI(fram, cvRect(2 * 50, 2 * 58, 2 * 320, 2 * 160));
	cvResize(fram, frame, CV_INTER_LINEAR);
	return true;
}

void vision::filterImageI(int i)
{
	//Thresholding the frame for colour [i]

	cvInRangeS(hsvframe,cvScalar(cores[i][0], cores[i][1], cores[i][2]),cvScalar(cores[i][3], cores[i][4], cores[i][5]),thresh[i]);
}

void vision::filterImage(void)
{
	//Changing the color space
	cvCvtColor(frame, hsvframe, CV_BGR2HSV);
	//Thresholding the frame for yellow
	for (int i = 0; i < 6; i++)
	{
		cvInRangeS(hsvframe, cvScalar(cores[i][0], cores[i][1], cores[i][2]), cvScalar(cores[i][3], cores[i][4], cores[i][5]), thresh[i]);
	}
}

void vision::findBlobI(int i)	
{
	cvLabel(thresh[i], labelImg[i], blobs[i]);
	//Filtering the blobs
	cvFilterByArea(blobs[i], area_min, area_max);
}

void vision::findBlob(void)
{
	for (int i = 0; i < 6; i++) {
		cvLabel(thresh[i], labelImg[i], blobs[i]);
		//Filtering the blobs
		cvFilterByArea(blobs[i], area_min, area_max);
	}
}

void vision::dataAcquire(void)
{
	std::vector < std::thread> threads;
	cvCvtColor(frame, hsvframe, CV_BGR2HSV);


	for (int i = 0; i < 6; i++)
		threads.push_back(std::thread(&vision::filterImageI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();
	
	for (int i = 0; i < 6; i++)
		threads.push_back(std::thread(&vision::findBlobI, this, i));
	for (auto& th : threads) th.join();

	threads.clear();
	
	findCircles();
	
	//for (int i = 0; i < 6; i++)
	//	threads.push_back(std::thread(&vision::findCirclesI, this, i));
	//for (auto& th : threads) th.join();

	//threads.clear();

	analizeBlob();
}

void vision::findCircles_old(void)
{
	for (int i = 0; i< 2; i++)
	{
		for (CvBlobs::const_iterator it=blobs[i].begin(); it!=blobs[i].end(); ++it)
		{
			double moment10 = it->second->m10;
			double moment01 = it->second->m01;
			double area = it->second->area;
			//Variable for holding position
			//Calculating the current position
			circlePos[i].x = moment10/area;
			circlePos[i].y = moment01/area;
		}
	}
}

void vision::findCirclesI(int i)
{
	blue.clear();
	yellow.clear();
	green.clear();
	purple.clear();
	brown.clear();
	orange.clear();
	robot temp;


	for (CvBlobs::const_iterator it = blobs[i].begin(); it != blobs[i].end(); ++it)
	{
		double moment10 = it->second->m10;
		double moment01 = it->second->m01;
		double area = it->second->area;

		switch (i)
		{
		case 0: //azul
			temp = robot(moment10 / area, moment01 / area);
			blue.push_back(temp);
			break;
		case 1: //amarelo
			temp = robot(moment10 / area, moment01 / area);
			yellow.push_back(temp);
			break;
		case 2: //roxo
			temp = robot(moment10 / area, moment01 / area);
			purple.push_back(temp);
			break;
		case 3: //verde
			temp = robot(moment10 / area, moment01 / area);
			green.push_back(temp);
			break;
		case 4: //marrom
			temp = robot(moment10 / area, moment01 / area);
			brown.push_back(temp);
			break;
		case 5: //laranja
			temp = robot(moment10 / area, moment01 / area);
			orange.push_back(temp);
			break;
		}
	}
}

void vision::findCircles(void)
{
	blue.clear();
	yellow.clear();
	green.clear();
	purple.clear();
	brown.clear();
	orange.clear();
	robot temp;

	for (int i = 0; i< 6; i++)
	{
		for (CvBlobs::const_iterator it=blobs[i].begin(); it!=blobs[i].end(); ++it)
		{
			double moment10 = it->second->m10;
			double moment01 = it->second->m01;
			double area = it->second->area;
			
			switch (i)
			{
			case 0: //azul
				temp = robot(moment10 / area, moment01 / area);
				blue.push_back(temp);
				break;
			case 1: //amarelo
				temp = robot(moment10 / area, moment01 / area);
				yellow.push_back(temp);
				break;
			case 2: //roxo
				temp = robot(moment10 / area, moment01 / area);
				purple.push_back(temp);
				break;
			case 3: //verde
				temp = robot(moment10 / area, moment01 / area);
				green.push_back(temp);
				break;
			case 4: //marrom
				temp = robot(moment10 / area, moment01 / area);
				brown.push_back(temp);
				break;
			case 5: //laranja
				temp = robot(moment10 / area, moment01 / area);
				orange.push_back(temp);
				break;
			}
		}
	}
}

void vision::findCar_old(void)
{
	one_car.x = (circlePos[0].x + circlePos[1].x)/2;
	one_car.y = (circlePos[0].y + circlePos[1].y)/2;
	one_car.z = atan2(double(circlePos[0].y - circlePos[1].y), double(circlePos[0].x - circlePos[1].x)) + M_PI/4;
}

void vision::findCar(robot primary, robot secondary, int index)
{
	players[index].x = (primary.x + secondary.x)/2;
	players[index].y = (primary.y + secondary.y)/2;
	players[index].theta = atan2(double(primary.y - secondary.y), double(primary.x - secondary.x)) + M_PI/4;
}

float vision::distance(robot first, robot second)
{
	float distance = 0;
	distance = sqrt(pow((first.x-second.x),2)+pow(first.y-second.y,2));

	return distance;
}

void vision::analizeBlob()
{
	bool found = false;
	float it_distance;
	float min_distance;
	int index_blue, index_yellow, index_purple, index_green, index_brown;


	if (orange.size() == 1)
		players[6] = orange[0];

	else
		players[6] = robot(-1, -1);


	if (time == 0)
	{
		if ((purple.size() > 0) && (blue.size() > 0))
		{
			index_purple = -1;
			index_blue = -1;
			min_distance = radius;
			for (int i = 0; i < purple.size(); i++)
			{
				for (int j = 0; j < blue.size(); j++)
				{
					it_distance = distance(purple[i], blue[j]);
					if (it_distance < min_distance)
					{
						index_purple = i;
						index_blue = j;
						min_distance = it_distance;
					}
				}
			}
			if ((index_purple != -1) && (index_blue != -1))
			{
				findCar(blue[index_blue], purple[index_purple], 0);
				blue.erase(blue.begin() + index_blue);
			}
			else
				players[0] = robot(-1, -1);
		}
		else
			players[0] = robot(-1, -1);

		
		if ((green.size() > 0) && (blue.size() > 0))
		{
			index_green = -1;
			index_blue = -1;
			min_distance = radius;
			for (int i = 0; i < green.size(); i++)
			{
				for (int j = 0; j < blue.size(); j++)
				{
					it_distance = distance(green[i], blue[j]);
					if (it_distance < min_distance)
					{
						index_green = i;
						index_blue = j;
						min_distance = it_distance;
					}
				}
			}
			if ((index_green != -1) && (index_blue != -1)) {
				findCar(blue[index_blue], green[index_green], 1);
				blue.erase(blue.begin() + index_blue);
			}
			else
				players[1] = robot(-1, -1);
		}
		else
			players[1] = robot(-1, -1);

		if ((brown.size() > 0) && (blue.size() > 0))
		{
			index_brown = -1;
			index_blue = -1;
			min_distance = radius;
			for (int i = 0; i < brown.size(); i++)
			{
				for (int j = 0; j < blue.size(); j++)
				{
					it_distance = distance(brown[i], blue[j]);
					if (it_distance < min_distance)
					{
						index_brown = i;
						index_blue = j;
						min_distance = it_distance;
					}
				}
			}
			if ((index_brown != -1) && (index_blue != -1)) {
				findCar(blue[index_blue], brown[index_brown], 2);
				blue.erase(blue.begin() + index_blue);
			}

			else
				players[2] = robot(-1, -1);
		}
		else
				players[2] = robot(-1, -1);

		for (int k = 0; k < 3; k++)
		{
			if (k < yellow.size())
				players[3+k] = yellow[k];

			else
				players[3+k] = robot(-1, -1);
		}
	} 
	else
	{
		if ((purple.size() > 0) && (yellow.size() > 0))
		{
			index_purple = -1;
			index_yellow = -1;
			min_distance = radius;
			for (int i = 0; i < purple.size(); i++)
			{
				for (int j = 0; j < yellow.size(); j++)
				{
					it_distance = distance(purple[i], yellow[j]);
					if (it_distance < min_distance)
					{
						index_purple = i;
						index_yellow = j;
						min_distance = it_distance;
					}
				}
			}
			if ((index_purple != -1) && (index_yellow != -1)) {
				findCar(yellow[index_yellow], purple[index_purple], 0);
				yellow.erase(yellow.begin() + index_yellow);
			}
			else
				players[0] = robot(-1, -1);
		}
		else
			players[0] = robot(-1, -1);
		
		if ((green.size() > 0) && (yellow.size() > 0))
		{
			index_green = -1;
			index_yellow = -1;
			min_distance = radius;
			for (int i = 0; i < green.size(); i++)
			{
				for (int j = 0; j < yellow.size(); j++)
				{
					it_distance = distance(green[i], yellow[j]);
					if (it_distance < min_distance)
					{
						index_green = i;
						index_yellow = j;
						min_distance = it_distance;
					}
				}
			}
			if ((index_green != -1) && (index_yellow != -1)) {
				findCar(yellow[index_yellow], green[index_green], 1);
				yellow.erase(yellow.begin() + index_yellow);
			}
			else
				players[1] = robot(-1, -1);

		}
		else
		{
			players[1] = robot(-1, -1);
		}

		if ((brown.size() > 0) && (yellow.size() > 0))
		{
			index_brown = -1;
			index_yellow = -1;
			min_distance = radius;
			for (int i = 0; i < brown.size(); i++)
			{
				for (int j = 0; j < yellow.size(); j++)
				{
					it_distance = distance(brown[i], yellow[j]);
					if (it_distance < min_distance)
					{
						index_brown = i;
						index_yellow = j;
						min_distance = it_distance;
					}
				}
			}
			if ((index_brown != -1) && (index_yellow != -1)) {
				findCar(yellow[index_yellow], brown[index_brown], 2);
				yellow.erase(yellow.begin() + index_yellow);
			}

			else
				players[2] = robot(-1, -1);
		}
		else
			players[2] = robot(-1, -1);

		for (int k = 0; k < 3; k++)
		{
			if (k < blue.size())
				players[3+k] = blue[k];

			else
				players[3+k] = robot(-1, -1);
		}
	}
}