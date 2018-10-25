/*
 * File: tone.h
 * -----------------------
 * This header file contains prototypes of the functions to generate the tone map for the main effect of our App.
 * There are three ways to generate the tone map which will be explained in tone.cpp file.
 */
#ifndef PENCILDRAWING_H
#define PENCILDRAWING_H
#define OUT_PARA
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include "math.h"
#include <vector>
#include <time.h>
#include <QImage>
#include "training.h"
#include "stroke.h"
#include <queue>
#include "qstring.h"

using namespace std;
using namespace cv;

/*
 * Class: PencilDrawing_Tone
 * -----------------------
 * Our project uses object orientation structure. Therefore, all of our functions are enclosed in their own class
 * for the convinience to pack them into the GUI and the main function.
 * This class aims to realize the generation of the tone map (layer) of the pencil drawing.
 */
class PencilDrawing_Tone: public Pic
{
/* Public section */
public:
    /*
     * Constructor: PencilDrawing_Tone
     * Usage: PencilDrawing_Tone pencil;
     * ------------------------
     * This is to construct a PencilDrawing_Tone object.
     */
    PencilDrawing_Tone();

    /*
     * Constructor: PencilDrawing_Tone(QImage * image)
     * Usage: pencil = PencilDrawing_Tone(image);
     * ------------------------
     * This mainly aims to read the picture file if it is the type of QImage.
     */
    PencilDrawing_Tone(QImage * image);

    /*
     * Constructor: PencilDrawing_Tone(Mat SourceImage)
     * Usage: pencil = PencilDrawing_Tone(SourceImage);
     * ------------------------
     * This mainly aims to read the picture file if it is the type of Mat.
     */
    PencilDrawing_Tone(Mat SourceImage);

    /*
     * Method: ComputeDistribution()
     * Usage: pencil.ComputeDistribution();
     * ------------------------
     * This method aims to compute(calculate) the distribution using the formula given
     * in the paper we mentioned in the proposal. This distribution is the histogram summarized from
     * the real pencildrawing picture. Therefore, if we use it to normalize our target picture,
     * we can get the bright layer, mid-tone and dark layer we want.
     */
    void ComputeDistribution();

    /*
     * Method: calculatehistogram(Mat &sourcePic);
     * Usage: Mat Histogram = pencil.calculatehistogram(sourcePic);
     * ------------------------
     * This method aims to calculate and return the histogram of the sourcePic in the format of Mat.
     */
    Mat calculatehistogram(Mat &sourcePic);

    /*
     * Method: generate_tone_map(Mat &sourcePic, Mat & ToneMap)
     * Usage: pencil.generate_tone_map(sourcePic, ToneMap);
     * ------------------------
     * This method aims to use the input Mat graph file sourcePic to generate the corresponding tone map in different ways.
     */
    void generate_tone_map(Mat &sourcePic, Mat & ToneMap);

    /*
     * Method: histogramMatching(double value)
     * Usage: uchar ToneMapData[i] = histogramMatching(value);
     * ------------------------
     * This method aims to use the distribution calculated before to generate a tone map of the source picture.
     * For the histogram has 0 - 255 color levels. Each level records the fraction of pixels that has that value in a picture.
     * For example, a picture has 1000 "255" color value, then the histogram at "255" will be 1000.
     * This method goes through every pixel of the source picture to see the color value it has. The source picture has it own histogram.
     * Then a blank tone map will be created and goes through every pixel to match the tone.
     * The way to match it is that the method will check the corresponding fraction of that value the pixel of the source picture has.
     * Then it will find the most similar fraction of the value in the distribution. Then give that pixel the corresponding value found in the distribution.
     * For example, for the first pixel in the blank tone map. The methods will check the first pixel of the source picture. This pixel has a certain color value
     * ranging from 0 to 255. Then the amount of that value the whole picture has will be known. For example, the first pixel of the source picture has the value of "200".
     * And in the histogram of the source picture, the fraction of pixels that has the value of "200" is 66.
     * Then the method will go to the distribution to see which color value has the closest fraction to 66.
     * For example, the color value 240 has the fraction 66 that is the closest fraction to 66.
     * Then assign the color value 240 to the first pixel of the blank tone map.
     * Following this way, the tone map can be generated.
     */
    uchar histogramMatching(double value);

