/*
 * File: cartoon.cpp
 * --------------
 * This file implements the cartoon.h interface.
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cartoon.h"
#include <iostream>
#include <math.h>
#include <istream>
using namespace cv;
using namespace std;

/*
 * Implementation notes: Constructor
 * ------------------------------------------------
 * The constructor initializes the private fields.
 */

Cartoon::Cartoon() {

}

Cartoon::Cartoon (QImage * qImageSrc)
    :Pic(qImageSrc) {
		srcImage = getsrc();
}

Cartoon::Cartoon(Mat matSrc) {
	srcImage = matSrc;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
}

/*
 * Implementation notes: reverse methods
 * -----------------------------------
 * This functions reverse the color of grayimage.
 */

Mat Cartoon::reverse(Mat inputImage) {
	for (int r = 0; r < inputImage.rows; r++) {
		for (int c = 0; c < inputImage.cols; c++) {
			inputImage.at<uchar>(r, c) = 255 - inputImage.at<uchar>(r, c);
		}
	}
	return inputImage;
}

/*
 * Implementation notes: setResults methods
 * -----------------------------------
 * These functions set the result images into private image.
 */

void Cartoon::set_cartoon() {
	medianBlur(grayImage, grayImage, 7); // use median blur to obscue the image
	Mat edge;
	Laplacian(grayImage, edge, CV_8U, 5); // use laplacian operator to detect edge
	Mat Binaryzation;
	threshold(edge, Binaryzation, 80, 255, THRESH_BINARY_INV); // set the range of grayscale
	Mat tmpImage = srcImage.clone();
	for (int i = 0; i < 7; i++) {
		int kernelSize = 9; // convolution kernel
		double sigmaColor = 9;
		double sigmaSpace = 7;
		bilateralFilter(srcImage, tmpImage, kernelSize, sigmaColor, sigmaSpace);
		bilateralFilter(tmpImage, srcImage, kernelSize, sigmaColor, sigmaSpace);
	}
	srcImage.copyTo(cartoonImage, Binaryzation);
}

/* 
 * Simple show methods: show_cartoon
 */

void Cartoon::show_cartoon() {
	imshow("cartoon_image", cartoonImage);
	waitKey(0);
	destroyWindow("cartoon_image");
}

/* 
 * Simple getter methods: get_cartoon
 */

QImage Cartoon::get_cartoon() {
	settmp(cartoonImage);
	confirmPic();
	return getQImage();
}
