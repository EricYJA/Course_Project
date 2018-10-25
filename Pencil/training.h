/*
 * File: training.h
 * --------------
 * This file defines the interface for the Pic class.
 */

#ifndef TRAINING_H
#define TRAINING_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "qimage.h"

using namespace cv;
using namespace std;

/* Class: Pic
 * ------------------------------------------
 * this is a class that can read the Mat file,
 * Image * file and the route of a picture and
 * convert them into a Mat format to do some
 * basic operation using OpenCV
 */

class Pic {
public:

/* Function: Pic()
 * Usage: Pic a = Pic()
 * ------------------------------------------
 * construct a Pic object
 */

    Pic();

/* Function: Pic(QImage * image)
 * Usage Pic a = Pic(image)
 * ------------------------------------------
 * construct a Pic object with a Qimage * input
 */

    Pic(QImage *image);

/* Function: Pic(Mat src)
 * Usage: Pic a = Pic(src)
 * ------------------------------------------
 * construct a Pic object with a Mat input
 */

    Pic(Mat src);

/* Function: Pic(string route)
 * Usage: Pic a = Pic(route)
 * ------------------------------------------
 * construct a Pic object with a route of
 * a picture input
 */

    Pic(string route);

/* Function: getFlag()
 * Usage  pic.getflag()
 * ------------------------------------------
 * return the bool value to determain if the
 * the picture is confirmed if it returns true
 * then it's confirmed
 */

    bool getFlag();

/* Function: getdst()
 * Usage: pic.getdst()
 * ------------------------------------------
 * return the dstImage stored in the class
 */

    Mat getdst();

/* Function: getsrc()
 * Usage: pic.getsrc()
 * ------------------------------------------
 * return the srcImage srored in the class
 */

    Mat getsrc();

/* Function: gettmp()
 * Usage: pic.gettmp()
 * ------------------------------------------
 * return the srcImage srored in the class
 */

    Mat gettmp();

/* Function: settmp(Mat tmp)
 * Usage: pic.settmp(tmp)
 * ------------------------------------------
 * set the instance variable tmpImage
 */

    void settmp(Mat tmp);

/* Function: showsrc()
 * Usage: pic.showsrc()
 * ------------------------------------------
 * show the srcImage using OpenCV package
 */

    void showsrc();

/* Function: showtmp()
 * Usage: pic.showtmp()
 * ------------------------------------------
 * show the tmpImage using OpenCV package
 */

    void showtmp();

/* Function: showdst()
 * Usage: pic.showdst()
 * ------------------------------------------
 * show the dstImage using OpenCV package
 */

    void showdst();

/* Function: standardSize()
 * Usage: pic.standardSize()
 * ------------------------------------------
 * show the dstImage using OpenCV package
 */

    void standardSize();

/* Function: confirmPic()
 * Usage: pic.confirmPic()
 * ------------------------------------------
 * set the tmpImage into dstImage
 */

    void confirmPic();

/* Function: getQImage()
 * Usage: QImage b = pic.getQImage()
 * ------------------------------------------
 * set the tmpImage into dstImage
 */

    QImage getQImage();




private:

    Mat srcImage;// the source Image
    Mat tmpImage;// the temporary Image
    Mat dstImage;// the destination Image

    bool flag;// the confirmed judging flag
};

/* Function: QImage2cvMat()
 * Usage: cv::Mat a = QImage2cvMat(image)
 * ------------------------------------------
 * convert a QImage to Mat format
 */

Mat QImage2cvMat(QImage image);
#endif // TRAINING_H