    /*
     * Method: GenerateTheBestTone(Mat & sourcePic_RGB);
     * Usage: Mat ToneMap = pencil.GenerateTheBestTone(sourcePic_RGB);
     * ------------------------
     * This is for the optimization of Tone Map Generation.
     * This method is for the condition that the input image is chosen to be handle in the three channels way (BGR).
     * This method aims to generate the best tone among the three channels that are Blue, Green and Red.
     * There are two methods:
     * 1. Seperate the three-channel source picture to 3 channels (Blue, Green, Red).
     * By comparing the color they have in the pixel checked according to the iteration, I sum them up to see which color is the main color of the picture.
     * For example, a pixel has the BGR of (123,22, 250), then the dominant one is Red, then the sum of red will add 1.
     * By searching the pixel according to the iteration, the sum of blue, green and red will be generated.
     * Then according to the fraction they dominant, I add the three layer up to generate the best tone.
     * 2. The histogram of the picture will first be equalization (basically, make the darker part easier to see).
     * This will make the histogram distribute more uniformly.
     * Then after the testing, I find that, all the step are the same as the step 1, excluding the last adding part.
     * I find that, If one color is dominent I need to make it 70% of the whole combination to acheive a relatively great effect.
     */
    Mat GenerateTheBestTone(Mat & sourcePic_RGB);

    /*
     * Method: Find_order(double & sum_Blue, double & sum_Red, double & sum_Green, double & Largest, double & Mid, double & Smallest);
     * Usage: pencil.Find_order(sum_Blue, sum_Red, sum_Green, Largest, Mid, Smallest);
     * ------------------------
     * This method aims to find the order among three values and put the value into the double type value: Largest, Mid and Smallest.
     */
    void Find_order(double & sum_Blue, double & sum_Red, double & sum_Green, double & Largest, double & Mid, double & Smallest);

    /*
     * Method: SortList(double & sum_Blue, double & sum_Red, double & sum_Green);
     * Usage: vector<double> sort = SortList(sum_Blue, sum_Red, sum_Green);
     * ------------------------
     * This method aims to return a sorted list of three values.
     */
    vector<double> SortList(double & sum_Blue, double & sum_Red, double & sum_Green);

    /*
     * Method: Weight_calculation(double & sum_Blue, double & sum_Green, double & sum_Red, double & Weight_Blue, double & Weight_Green, double & Weight_Red, double & Weight_B1, double & Weight_G1, double & Weight_BG);
     * Usage: pencil.Weight_calculation(sum_Blue, sum_Green, sum_Red, Weight_Blue, Weight_Green, Weight_Red, Weight_B1, Weight_G1, Weight_BG);
     * ------------------------
     * This method aims to return a sorted list of three values.
     */
    void Weight_calculation(double & sum_Blue, double & sum_Green, double & sum_Red,
                            double & Weight_Blue, double & Weight_Green, double & Weight_Red,
                            double & Weight_B1, double & Weight_G1, double & Weight_BG);

    /*
     * Method: turnpictureintowhite(Mat & SourcePic);
     * Usage: Mat sourcePic_BW = turnpictureintowhite(SourcePic);
     * ------------------------
     * This method aims to turn the input SourcePic to black and white picture which has only one channel.
     */
    Mat turnpictureintowhite(Mat & SourcePic);

    /*
     * Method: is_RGB(Mat SourcePic);
     * Usage: pencil.is_RGB(SourcePic);
     * ------------------------
     * This method aims to judge whether the input picture SourcePic is a RGB (3 channels) picture.
     * Return true if the picture is a RGB picture. Return false if the picture is a grey picture.
     */
    bool is_RGB(Mat SourcePic);

