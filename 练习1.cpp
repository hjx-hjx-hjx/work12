#include<opencv.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int main() {
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) {
		cout << "打不开摄像头!" << endl;
		return -1;
	}
	Mat frame,bgMat,subMat,bny_subMat;
	int cnt = 0;
	while (1) {
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt == 0) {
			frame.copyTo(bgMat);
			cnt++;
		}
		else {
			absdiff(frame, bgMat, subMat);
			threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);
			imshow("b_subMat", bny_subMat);
			imshow("subMat",subMat);
			waitKey(30);
		}
	}
	return 0;
}