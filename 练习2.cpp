#include<opencv.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
using namespace std;
using namespace cv;
Mat goos(Mat frame,float bgMat[],float fcMat[]) {
	Mat outMat(frame.size(),frame.type());
	int height = frame.rows;
	int width = frame.cols;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (frame.at<uchar>(i, j) - bgMat[i * j + j] > fcMat[i * j + j]){
				outMat.at<uchar>(i, j) = 0;
				}
			else outMat.at<uchar>(i, j) = 255;
		}
		return outMat;
	}
}
Mat f(Mat frame, Mat bgMat, Mat fcMat) {
	Mat outMat(frame.size(), frame.type());
	int height = frame.rows;
	int width = frame.cols;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (frame.at<uchar>(i, j) - bgMat.at<uchar>(i, j) > fcMat.at<uchar>(i, j)) {
				outMat.at<uchar>(i, j) = 255;
			}
			else outMat.at<uchar>(i, j) = 0;
		}
	}
	return outMat;
}
int main() {
	int N = 30;
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) {
		cout << "打不开视频！" << endl;
		return 0;
	}
	Mat frame;
	cap >> frame;
	int height = frame.rows;
	int width = frame.cols;
	int bin = height * width;
	cvtColor(frame, frame, CV_BGR2GRAY);
	Mat BGMat(frame.size(), frame.type()),FCMat(frame.size(),frame.type());
	float* bgMat = new float[bin];
	float* fcMat = new float[bin];
	memset(bgMat, 0, sizeof(float) * bin);
	memset(fcMat, 0, sizeof(float) * bin);
	vector<Mat> nMat(N);
	for (int a = 0; a < N; a++) {
		cap >> frame;
		cvtColor(frame, frame, CV_BGR2GRAY);
		frame.copyTo(nMat[a]);
	}
	/*for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float p = 0;
			for (int a = 0; a < N; a++) {
				p+= nMat[a].at<uchar>(i,j);
			}
			p = p / N;
			bgMat[i * j + j] = static_cast<float>(p);
			float f=0;
			for (int b = 0; b < N; b++) {
				f += (p - nMat[b].at<uchar>(i,j)) * (p - nMat[b].at<uchar>(i,j));
			}
			fcMat[i * j + j] = static_cast<float>(f/N);
		}
	}*/
	for (int i = 0; i <height; i++) {
		for (int j = 0; j < width; j++) {
			int p = 0;
			for (int a = 0; a < N; a++) {
				p += (int)nMat[a].at<uchar>(i, j);
			}
			p = p / 30;
			BGMat.at<uchar>(i, j) = (uchar)p;
			float f = 0;
			for (int b = 0; b < N; b++) {
				f += (nMat[b].at<uchar>(i,j)-p) * (nMat[b].at<uchar>(i,j)-p);
			}
			FCMat.at<uchar>(i, j) =static_cast<uchar>(sqrt(f/N));
		}
	}
	while (1) {
		cap >> frame;
		cvtColor(frame, frame, CV_BGR2GRAY);
		Mat outMat = f(frame, BGMat, FCMat);
		//Mat outMat = goos(frame, bgMat, fcMat);
		imshow("out", outMat);
		waitKey(30);
	}
	//cout << nMat[18].at<uchar>(0, 0) << endl;
	//cout << height << "    " << width << endl;
	delete[] bgMat;
	delete[] fcMat;
	return 0;
}