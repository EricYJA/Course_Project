/*
 * File: arc.cpp
 * --------------
 * This file implements the arc.h interface.
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "arc.h"
#include <iostream>
using namespace cv;
using namespace std;

/*
 * Implementation notes: Constructor
 * ------------------------------------------------
 * The constructor initializes the private fields.
 */

Arc::Arc() {

}

Arc::Arc (QImage * qImageSrc)
	:Pic(qImageSrc) {
		srcImage = getsrc();
		cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
		blur(grayImage, grayImage, Size(3, 3));
}

Arc::Arc(Mat matSrc) {
	srcImage = matSrc;
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
	blur(grayImage, grayImage, Size(3, 3));
}

/*
 * Implementation notes: setResults methods
 * -----------------------------------
 * These functions set the result images into private image respectively.
 */

void Arc::set_canny_white() {
	Mat tmpImage;
	int thresh = 72; //canny operator parameter
	Mat canny;
	Canny(grayImage, canny, thresh, thresh * 2); //using canny operator to detect edge
	findContours(canny, contour, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // RETR_TREE means it will ouput all the contour it detects
	
	// counting moments
	vector<Moments> mu(contour.size());
	for (unsigned int i = 0; i < contour.size(); i++) {
		mu[i] = moments(contour[i], false);
	}

	// couting centrol moments
	vector<Point2f> mc(contour.size());
	for (unsigned int i = 0; i < contour.size(); i++) {
		mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));
	}

	// complete contours
	tmpImage = Mat::zeros(canny.size(), CV_8UC3);
	for (unsigned int i = 0; i < contour.size(); i++) {
		Scalar color = Scalar(255, 255, 255);// contour color will be white
		drawContours(tmpImage, contour, i, color, 2, 8, hierarchy, 0, Point());
		circle(tmpImage, mc[i], 4, color, -1);
	}
	cannyWhiteImage = tmpImage;
}

void Arc::set_canny_color() {
	Mat tmpImage;
	int thresh = 72; //canny operator parameter
	Mat canny;
	Canny(grayImage, canny, thresh, thresh * 2); //using canny operator to detect edge
	findContours(canny, contour, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // RETR_TREE means it will ouput all the contour it detects
	
	// counting moments
	vector<Moments> mu(contour.size());
	for (unsigned int i = 0; i < contour.size(); i++) {
		mu[i] = moments(contour[i], false);
	}
	
	// couting centrol moments
	vector<Point2f> mc(contour.size());
	for (unsigned int i = 0; i < contour.size(); i++) {
		mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));
	}
	
	// complete contours
	tmpImage = Mat::zeros(canny.size(), CV_8UC3);
	for (unsigned int i = 0; i < contour.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)); // contour color will be random
		drawContours(tmpImage, contour, i, color, 2, 8, hierarchy, 0, Point());
		circle(tmpImage, mc[i], 4, color, -1);
	}
	cannyColorImage = tmpImage;
}

void Arc::set_laplacian_white() {
	Mat tmpImage;
	Mat laplacian;
	Laplacian(grayImage, laplacian, CV_8U, 5); // use laplacian operator
	// reverse color
	for (int r = 0; r < laplacian.rows; r++) {
		for (int c = 0; c < laplacian.cols; c++) {
			laplacian.at<uchar>(r, c) = 255 - laplacian.at<uchar>(r, c);
		}
	}
	findContours(laplacian, contour, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // RETR_TREE means it will ouput all the contour it detects
	
	// counting moments
	vector<Moments> mu(contour.size());
	for (unsigned int i = 0; i < contour.size(); i++) {
		mu[i] = moments(contour[i], false);
	}
	
	// couting centrol moments
	vector<Point2f> mc(contour.size());
	for (unsigned int i = 0; i < contour.size(); i++) {
		mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));
	}
	
	// complete contours
	tmpImage = Mat::zeros(laplacian.size(), CV_8UC3);
	for (unsigned int i = 0; i < contour.size(); i++) {
		Scalar color = Scalar(255, 255, 255); // contour color will be white
		drawContours(tmpImage, contour, i, color, 2, 8, hierarchy, 0, Point());
		circle(tmpImage, mc[i], 4, color, -1);
	}
	laplacianImage = tmpImage;
}

/* 
 * Simple getter methods: get_canny_white, 
 *                        get_canny_color,
 *                        get_laplacian_white
 */

QImage Arc::get_canny_white() {
	settmp(cannyWhiteImage);
	confirmPic();
	return getQImage();
}

QImage Arc::get_canny_color() {
	settmp(cannyColorImage);
	confirmPic();
	return getQImage();
}

QImage Arc::get_laplacian_white() {
	settmp(laplacianImage);
	confirmPic();
	return getQImage();
}

/* Simple show methods: show_laplacian_white,
 *                      show_canny_color,
 *                      show_canny_white
 */

void Arc::show_laplacian_white() {
	imshow("laplacian_image", laplacianImage);
	destroyWindow("laplacian_image");
}

void Arc::show_canny_color() {
	imshow("canny_color_image", cannyColorImage);
	destroyWindow("canny_color_image");
}

void Arc::show_canny_white() {
	imshow("canny_white_image", cannyWhiteImage);
	destroyWindow("canny_white_image");
}
