/*
 * File: stroke.h
 * --------------
 * This file defines the interface for the Stroke class.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>    
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include "training.h"

using namespace cv;
using namespace std;

/*
 * Class: Stroke
 * -------------------
 * This class transfer an source picture into stroke effect and 
 * sketch effect. The effects will use sobel operator and convolution
 * The effects are all in gray style.
 */

class Stroke: public Pic {

/* Public section */

public:

/*
 * Constructor: Stroke
 * Usage: Stroke strokeExample;
 * ---------------------------
 * Creates an empty Stroke object.
 */

  Stroke();

/*
 * Constructor: Stroke
 * Usage: Stroke strokeExample(qImageSrc);
 * ---------------------------
 * Creates an empty Stroke object with QImage pointer parameter.
 */

  Stroke(QImage * image);

/*
 * Constructor: Stroke
 * Usage: Stroke strokeExample(matSrc);
 * ---------------------------
 * Creates an empty Stroke object with Mat parameter.
 */

  Stroke(Mat matSrc);

/*
 * Methods: set_sketch, set_stroke
 * Usage: strokeExample.set_sketch();
 *        strokeExample.set_stroke();
 * -----------------------------------
 * These functions set the result images into private image respectively.
 * It should be initialized at first to make sure the later functions 
 * can be used.
 */

  void set_sketch();
  void set_stroke();

/*
 * Methods: show_stroke, show_sketch
 * Usage: strokeExample.show_stroke();
 *        strokeExample.show_sketch();
 * -----------------------------------
 * These functions show the result images respectively.It should be
 * used after initializing functions, which are set_sketch(),
 * set_stroke() respectively.
 */

  void show_stroke();
  void show_sketch();

/*
 * Methods: get_sketch, get_stroke
 * Usage: QImage sketch = strokeExample.get_sketch();
 *        QImage stroke = strokeExample.get_stroke();
 * -----------------------------------
 * These functions return the result images in the type of QImage
 * respectively. It should be used after initializing functions,
 * which are set_sketch() and set_stroke() respectively.
 */

  QImage get_sketch();
  QImage get_stroke();

/*
 * Methods: get_matSketch, get_matStroke
 * Usage: Mat matSketch = strokeExample.get_matSketch();
 *        Mat matStroke = strokeExample.get_matStroke();
 * -----------------------------------
 * These functions return the result images in the type of Mat
 * respectively. It should be used after initializing functions,
 * which are set_sketch() and set_stroke() respectively.
 */

  Mat get_matSketch();
  Mat get_matStroke();

/*
 * Methods: gammaCorrection
 * Usage: gammaCorrection(inputMat, outputMat, gammaParameter);
 * -----------------------------------
 * This function use a gamma algorithm to improve finding 
 * sketch of image.
 */

  void gammaCorrection(Mat& src, Mat& dst, float fGamma);

/*
 * Methods: sobel
 * Usage: sobel();
 * -----------------------------------
 * This function return the Mat data of srcImage after using 
 * sobel parameter.
 */

  Mat sobel();

/*
 * Methods: motionBlur
 * Usage: motionBlur(inputMat, outputMat, max);
 * -----------------------------------
 * This function first construct four filters represent four
 * directions. Then it will convolute in four direction using
 * source image. There are two type of motion blur. One of them 
 * will need to find out the maximum while another one don't 
 * need to.
 */

  Mat motionBlur(Mat srcImg, int filterSize, bool max);

/*
 * Methods: reverse
 * Usage: reverse(inputMat);
 * -----------------------------------
 * This function reverse the color of gray image. Black pixel will 
 * be transferred into white pixel.
 */

  Mat reverse(Mat srcImg);

/*
 * Methods: convolution
 * Usage: convolution(inputMat);
 * -----------------------------------
 * This function will calculate the convolution of input Mat data.
 * and use a filter to improve the effect.
 */

  Mat convolution(Mat srcImage);

/* Private section */

private:

/*
 * Implementation notes: Arc data structure
 * -------------------------------------------
 * In the opencv-based implementation of the Arc class, the results are stored 
 * in private variables.  In addition to the results, the grayImage is also
 * initialized in the first place so that it don't need to transfer so many
 * times.
 */

  Mat srcImage;         /* Mat data from input                  */
  Mat grayImage;        /* Mat data that are transfered to gray */
  Mat sketchImage;      /* Result of sketch                     */
  Mat strokeImage;      /* Result of stroke                     */
};
