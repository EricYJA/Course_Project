/*
 * File: cartoon.h
 * --------------
 * This file defines the interface for the Cartoon class.
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "training.h"
#include <iostream>
using namespace cv;
using namespace std;

/*
 * Class: Cartoon
 * -------------------
 * This class transfer an source picture into cartoon effects.
 * The effects will use laplacian operator and bilateralFilter.
 * The effects will be in cartoon and colorful style.
 */


class Cartoon: public Pic {

/* Public section */

public:

/*
 * Constructor: Cartoon
 * Usage: Cartoon cartoonExample;
 * ---------------------------
 * Creates an empty arc object.
 */

  Cartoon();

/*
 * Constructor: Cartoon
 * Usage: Cartoon cartoonExample(qImageSrc);
 * ---------------------------
 * Creates an Cartoon object with QImage pointer parameter.
 */

  Cartoon(QImage * qImageSrc);

/*
 * Constructor: Cartoon
 * Usage: Cartoon cartoonExample(matSrc);
 * ---------------------------
 * Creates an Cartoon object with Mat parameter.
 */

  Cartoon(Mat matSrc);

/*
 * Methods: reverse
 * Usage: reverse(inputMat);
 * -----------------------------------
 * This function reverse the color of gray image. Black pixel will 
 * be transferred into white pixel.
 */

  Mat reverse(Mat inputImage);

/*
 * Methods: set_cartoon
 * Usage: cartoonExample.set_cartoon();
 * -----------------------------------
 * This function set the result image into private image.It should
 * be used after initializing at first place.
 */

  void set_cartoon();

/*
 * Methods: show_cartoon
 * Usage: cartoonExample.show_cartoon();
 * -----------------------------------
 * This function show the result image.It should be used after 
 * initializing functions, which is set_cartoon().
 */

  void show_cartoon();

/*
 * Methods: get_cartoon
 * Usage: QImage cartoon = cartoonExample.get_cartoon();
 * -----------------------------------
 * This function return the result image in the type of QImage.It 
 * should be used after initializing functions, which is set_cartoon().
 */

  QImage get_cartoon();

/* Private section */

private:

/*
 * Implementation notes: Cartoon data structure
 * -------------------------------------------
 * In the opencv-based implementation of the Cartoon class, the results are
 * stored in private variables.  In addition to the results, the grayImage is also
 * initialized in the first place so that it don't need to transfer so many
 * times.
 */

/* Instance variables */

  Mat srcImage;         /* Mat data from input                                 */
  Mat grayImage;        /* Mat data that are transfered to gray                */
  Mat cartoonImage;     /* Mat data afetr cartoon operation                    */

};