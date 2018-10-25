/*
 * File: stroke.cpp
 * --------------
 * This file implements the stroke.h interface.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>    
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include "stroke.h"
using namespace cv;
using namespace std;

/*
 * Implementation notes: Constructor
 * ------------------------------------------------
 * The constructor initializes the private fields.
 */

Stroke::Stroke() {

}

Stroke::Stroke (QImage * image)
	:Pic(image) {
		srcImage = getsrc();
        cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

}

Stroke::Stroke(Mat src) {
    srcImage = src;
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
}

/*
 * Implementation notes: setResults methods
 * -----------------------------------
 * These functions set the result images into private image respectively.
 */

void Stroke::set_sketch() {
	Mat reverseGray,tmpImage;
	tmpImage.create(srcImage.size(), CV_8UC1);
	addWeighted(grayImage, -1, NULL, 0, 255, reverseGray); // reverse the color
	GaussianBlur(reverseGray, reverseGray, Size(3, 3), 0); // use gaussianblur to process the reversed image
	
	// dodge color process
	for (int y = 0; y<srcImage.rows; y++) {
		uchar* P0 = grayImage.ptr<uchar>(y);
		uchar* P1 = reverseGray.ptr<uchar>(y);
		uchar* P = tmpImage.ptr<uchar>(y);
		for (int x = 0; x<srcImage.cols; x++) {
			int tmp0 = P0[x];
			int tmp1 = P1[x];
			// dodge color process formula
			P[x] = (uchar)min((tmp0 +(tmp0*tmp1) / (256 - tmp1)), 255);
		}
	}
    Mat bigGamma;
    float bGamma = 2.8; // gamma parameter
	gammaCorrection(tmpImage, bigGamma, bGamma);
	sketchImage = tmpImage;
}

void Stroke::set_stroke() {
    Mat sobel_Image = sobel();
	Mat stroke = convolution(sobel_Image);
	Mat tmpImage;
	addWeighted(sobel_Image, 0.5, stroke, 2.5, 0, tmpImage);
	tmpImage = reverse(tmpImage);
	strokeImage = tmpImage;
}

/* 
 * Simple getter methods: get_matSketch, 
 *                        get_matStroke,
 *                        get_sketch,
 *                        get_stroke
 */

Mat Stroke::get_matSketch() {
    return sketchImage;
}

Mat Stroke::get_matStroke() {
    return strokeImage;
}

QImage Stroke::get_sketch() {
	settmp(sketchImage);
	confirmPic();
	return getQImage();
}

QImage Stroke::get_stroke() {
	settmp(strokeImage);
	confirmPic();
	return getQImage();
}

/* Simple show methods: show_sketch,
 *                      show_stroke
 */

void Stroke::show_sketch() {
	imshow("sketch_image", sketchImage);
	waitKey(0);
	destroyWindow("sketch_image");
}

void Stroke::show_stroke() {
	imshow("stroke_image", strokeImage);
	waitKey(0);
	destroyWindow("stroke_image");
}

/*
 * Implementation notes: gammaCorrection methods
 * -----------------------------------
 * This functions use a gamma algorithm to improve finding 
 * sketch of image.
 */

void Stroke::gammaCorrection(Mat& src, Mat& dst, float fGamma) {
	// create look up table
	unsigned char lut[256];
	for (int i = 0; i < 256; i++) {
		// gamma process formula
		lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
	}
	dst = src.clone();
	const int channels = dst.channels();
	// copy the pixel after processing to dst
	switch (channels) {
		case 1:
		{
			MatIterator_<uchar> it, end;
			for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++) {
				*it = lut[(*it)];
			}
			break;
		}
		case 3:
		{
			MatIterator_<Vec3b> it, end;
			for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++) {
				(*it)[0] = lut[((*it)[0])];
				(*it)[1] = lut[((*it)[1])];
				(*it)[2] = lut[((*it)[2])];
			}
			break;
		}
	}
}

/*
 * Implementation notes: sobel methods
 * -----------------------------------
 * This functions return a Mat ouput of sourcre
 * data after using sobel parameter.
 */

Mat Stroke::sobel() {
    Mat grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);// convert color to gray
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y, sobel;
	// process sobel operator in two direction x and y
	Sobel(grayImage, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	Sobel(grayImage, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel);
	return sobel;
}

/*
 * Implementation notes: motionBlur methods
 * -----------------------------------
 * This functions return a Mat ouput of sourcre
 * data after convolutes from four direction.
 */

