/*
 * File: effect.h
 * --------------
 * This file defines the prototype for different style functions.
 */

#ifndef EFFECT_H
#define EFFECT_H
#include <iostream>
#include "tone.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include "math.h"
using namespace std;
using namespace cv;

//The following 6 functions are written by Zeng Yuchen.
/*
 * Function: P1_standard
 * Usage: queue Result = P1_standard(Show_queue);
 * ------------------------
 * This is to use the standard method including the sobel stroke and BGR tone mapping without first normalizing the graph.
 * It is relatively slow because it needs to go through 3 color channels during tone mapping
 * and combine the three color tone map according to the color they weigh in the original graph will receive good effect.
 * The combinations of tone mapping and stroke have already been adjusted to a relatively good effect.
 * It is the one we recommend you to use.
 */
queue<QImage> P1_standard(queue<QImage> &Show_queue);


/*
 * Function: P1_quick
 * Usage: queue Result = P1_quick(Show_queue);
 * ------------------------
 * This is to use the standard method including the sobel stroke and Black and White tone mapping without first normalizing the graph.
 * It is realatively fast because when mapping the graph is handled only in one color channels.
 * The combinations of tone mapping and stroke have already been adjusted to a relatively good effect.
 * This is for the graph that is very large and can give you feedback in a short time.
 */
queue<QImage> P1_quick(queue<QImage> &Show_queue);


/*
 * Function: P1_advance
 * Usage: queue Result = P1_advance(Show_queue);
 * ------------------------
 * This is to use the standard method including the sobel stroke and BGR tone mapping with first normalizing the graph.
 * It is the slowest because it needs first to normalize the histogram of the original graph then to go through 3 color channels during tone mapping.
 * Because the histogram has already been normalized, the combination according to the weight is not idealy good.
 * Therefore, I combine the three color tone map according to the color dominance of the original graph.
 * The combinations of tone mapping and stroke have already been adjusted to a relatively good effect.
 * You want to use this if your original graph is too bright or too dark.
 */
queue<QImage> P1_advance(queue<QImage> &Show_queue);


/*
 * Function: P2_standard
 * Usage: queue Result = P2_standard(Show_queue);
 * ------------------------
 * This is to use the standard method including the Gamma sketch and BGR tone mapping without first normalizing the graph.
 * It is relatively slow because it needs to go through 3 color channels during tone mapping
 * and combine the three color tone map according to the color they weigh in the original graph will receive good effect.
 * The combinations of tone mapping and sketch have already been adjusted to a relatively good effect.
 * It is the one we recommend you to use.
 */
queue<QImage> P2_standard(queue<QImage> &Show_queue);


/*
 * Function: P2_quick
 * Usage: queue Result = P2_quick(Show_queue);
 * ------------------------
 * This is to use the standard method including the Gamma sketch and Black and White tone mapping without first normalizing the graph.
 * It is realatively fast because when mapping the graph is handled only in one color channels.
 * The combinations of tone mapping and sketch have already been adjusted to a relatively good effect.
 * This is for the graph that is very large and can give you feedback in a short time.
 */
queue<QImage> P2_quick(queue<QImage> &Show_queue);


/*
 * Function: P2_advance
 * Usage: queue Result = P2_advance(Show_queue);
 * ------------------------
 * This is to use the standard method including the Gamma sketch and BGR tone mapping with first normalizing the graph.
 * It is the slowest because it needs first to normalize the histogram of the original graph then to go through 3 color channels during tone mapping.
 * Because the histogram has already been normalized, the combination according to the weight is not idealy good.
 * Therefore, I combine the three color tone map according to the color dominance of the original graph.
 * The combinations of tone mapping and sketch have already been adjusted to a relatively good effect.
 * You want to use this if your original graph is too bright or too dark.
 */
queue<QImage> P2_advance(queue<QImage> &Show_queue);


//The following 4 functions are written by Zhang Hongbo.
/*
 * Implementation notes: P3_1
 * -----------------------------------
 * This function get QImage from parameter and transfer it to Mat.
 * Process the mat image and return QImage back. Push it by call
 * by reference.
 */
void P3_1(queue<QImage> & Show_queue);

/*
 * Implementation notes: P3_2
 * -----------------------------------
 * This function get QImage from parameter and transfer it to Mat.
 * Process the mat image and return QImage back. Push it by call
 * by reference.
 */
void P3_2(queue<QImage> & Show_queue);

/*
 * Implementation notes: P3_3
 * -----------------------------------
 * This function get QImage from parameter and transfer it to Mat.
 * Process the mat image and return QImage back. Push it by call
 * by reference.
 */
void P3_3(queue<QImage> & Show_queue);

/*
 * Implementation notes: P4
 * -----------------------------------
 * This function get QImage from parameter and transfer it to Mat.
 * Process the mat image and return QImage back. Push it by call
 * by reference.
 */
void P4(queue<QImage> & Show_queue);



#endif // EFFECT_H
