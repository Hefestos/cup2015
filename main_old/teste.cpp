#include <opencv2/opencv.hpp>

using namespace cv;


VideoCapture capture(1);
Mat frame, hsv, bw;

bool stop=false;



int main(){

	int hmin = 0, hmax = 180;
	int smin = 0, smax = 255;
	int vmin = 0, vmax = 255;

	cvNamedWindow("Resultado");
	cvCreateTrackbar("Hmin", "Resultado", &hmin, 180, NULL);
	cvCreateTrackbar("Hmax", "Resultado", &hmax, 180, NULL);
	cvCreateTrackbar("Smin", "Resultado", &smin, 255, NULL);
	cvCreateTrackbar("Smax", "Resultado", &smax, 255, NULL);
	cvCreateTrackbar("Vmin", "Resultado", &vmin, 255, NULL);
	cvCreateTrackbar("Vmax", "Resultado", &vmax, 255, NULL);



	while (!stop)
	{
	if (!capture.read(frame))
		break;
	imshow( "Resultado", frame ); 
	

	cvtColor(frame, hsv, CV_BGR2HSV);
	
	inRange(hsv, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), bw);

	imshow( "filter", bw ); 
	
	int c = cvWaitKey(10);
    if(c!=-1)
    {
        // If pressed, break out of the loop
        break;
    }
	}
}