Mat Stroke::motionBlur(Mat srcImg, int filterSize, bool max) {
	int size = filterSize;
	// create four direction filters which prepare for convolution
	Mat filter1 = Mat::zeros(size, size, CV_8UC1);
	Mat filter2 = Mat::zeros(size, size, CV_8UC1);
	Mat filter3 = Mat::zeros(size, size, CV_8UC1);
	Mat filter4 = Mat::zeros(size, size, CV_8UC1);
	Mat result = srcImg.clone();
	for (int i = 0; i < size; i++) {
		filter1.at<uchar>((size - 1) / 2, i) = (uchar)1;
	}
	for (int i = 0; i < size; i++) {
		filter2.at<uchar>(i, (size - 1) / 2) = (uchar)1;
	}
	for (int i = 0; i < size; i++) {
		filter3.at<uchar>(i, i) = (uchar)1;
	}
	for (int i = 0; i < size; i++) {
		filter4.at<uchar>(size - i - 1, i) = (uchar)1;
	}
	Mat filters[4] = {filter1, filter2, filter3, filter4};

	int len = size / 2;
	const int channels = srcImg.channels();
	switch (channels) {
		case 1: // for gray image
		{
			for (int r = 0; r < srcImg.rows; r++) {
				for (int c = 0; c < srcImg.cols; c++) {
					// for pixel (r, c) in srcImg
					int max_gray = 0;
					// convolution to 4 directions
					for (int k = 0; k < 4; k++) {
						int gray = 0;
						// calculate the sum of convolution in one direction
						for (int i = r - len; i <= r + len; i++) {
							for (int j = c - len; j <= c + len; j++) {
								if (i < 0 || j < 0 || i >= srcImg.rows || j >= srcImg.cols) continue;
								gray += ((int)srcImg.at<uchar>(i, j)) * ((int)filters[k].at<uchar>(i - (r - len), j - (c - len)));
							}
						}
						// find the max sum of gray
						if (gray > max_gray) {
							max_gray = gray;
						}
						// don't need to calculate maximum of sum of gray
						if (!max) {
							result.at<uchar>(r, c) = 255 - (uchar)(gray / size);
						}
					}
					// need to calculate maximum of sum of gray
					if (max) {
						result.at<uchar>(r, c) = 255 - (uchar)(max_gray / size);
					}
				}
			}
			break;
		}
		case 3: // for color image
		{
			for (int r = 0; r < srcImg.rows; r++) {
				for (int c = 0; c < srcImg.cols; c++) {
					//mask
					int red = 0, green = 0, blue = 0;
					for (int i = r - len; i <= r + len; i++) {
						for (int j = c - len; j <= c + len; j++) {
							if (i < 0 || j < 0 || i >= srcImg.rows || j >= srcImg.cols) continue;
							blue += ((int)srcImg.at<Vec3b>(i, j)[0]) * ((int)filter1.at<uchar>(i - (r - len), j - (c - len)));
							green += ((int)srcImg.at<Vec3b>(i, j)[1]) * ((int)filter1.at<uchar>(i - (r - len), j - (c - len)));
							red += ((int)srcImg.at<Vec3b>(i, j)[2]) * ((int)filter1.at<uchar>(i - (r - len), j - (c - len)));
						}
					}
					result.at<Vec3b>(r, c)[0] = (uchar)(blue / size);
					result.at<Vec3b>(r, c)[1] = (uchar)(green / size);
					result.at<Vec3b>(r, c)[2] = (uchar)(red / size);
				}
			}
			break;
		}
	}
	return result;
}

/*
 * Implementation notes: reverse methods
 * -----------------------------------
 * This functions reverse the color of grayimage.
 */

Mat Stroke::reverse(Mat srcImg) {
	for (int r = 0; r < srcImg.rows; r++) {
		for (int c = 0; c < srcImg.cols; c++) {
			srcImg.at<uchar>(r, c) = 255 - srcImg.at<uchar>(r, c); // reverse color formula
		}
	}
	return srcImg;
}

/*
 * Implementation notes: convolution methods
 * -----------------------------------
 * This functions convolues source image and use a filter
 * to improve the effect.
 */

Mat Stroke::convolution(Mat srcImage) {
	Mat stroke;
	// convolution for twice. One time need to calculate maximum and the other don't
    Mat motion_blur = motionBlur(srcImage, 11, true);
	Mat motion_blur_add = motionBlur(motion_blur, 11, false);
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 4.2, -1, 0, -1, 0); // kernel for fileter
	filter2D(motion_blur_add, stroke, motion_blur.depth(), kernel);
	return stroke;
}
