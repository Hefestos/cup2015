#include <opencv2/opencv.hpp>

using namespace cv;

SimpleBlobDetector::Params params;


VideoCapture capture(1);
Mat frame;

bool stop = false;


int main(){
	Ptr<FeatureDetector> blob_detector = new SimpleBlobDetector(params);
	blob_detector->create("SimpleBlob");
	vector<cv::KeyPoint> keypoints;
	
	
	while(!stop){
		
		if (!capture.read(frame))
			break;
		imshow( "Resultado", frame );
		waitKey(10);
		
		blob_detector->detect(frame, keypoints);
		
		for (int i=0; i<keypoints.size(); i++){
			float X=keypoints[i].pt.x; 
			float Y=keypoints[i].pt.y;
		}

		for (int i=0; i<keypoints.size(); i++){
			printf("X:%f  ",keypoints[i].pt.x);
			printf("Y:%f  \n",keypoints[i].pt.y);
			printf("size: %f", (float) keypoints.size());

			//float X=keypoints[i].pt.x; 
			//float Y=keypoints[i].pt.y;
		}	

	}



}