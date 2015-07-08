//OpenCV Headers
#include<cv.h>
#include<highgui.h>
//Input-Output
#include<stdio.h>
//Blob Library Headers
#include<cvblob.h>
//Definitions
#define h 480
#define w 640
//NameSpaces
using namespace cvb;
using namespace std;
int main()
{
	int hmin = 0, hmax = 180;
	int smin = 0, smax = 255;
	int vmin = 0, vmax = 255;
	int amin = 0, amax = 1000;

	//Structure to get feed from CAM
	CvCapture* capture=cvCreateCameraCapture(1);
	//Structure to hold blobs
	CvBlobs blobsBlue;
	CvBlobs blobsYellow;

	//Windows
	
	
	cvNamedWindow("Live");
	cvNamedWindow("Level");
	cvResizeWindow("Level",400,400);
	
	cvCreateTrackbar("Hmin", "Level", &hmin, 180, NULL);
	cvCreateTrackbar("Hmax", "Level", &hmax, 180, NULL);
	cvCreateTrackbar("Smin", "Level", &smin, 255, NULL);
	cvCreateTrackbar("Smax", "Level", &smax, 255, NULL);
	cvCreateTrackbar("Vmin", "Level", &vmin, 255, NULL);
	cvCreateTrackbar("Vmax", "Level", &vmax, 255, NULL);
	cvCreateTrackbar("Amin", "Level", &amin, 1000, NULL);
	cvCreateTrackbar("Amax", "Level", &amax, 1000, NULL);

	
	//Image Variables
	IplImage *frame=cvCreateImage(cvSize(w,h),8,3);   //Original Image
	IplImage *hsvframe=cvCreateImage(cvSize(w,h),8,3);//Image in HSV color space
	IplImage *labelImgYellow=cvCreateImage(cvSize(w,h),IPL_DEPTH_LABEL,1);//Image Variable for blobs
	IplImage *labelImgBlue=cvCreateImage(cvSize(w,h),IPL_DEPTH_LABEL,1);//Image Variable for blobs
	IplImage *threshYellow=cvCreateImage(cvSize(w,h),8,1); //Threshold image of yellow color
	IplImage *threshBlue=cvCreateImage(cvSize(w,h),8,1); //Threshold image of blue color
	//IplImage *threshYellow=cvCreateImage(cvSize(w,h),8,1); //Threshold image of yellow color
 
	//Getting the screen information
	int screenx = 1600;
	int screeny = 900;
 
	while(1)
	{
		//Getting the current frame
		IplImage *fram=cvQueryFrame(capture);
		//If failed to get break the loop
		if(!fram)
			break;
		//Resizing the capture
		cvResize(fram,frame,CV_INTER_LINEAR );
		//Flipping the frame
		cvFlip(frame,frame,1);
		//Changing the color space
		cvCvtColor(frame,hsvframe,CV_BGR2HSV);
		//Thresholding the frame for yellow
		cvInRangeS(hsvframe,cvScalar(hmin,smin,vmin),cvScalar(hmax,smax,vmax),threshBlue);
		cvInRangeS(hsvframe,cvScalar(23,41,133),cvScalar(40,150,255),threshYellow);
		//cvInRangeS(hsvframe,cvScalar(153,102,76),cvScalar(178,255,255),threshYellow);

		//Filtering the frame
		cvSmooth(threshYellow,threshYellow,CV_MEDIAN,7,7);
		cvSmooth(threshBlue,threshBlue,CV_MEDIAN,7,7);



		//Finding the blobs
		unsigned int resultYellow=cvLabel(threshYellow,labelImgYellow,blobsYellow);
		unsigned int resultBlue=cvLabel(threshBlue,labelImgBlue,blobsBlue);
		//Filtering the blobs
		cvFilterByArea(blobsYellow,amin,amax);
		cvFilterByArea(blobsBlue,amin,amax);

		cvNamedWindow("FilterYellow");
		cvShowImage("FilterYellow",threshYellow);
		//cvResizeWindow("FilterYellow",400,400);

		cvNamedWindow("FilterBlue");
		cvShowImage("FilterBlue",threshBlue);
		//cvResizeWindow("FilterBlue",400,400);
		//Rendering the blobs
		//cvRenderBlobs(labelImgYellow,blobsYellow,frame,frame);
	
		for (CvBlobs::const_iterator it=blobsYellow.begin(); it!=blobsYellow.end(); ++it)
		{
			double moment10 = it->second->m10;
			double moment01 = it->second->m01;
			double area = it->second->area;
			//Variable for holding position
			int x1;
			int y1;
			//Calculating the current position
			x1 = moment10/area;
			y1 = moment01/area;
			//Mapping to the screen coordinates
			//int x=(int)(x1*screenx/w);
			//int y=(int)(y1*screeny/h);
			//Printing the position information
			cout<<"Yellow X: "<<x1<<" Yellow Y: "<<y1<<endl;
		}

		for (CvBlobs::const_iterator it=blobsBlue.begin(); it!=blobsBlue.end(); ++it)
		{
			double moment10 = it->second->m10;
			double moment01 = it->second->m01;
			double area = it->second->area;
			//Variable for holding position
			int x1;
			int y1;
			//Calculating the current position
			x1 = moment10/area;
			y1 = moment01/area;
			//Mapping to the screen coordinates
			//int x=(int)(x1*screenx/w);
			//int y=(int)(y1*screeny/h);
			//Printing the position information
			cout<<"Blue X: "<<x1<<" Blue Y: "<<y1<<endl;
		}
		//Showing the images
		cvShowImage("Live",frame);
		//Escape Sequence
		char c=cvWaitKey(33);
		if(c==27)
		break;
	}
	//Cleanup
	cvReleaseCapture(&capture);
	//cvDestroyAllWindows();
 
}