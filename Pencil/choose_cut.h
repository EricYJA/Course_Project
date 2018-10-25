/*
 * File: choose_cut.h
 * --------------
 * This file defines the interface for the Choose_cut class.
 */

#ifndef CHOOSE_CUT_H
#define CHOOSE_CUT_H
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <stack>
#include <QImage>
#include <cmath>
#include "training.h"

#define WINDOW_NAME_1 "Choose & Cut" // predfiend the WINDOW_NAME_1
#define WINDOW_NAME "Change_Size" // predefine the WINDOW_NAME
#define WINDOW_NAME_2 "Rotation" // prediefine the WINDOW_NAME_2
#define pi 3.1415926535898 // predefine the pi to 3.1415926535898
using namespace cv;
using namespace std;

/* Class: Choose_cut
 * -----------------------------
 * this class is used to do the
 * editing method to the image
 */

class Choose_Cut: public Pic
{
public:

/* Function: Choose_Cut()
 * Usage: Choose_Cut a = Choose_Cut()
 * ------------------------------------------
 * construct a Choose_Cut object
 */

    Choose_Cut();

/* Function: Choose_Cut(QImage * image)
 * Usage: Choose_Cut a = Choose_Cut(image)
 * ------------------------------------------
 * construct a Choose_Cut object with QImage
 * input
 */

    Choose_Cut(QImage * image, double min, double max);

/* Function: Choose_Cut(string route)
 * Usage: Choose_Cut a = Choose_Cut(route)
 * ------------------------------------------
 * construct a Choose_Cut object with a route
 * of a picture input
 */

    Choose_Cut(string route, double min, double max);

/* Function: Choose_Cut(string route)
 * Usage: Choose_Cut a = Choose_Cut(route)
 * ------------------------------------------
 * construct a Choose_Cut object with a Mat
 * input
 */

    Choose_Cut(Mat src, double min, double max);

/*Function: operating
 * ------------------------------------------
 * this function is only used for testing
 * the three method thus it's commented
 */
//    void operating();


/* Function: getcut(double ratio)
 * Usage: a.getcut(ratio)
 * ------------------------------------------
 * cut the tmpImage and set and return the tmpImage
 * with the cropped Image
 */

    Mat getCut(double ratio);

/* Function: change(double ratio)
 * Usage: a.change(ratio)
 * ------------------------------------------
 * change the size of the tmpImage and set and
 * return the tmpImage with the changed Image
 */

    Mat change(double ratio);

/* Function: rotate(double ratio)
 * Usage: a.rotate(ratio)
 * ------------------------------------------
 * rotate the tmpImage and set and return the
 * tmpImage with the rotated Image
 */

    Mat rotate(double ratio);


/* Function: getBigFlag(), getTinyFlag()
 * Usage: a.getBigFlag(ratio), a.getTinyFlag(ratio)
 * ------------------------------------------
 * return true if the picture is too big or
 * too small for the window
 */

    bool getBigFlag();
    bool getTinyFlag();

private:

    //store the bool value to judge if the picture is too big or too small;
    bool bigFlag = false;
    bool tinyFlag = false;

    Mat srcImage;// store the srcImage
    Mat tmpImage;// store the tmpImage
    Mat dstImage;// store the dstImage

    double minSize;
    double maxSize;
//    stack<Mat> recordBegin; // the stack for undoing in operating
//    stack<Mat> recordEnd; // the stack for redoing in operating

    int colsize, rowsize;// record the colsize and the rowsize of the srcImage
};


/* Function: on_mouse(double ratio)
 * Usage: on_mouse
 * ------------------------------------------
 * record and return the mouse operating
 * information to the function
 */

void on_mouse(int mouseEvent, int x, int y, int, void*);


#endif // CHOOSE_CUT_H