    /*
     * Method: BlackWhiteTone(Mat & sourcePic_BW);
     * Usage: Mat ToneMap = BlackWhiteTone(sourcePic_BW);
     * ------------------------
     * This method is only for the input picture SourcePic_BW that is grey picture.
     * This method is faster than GenerateTheBestTone(Mat & sourcePic_RGB) because it only needs to handle one channel.
     */
    Mat BlackWhiteTone(Mat & sourcePic_BW);

    /*
     * Method: Get_iteration(IplImage * & image_temp);
     * Usage: int iteration = Get_iteration(image_temp);
     * ------------------------
     * This method is to optimize the run time of generate the best tone map.
     * Judging by the resolution of the picture, the method return different iteration value.
     */
    int Get_iteration(IplImage * & img);

    /*
     * Method: Equalization_Image(IplImage * & SourceImage);
     * Usage: sourcePic_RGB = Equalization_Image(SourceImage);
     * ------------------------
     * This method is to equalize the input image to make its histogram distribute more uniformly.
     */
    Mat Equalization_Image(IplImage * & SourceImage);

    /*
     * Method: Start_function();
     * Usage: Mat ToneMap = pencil.Start_function();
     * ------------------------
     * This method is to integrate my methods in order to more simply generate a Tone Map.
     */
    Mat Start_function();

    /*
     * Method: get_height(Mat SourcePic);
     * Usage: int height = get_height(SourcePic);
     * ------------------------
     * This method is to get the height of the input picture SourcePic.
     */
    int get_height(Mat SourcePic);

    /*
     * Method: get_width(Mat SourcePic);
     * Usage: int height = get_width(SourcePic);
     * ------------------------
     * This method is to get the width of the input picture SourcePic.
     */
    int get_width(Mat SourcePic);

    /*
     * Method: Add_Texture(Mat & SourcePic);
     * Usage: Mat ToneMap = Add_Texture(SourcePic);
     * ------------------------
     * This method is to add a layer of pencil texture to the input picture SourcePic.
     */
    void Add_Texture(Mat & SourcePic);

    /*
     * Method: change_Tone_method1();
     * Usage: pencil.change_Tone_method1();
     * ------------------------
     * Because there are two ways to handle the BGR picture. This is to make the private variable Tone_Method equal to 1.
     * When Tone_Method is 1, the function will use method 1 to generate the tone map.
     */
    void change_Tone_method1();

    /*
     * Method: change_Tone_method2();
     * Usage: pencil.change_Tone_method2();
     * ------------------------
     * This is to make the private variable Tone_Method equal to 2.
     * When Tone_Method is 2, the function will use method 2 to generate the tone map.
     */
    void change_Tone_method2();

    /*
     * Method: Get_BGR_Tone_Queue;
     * Usage: queue BGRTone = pencil.Get_BGR_Tone_Queue();
     * ------------------------
     * This is to get the BGR Tone.
     */
    queue<QImage> Get_BGR_Tone_Queue();

    /*
     * Method: Clear_BGR_Tone_Queue;
     * Usage: pencil.Clear_BGR_Tone_Queue();
     * ------------------------
     * This is to clear the queue BGR_Tone.
     */
    void Clear_BGR_Tone_Queue();

    /*
     * Method: Get_Gsource;
     * Usage: sourcePic = pencil.Get_Gsource();
     * ------------------------
     * This is to get G_SourceImage.
     */
    Mat Get_Gsource();

/* Private section */
private:
    /* Instance Variables */

    //This is the distribution that will be calculated according to the formula in the paper.
    vector<double> distribution;

    //These are the weight of the three layer (bright layer, mid-tone, dark layer).
    double weight1, weight2, weight3;

    //The following are the variable in the formula.
    double delta_b, delta_d;
    double u_a, u_b;
    double miu_d;

    //This is treated as the switch to turn to different method to generate the tone map.
    int Tone_Method;

    //This can be used as the source image.
    Mat G_SourceImage;

    //This is a queue that stores the BGR Tone.
    queue<QImage> BGR_Tone;

    //This is to record the Application running path to store temporary file for the processing using string.
    string path;

    //This is to record the Application running path to store temporary file for the processing for QT using.
    QString qpath;

};


#endif // PENCILDRAWING_H
