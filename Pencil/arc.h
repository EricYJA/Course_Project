/*
 * File: arc.h
 * --------------
 * This file defines the interface for the Arc class.
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "training.h"
#include <iostream>
using namespace cv;
using namespace std;

/*
 * Class: Arc
 * -------------------
 * This class transfer an source picture into different arc effects
 * in different ways. The effects will use canny operator and
 * laplacian operator. The effects have white color style and 
 * random color style.
 */

class Arc: public Pic {

/* Public section */

public:

/*
 * Constructor: Arc
 * Usage: Arc arcExample;
 * ---------------------------
 * Creates an empty Arc object.
 */

  Arc();

/*
 * Constructor: Arc
 * Usage: Arc arcExample(qImageSrc);
 * ---------------------------
 * Creates an Arc object with QImage pointer parameter.
 */

  Arc(QImage * qImageSrc);

/*
 * Constructor: Arc
 * Usage: Arc arcExample(matSrc);
 * ---------------------------
 * Creates an Arc object with Mat parameter.
 */

  Arc(Mat matSrc);

/*
 * Methods: set_laplacian_white, set_canny_color, set_canny_white
 * Usage: arcExample.set_laplacian_white();
 *        arcExample.set_canny_color();
 *        arcExample.set_canny_white();
 * -----------------------------------
 * These functions set the result images into private image respectively.
 * It should be initialized at first to make sure the later functions 
 * can be used.
 */

  void set_laplacian_white();
  void set_canny_color();
  void set_canny_white();

/*
 * Methods: show_laplacian_white, show_canny_color, show_canny_white
 * Usage: arcExample.show_laplacian_white();
 *        arcExample.show_canny_color();
 *        arcExample.show_canny_white();
 * -----------------------------------
 * These functions show the result images respectively.It should be
 * used after initializing functions, which are set_laplacian_white(),
 * set_canny_color() and set_canny_white() respectively.
 */

  void show_laplacian_white();
  void show_canny_color();
  void show_canny_white();

/*
 * Methods: get_laplacian_white, get_canny_color, get_canny_white
 * Usage: QImage lw = arcExample.get_laplacian_white();
 *        QImage cc = arcExample.get_canny_color();
 *        QImage cw = arcExample.get_canny_white();
 * -----------------------------------
 * These functions return the result images in the type of QImage respectively.
 * It should be used after initializing functions, which are set_laplacian_white(),
 * set_canny_color and set_canny_white respectively.
 */

  QImage get_laplacian_white();
  QImage get_canny_color();
  QImage get_canny_white();

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

/* Instance variables */

  Mat srcImage;                      /* Mat data from input                                 */
  Mat grayImage;                     /* Mat data that are transfered to gray                */
  Mat laplacianImage;                /* Result using Laplacian operator with white style    */
  Mat cannyColorImage;               /* Result using Canny operator with random color style */
  Mat cannyWhiteImage;               /* Result using Canny operator with random white style */
  vector<vector<Point> > contour;    /* Contour data of findContour function                */
  vector<Vec4i> hierarchy;           /* hierarchy data of findContour function              */
  RNG rng;                           /* RNG data                                            */

};