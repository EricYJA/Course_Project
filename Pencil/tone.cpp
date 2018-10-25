/*
 * File: tone.cpp
 * -----------------------
 * This source file contains the main implementation and explanation of the way to generate the tone map.
 */
#include <iostream>
#include "tone.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include "math.h"
#include <QCoreApplication>
#include <queue>
#include "qmessagebox.h"
#include "qapplication.h"
using namespace std;
using namespace cv;

/*
 * Constructor: PencilDrawing_Tone
 * Usage: PencilDrawing_Tone pencil;
 * ------------------------
 * This is to construct a PencilDrawing_Tone object.
 */
PencilDrawing_Tone::PencilDrawing_Tone() {
    weight1 = 52;
    weight2 = 37;
    weight3 = 11;
    delta_b = 9;
    delta_d = 11;
    u_a = 105;
    u_b = 225;
    miu_d = 90;
    //The default Tone_Method will be 1.
    Tone_Method = 1;
    qpath = QApplication::applicationDirPath();
    path = qpath.toStdString();
}

/*
 * Constructor: PencilDrawing_Tone(QImage * image)
 * Usage: pencil = PencilDrawing_Tone(image);
 * ------------------------
 * This mainly aims to read the picture file if it is the type of QImage.
 */
PencilDrawing_Tone::PencilDrawing_Tone(QImage * image):
    Pic(image) {
    weight1 = 52;
    weight2 = 37;
    weight3 = 11;
    delta_b = 9;
    delta_d = 11;
    u_a = 105;
    u_b = 225;
    miu_d = 90;
    //The default Tone_Method will be 1.
    Tone_Method = 1;
    G_SourceImage = getsrc();
    qpath = QApplication::applicationDirPath();
    path = qpath.toStdString();
}

/*
 * Constructor: PencilDrawing_Tone(Mat SourceImage)
 * Usage: pencil = PencilDrawing_Tone(SourceImage);
 * ------------------------
 * This mainly aims to read the picture file if it is the type of Mat.
 */
PencilDrawing_Tone::PencilDrawing_Tone(Mat SourceImage) {
    weight1 = 52;
    weight2 = 37;
    weight3 = 11;
    delta_b = 9;
    delta_d = 11;
    u_a = 105;
    u_b = 225;
    miu_d = 90;
    //The default Tone_Method will be 1.
    Tone_Method = 1;
    G_SourceImage = SourceImage;
    qpath = QApplication::applicationDirPath();
    path = qpath.toStdString();
}

/*
 * Method: ComputeDistribution()
 * Usage: pencil.ComputeDistribution();
 * ------------------------
 * This method aims to compute(calculate) the distribution using the formula given
 * in the paper we mentioned in the proposal. This distribution is the histogram summarized from
 * the real pencildrawing picture. Therefore, if we use it to normalize our target picture,
 * we can get the bright layer, mid-tone and dark layer we want.
 */
void PencilDrawing_Tone::ComputeDistribution() {

    //First, resize the distribution as 256 becuase 0 - 255, total 256.
    distribution.resize(256);

    //In order to avoid issues, use an integer to record the size of the distribution.
    int sizeofdistribution = distribution.size();

    //Initially, sum equals to 0.0.
    double sum = 0.0;

    //Use auto for p1, p2 and p3 means they are automatic variables.
    //Because they will change according to i, like the discrete function. Therefore, using auto is the best way.
    auto p1 = [this](const double & i)->double {
        if (i <= 255 ) {
            //Here I modify the formula in the paper a little bit because the effect is not so well. And the original formula may be wrong.
            return (1 / delta_b) * exp(-1 * (195 - i) /  delta_b);
        } else {
            return 0;
        }
    };

    auto p2 = [this](const double & i)->double {
        if (i >=  u_a && i <=  u_b) {
            return (1 / ( u_b -  u_a));
        } else {
            return 0;
        }
    };

    auto p3 = [this](const double & i)->double {
        if (i <= 255) {
            return (1 / (sqrt(2 * M_PI *  delta_d))) * exp(-((i -  miu_d) * (i -  miu_d)) / (2 * delta_d * delta_d));
        } else {
            return 0;
        }
    };

    //Use a full loop to calculate each distribution from 0 to 255 according to the weight of three function.
    //Because the weight is in %, therefore, it needs to be divided by 100.
    for(int i = 0; i < sizeofdistribution; i++) {
        distribution[i] = (weight1 * p1(i) + weight2 * p2(i) + weight3 * p3(i)) / 100;

        //sum is to sum the whole distribution together for an important use.
        sum = sum + distribution[i];
    }

    //This is the critical part, becuase each picture has the different number of pixels,
    //therefore, they can not be directly compare. They can only be compared by the fraction.
    for(int i = 0; i < sizeofdistribution; i++) {
        distribution[i] = distribution[i] / sum;
    }
}

/*
 * Method: calculatehistogram(Mat &sourcePic);
 * Usage: Mat Histogram = pencil.calculatehistogram(sourcePic);
 * ------------------------
 * This method aims to calculate and return the histogram of the sourcePic in the format of Mat.
 */
Mat PencilDrawing_Tone::calculatehistogram(Mat &sourcePic) {
    //The following are the basic parameters needed for the function calcHist.
    int numberofimages = 1;
    int channels[1] = {0};
    Mat outputHistogram;
    int dimension = 1;
    int SizeofHistogram[1] = {256};
    float histogramranges[2] = {0, 255};
    const float* ranges[1] = {histogramranges};
    bool uniform = true;
    bool accum = false;

    //calcHist is the function from OpenCV.
    calcHist(&sourcePic, numberofimages, channels, Mat(), outputHistogram, dimension, SizeofHistogram, ranges, uniform, accum);

    //Return the Histogram in the format of Mat.
    return outputHistogram;
}

/*
 * Method: generate_tone_map(Mat &sourcePic, Mat & ToneMap)
 * Usage: pencil.generate_tone_map(sourcePic, ToneMap);
 * ------------------------
 * This method aims to use the input Mat graph file sourcePic to generate the corresponding tone map in different ways.
 */
void PencilDrawing_Tone::generate_tone_map(Mat &sourcePic, Mat &ToneMap) {
    //Create an empty histogram with the size of 256 and every value is 0.0 initially.
    vector<double> histogram(256, 0.0);

    //This is very important to use the pointer to handle the data of image.
    //In OpenCV, the data of image can be directly accessed and modified using the pointer uchar*.
    const uchar* imagedata = (uchar*) sourcePic.data;

    //Use the for loop to go through every pixel of the graph.
    //After the loop, the histogram of the input graph is generated.
    for (int row = 0; row < sourcePic.rows; row++) {
        for ( int col = 0; col < sourcePic.cols ; col++) {
            //Next move is the essential one. imagedata[row * sourcePic.cols + col] is the color value (0 - 255) the pixel has.
            //Then histogram[corlor value] is the corresponding color value in the histogram and it add one.
            histogram[imagedata[row * sourcePic.cols + col]] += 1;
        }
    }

    //This step is like the one in ComputeDistribution. The histogram can be compared only by fractions.
    int SizeHistogram = histogram.size();
    for(int i = 0; i < SizeHistogram; i++) {
        histogram[i] = histogram[i] / (sourcePic.cols * sourcePic.rows);
        //histogram[i] now represents the fraction of one color value's fraction among the all pixels of the picture.
    }

    //Use the same pointer method to handle the data of the tone map.
    uchar* ToneMapData = ToneMap.data;

    //Use the for loop to go through every pixel of the tone map to do the histrogram matching work.
    for (int row = 0; row < sourcePic.rows; row++) {
        for ( int col = 0; col < sourcePic.cols ; col++) {
             //Here, the value in the histogramMatching function is the number of color value the input image's pixel has.
             ToneMapData[row * sourcePic.cols + col] =
                     histogramMatching(histogram[imagedata[row * sourcePic.cols + col]]);
             //After the function, the pixel (ToneMapData[row * sourcePic.cols + col]) has the color value return by histogramMatching.
        }
    }
}

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
uchar PencilDrawing_Tone::histogramMatching(double value) {
    int sizeDistribution = distribution.size();

    //Set the mostSimilarIndex as 0 initially.
    uchar mostSimilarIndex = 0;

    //This is the most important step.
    //Use the for loop to go through 0 - 255 color value.
    for (int i = 0; i < sizeDistribution ; ++i) {
        //The value is the number of a color value.
        if (abs(distribution[i] - value) < abs(distribution[mostSimilarIndex] - value)) {
            //Use the abs() to find the most similar index of the color value in both histograms in the distribution and the input graph.
            //For example, distribution[250] means the number of pixels has the color value of 250.
            //If distribution[250] = 100 and value = 100, then return 250.
            mostSimilarIndex = i;
        }
    }
    //This mostSimilarIndex also represents the color value.
    return mostSimilarIndex;
}

/*
 * Method: BlackWhiteTone(Mat & sourcePic_BW);
 * Usage: Mat ToneMap = BlackWhiteTone(sourcePic_BW);
 * ------------------------
 * This method is only for the input picture SourcePic_BW that is grey picture.
 * This method is faster than GenerateTheBestTone(Mat & sourcePic_RGB) because it only needs to handle one channel.
 */
Mat PencilDrawing_Tone::BlackWhiteTone(Mat & sourcePic_BW) {
    //Use Mat::zeros to make the newly estalish ToneMap_BW has the same rows and cols. CV_8UC1 means the Mat has one channel.
    Mat ToneMap_BW = Mat::zeros(sourcePic_BW.rows, sourcePic_BW.cols, CV_8UC1);

    //Then use function generate_tone_map() to generate the tone map.
    generate_tone_map(sourcePic_BW, ToneMap_BW);

    return ToneMap_BW;
}

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
Mat PencilDrawing_Tone::GenerateTheBestTone(Mat &sourcePic_RGB) {
    //First, create a vector to store the 3 channels data.
    vector<Mat> channels;

    //Create three Mat variables represent 3 channels (Blue, Green and Red).
    Mat imageBlueChannel;
    Mat imageGreenChannel;
    Mat imageRedChannel;

    //If the Tone_Method is 2, the input color image needs first to be equalized.
    if(Tone_Method == 2) {
        //In order to do the equalization, the picture needed to be read as pointer IplImage*. Therefore, I write a temporary file.
        imwrite(path + "/Temp.png",sourcePic_RGB);
        string temp_path = path + "/Temp.png";
        const char *C_path = temp_path.c_str();
        IplImage* tempted = cvLoadImage(C_path);

        //Remove the temporary file to release the storage memory.
        remove(C_path);

        //Use the Equalization_Image() to Equalize the histogram of the image.
        sourcePic_RGB = Equalization_Image(tempted);
    }

    //I write a temporary file for later pointer IplImage to read.
    imwrite(path + "/Temp.png",sourcePic_RGB);

    //Split the sourcePic_RGB to three channels and stored them in channels vector.
    split(sourcePic_RGB, channels);

    //In OpenCV, the order of color is Blue, Green and Red (BGR) instead of RGB).
    imageBlueChannel = channels.at(0);
    imageGreenChannel = channels.at(1);
    imageRedChannel = channels.at(2);

    //After using the channels vector, I clear it to release some memory.
    channels.clear();

    double sum_Blue, sum_Red, sum_Green;
    sum_Blue = sum_Red = sum_Green = 0;

    string temp_path_2 = path + "/Temp.png";
    const char *C_path_2 = temp_path_2.c_str();
    //Read the Temp picture as the pointer IplImage.
    IplImage* image_temp = cvLoadImage(C_path_2);

    //This is the variable stores the BGR(A)(A stands for the transparency value which will not be used usually).
    CvScalar p;

    double Blue, Green, Red;

    int Height = image_temp->height;
    int Width = image_temp->width;

    //Get the iteration according to the size of the picture.
    int iteration = Get_iteration(image_temp);

    //Use the for loop to go through the picture according to the iteration calculated before.
    //This method mainly to find out which color among (BGR) is the dominant, secondary and least one.
    for (int row = 0; row < Height; row += iteration) {
        for ( int col = 0; col < Width ; col += iteration) {

            //cvGet2D is the function to get the image_temp's pixel at [row,col]'s color value.
            p = cvGet2D(image_temp, row, col);

            //Blue has the value of the Blue value of the pixel.
            Blue = p.val[0];

            //Green has the value of the Green value of the pixel.
            Green = p.val[1];

            //Red has the value of the Red value of the pixel.
            Red = p.val[2];

            double Largest, Mid, Smallest;

            //This function first find the order of Blue, Green and Red and put the value to the Largest, Mid and Smallest.
            Find_order(Blue, Green, Red, Largest, Mid, Smallest);

            //Add one to the most dominant color.
            if (Largest == Blue) {
                sum_Blue += 1;
            }

            if (Largest == Green) {
                sum_Green += 1;
            }

            if(Largest == Red) {
                sum_Red += 1;
            }
        }
    }

    //Create three Mat variable to have the same size as the sourcePic_RGB with only one channel
    //to later store the Blue, Green, Red channels' tone map.
    Mat ToneMap_Blue = Mat::zeros(sourcePic_RGB.rows, sourcePic_RGB.cols, CV_8UC1);
    Mat ToneMap_Green = Mat::zeros(sourcePic_RGB.rows, sourcePic_RGB.cols, CV_8UC1);
    Mat ToneMap_Red = Mat::zeros(sourcePic_RGB.rows, sourcePic_RGB.cols, CV_8UC1);

    //Generate the tone map for Blue, Green and Red Channel.
    generate_tone_map(imageBlueChannel, ToneMap_Blue);
    generate_tone_map(imageGreenChannel, ToneMap_Green);
    generate_tone_map(imageRedChannel, ToneMap_Red);

    Mat ToneMap_Both = Mat::zeros(sourcePic_RGB.rows, sourcePic_RGB.cols, CV_8UC1);
    Mat ToneMap_RGB_Result = Mat::zeros(sourcePic_RGB.rows, sourcePic_RGB.cols, CV_8UC1);

    double Largest_Tone, Mid_Tone, Smallest_Tone;

    //Find the order of the Blue, Green and Red tone.
    Find_order(sum_Blue, sum_Red, sum_Green, Largest_Tone, Mid_Tone, Smallest_Tone);

    //If the histogram is not equalized, the Tone map has the relatively good effect
    //when the Tonemap are added according to the weight of the BGR.
    if(Tone_Method == 1) {

        double Weight_Blue, Weight_Green, Weight_Red, Weight_B1, Weight_G1, Weight_BG;

        //Calculate the weight of BGR.
        Weight_calculation(sum_Blue, sum_Green, sum_Red,
                           Weight_Blue, Weight_Green, Weight_Red,
                           Weight_B1, Weight_G1, Weight_BG);

        //Add the three tone map according to the weight.
        //The formula is based on the math: for a, b, c.
        //a / (a + b) + b / (a + b) = 1.
        addWeighted(ToneMap_Blue, Weight_B1, ToneMap_Green, Weight_G1, 0, ToneMap_Both);
        //(a + b) / (a + b + c) + c / (a + b + c) = 1
        addWeighted(ToneMap_Both, Weight_BG, ToneMap_Red, Weight_Red, 0, ToneMap_RGB_Result);
    }
    //When the histogram is equalized,
    //I find that put the most dominant tone map as the final tone map can achieve a good effect.
    if(Tone_Method == 2) {

        if (Largest_Tone == sum_Blue) {
            ToneMap_RGB_Result = ToneMap_Blue;
        } else if (Largest_Tone == sum_Green) {
            ToneMap_RGB_Result = ToneMap_Green;
        } else if (Largest_Tone == sum_Red) {
            ToneMap_RGB_Result = ToneMap_Red;
        } else {
        Mat ToneMap_BR = Mat::zeros(sourcePic_RGB.rows, sourcePic_RGB.cols, CV_8UC1);

        //If the above will not be executed, then there is no largest one, add the three tone map evenly.
        addWeighted(ToneMap_Blue, 0.5, ToneMap_Red, 0.5,0, ToneMap_BR);
        addWeighted(ToneMap_BR,0.67, ToneMap_Green, 0.33, 0, ToneMap_RGB_Result);
        }
    }
    //These steps use the convertion function in training.cpp to conververt the Mat ToneMap to QImage.
    settmp(ToneMap_Blue);
    confirmPic();
    BGR_Tone.push(getQImage());

    settmp(ToneMap_Green);
    confirmPic();
    BGR_Tone.push(getQImage());

    settmp(ToneMap_Red);
    confirmPic();
    BGR_Tone.push(getQImage());

    //Remove the temporary file.
    remove(C_path_2);

    return ToneMap_RGB_Result;
}

/*
 * Method: Get_iteration(IplImage * & image_temp);
 * Usage: int iteration = Get_iteration(image_temp);
 * ------------------------
 * This method is to optimize the run time of generate the best tone map.
 * Judging by the resolution of the picture, the method return different iteration value.
 */
int PencilDrawing_Tone::Get_iteration(IplImage * & image_temp) {
    //Set the initial iteration as 100000.
    int iteration = 100000;

    //Calculate the size of the input image.
    int size = image_temp->height * image_temp->width;

    //Set the iteration according to the different sizes of the picture.
    if (size <= 480 * 320) {
        iteration = 10;
    }
    if (size > 480 * 320 && size <= 720 * 480) {
        iteration = 15;
    }
    if (size > 480 * 320 && size <= 960 * 640 ) {
        iteration = 20;
    }
    if (size > 960 * 640 && size <= 1280 * 720) {
        iteration = 25;
    }
    if (size > 1280 * 720 && size <= 1440 * 900 ) {
        iteration = 30;
    }
    if (size > 1440 * 900 && size <= 1600 * 900 ) {
        iteration = 35;
    }
    if (size > 1600 * 900 && size <= 1920 * 1080) {
        iteration = 45;
    }
    if (size > 1920 * 1080 && size <= 1920 * 1440) {
        iteration = 50;
    }
    if (size > 1920 * 1440 && size <= 2500 * 2000) {
        iteration = 60;
    }
    if (size > 2500 * 2000 && size <= 3000 * 2000) {
        iteration = 65;
    }
    if (size > 3000 * 2000 && size <= 3648 * 2736) {
        iteration = 75;
    }
    if (size > 3648 * 2736 && size <= 4480 * 3360) {
        iteration = 85;
    }
    if (size > 4480 * 3360 && size <= 5120 * 3840) {
        iteration = 90;
    }
    if (size > 5120 * 3840 && size <= 7216 * 5412) {
        iteration = 95;
    }
    if (size > 7216 * 5412 && size <= 9600 * 7200) {
        iteration = 100;
    }
    //There are only a few picture that is larger than 9600 * 7200. Therefore, no need to continue to catagorize.
    if (size > 9600 * 7200) {
        iteration = 110;
    }
    return iteration;
}

/*
 * Method: Weight_calculation(double & sum_Blue, double & sum_Green, double & sum_Red, double & Weight_Blue, double & Weight_Green, double & Weight_Red, double & Weight_B1, double & Weight_G1, double & Weight_BG);
 * Usage: pencil.Weight_calculation(sum_Blue, sum_Green, sum_Red, Weight_Blue, Weight_Green, Weight_Red, Weight_B1, Weight_G1, Weight_BG);
 * ------------------------
 * This method aims to return a sorted list of three values.
 */
void PencilDrawing_Tone::Weight_calculation(double & sum_Blue, double & sum_Green, double & sum_Red,
                                            double & Weight_Blue, double & Weight_Green, double & Weight_Red,
                                            double & Weight_B1, double & Weight_G1, double & Weight_BG) {
    double total = sum_Blue + sum_Red + sum_Green;

    Weight_Blue = sum_Blue / total;

    Weight_Green = sum_Green / total;

    Weight_Red = sum_Red / total;

    Weight_B1 = sum_Blue / (sum_Blue + sum_Green);

    Weight_G1 = sum_Green / (sum_Blue + sum_Green);

    Weight_BG = Weight_Blue + Weight_Green;
}

/*
 * Method: Find_order(double & sum_Blue, double & sum_Red, double & sum_Green, double & Largest, double & Mid, double & Smallest);
 * Usage: pencil.Find_order(sum_Blue, sum_Red, sum_Green, Largest, Mid, Smallest);
 * ------------------------
 * This method aims to find the order among three values and put the value into the double type value: Largest, Mid and Smallest.
 */
void PencilDrawing_Tone::Find_order(double & sum_Blue, double & sum_Red, double & sum_Green, double & Largest, double & Mid, double & Smallest) {
    //Create a vector of double to find the order.
    vector<double> Sorting(3);

    //SortList() return the vector in the decrescent order.
    Sorting = SortList(sum_Blue, sum_Red, sum_Green);

    Largest = Sorting[0];
    Mid = Sorting[1];
    Smallest = Sorting[2];
}

/*
 * Method: SortList(double & sum_Blue, double & sum_Red, double & sum_Green);
 * Usage: vector<int> sort = SortList(sum_Blue, sum_Red, sum_Green);
 * ------------------------
 * This method aims to return a sorted list of three values.
 */
vector<double> PencilDrawing_Tone::SortList(double & sum_Blue, double & sum_Red, double & sum_Green) {
    vector<double> Sorting(3);
    //Because there are only three variables, I use the simplest comparison between them to sort the vector.
    if(sum_Blue >= sum_Green && sum_Blue >= sum_Red)
    {
        Sorting[0] = sum_Blue;
        if (sum_Green >= sum_Red){
            Sorting[1] = sum_Green;
            Sorting[2] = sum_Red;
        } else {
            Sorting[1] = sum_Red;
            Sorting[2] = sum_Green;
        }
    }
    if (sum_Green >= sum_Blue && sum_Green >= sum_Red)
    {
        {
            Sorting[0] = sum_Green;
            if (sum_Blue >= sum_Red)
            {
                Sorting[1] = sum_Blue;
                Sorting[2] = sum_Red;
            } else {
                Sorting[1] = sum_Red;
                Sorting[2] = sum_Blue;
            }
        }
    }
    if (sum_Red >= sum_Blue && sum_Red >= sum_Green)
    {
        Sorting[0] = sum_Red;
        if (sum_Blue >= sum_Green)
        {
            Sorting[1] = sum_Blue;
            Sorting[2] = sum_Green;
        } else {
            Sorting[1] = sum_Green;
            Sorting[2] = sum_Blue;
        }
    }
    return Sorting;
}

/*
 * Method: change_Tone_method1();
 * Usage: pencil.change_Tone_method1();
 * ------------------------
 * Because there are two ways to handle the BGR picture. This is to make the private variable Tone_Method equal to 1.
 * When Tone_Method is 1, the function will use method 1 to generate the tone map.
 */
void PencilDrawing_Tone::change_Tone_method1() {
    Tone_Method = 1;
}

/*
 * Method: change_Tone_method2();
 * Usage: pencil.change_Tone_method2();
 * ------------------------
 * This is to make the private variable Tone_Method equal to 2.
 * When Tone_Method is 2, the function will use method 2 to generate the tone map.
 */
void PencilDrawing_Tone::change_Tone_method2() {
    Tone_Method = 2;
}

/*
 * Method: Start_function();
 * Usage: Mat ToneMap = pencil.Start_function();
 * ------------------------
 * This method is to integrate my methods in order to more simply generate a Tone Map.
 */
Mat PencilDrawing_Tone::Start_function() {
    //The main function will read the input image as G_SourceImage.
    Mat SourcePic = G_SourceImage;

    ComputeDistribution();

    Mat ToneMap_Final;

    //Use the if structure to judge whether the input image is a RGB image or a grey image.
    if(is_RGB(SourcePic) == true)
    {
        //If the input image is a RGB image.
        ToneMap_Final = GenerateTheBestTone(SourcePic);
    }
    else
    {
        //If the input image is a grey image, the application will handle it faster than the RGB one.
        ToneMap_Final = BlackWhiteTone(SourcePic);
    }

    //Add the pencil texture to the generated tone map.
    Add_Texture(ToneMap_Final);


    //Set the tmpImage in training.cpp as ToneMap_final.
    settmp(ToneMap_Final);

    return ToneMap_Final;
}

/*
 * Method: is_RGB(Mat SourcePic);
 * Usage: pencil.is_RGB(SourcePic);
 * ------------------------
 * This method aims to judge whether the input picture SourcePic is a RGB (3 channels) picture.
 * Return true if the picture is a RGB picture. Return false if the picture is a grey picture.
 */
bool PencilDrawing_Tone::is_RGB(Mat SourcePic) {
    //If the channels of the image is 3, then it is an RGB image.
    if(SourcePic.channels() == 3) {
        return true;
    } else {
        return false;
    }
}

/*
 * Method: get_height(Mat SourcePic);
 * Usage: int height = get_height(SourcePic);
 * ------------------------
 * This method is to get the height of the input picture SourcePic.
 */
int PencilDrawing_Tone::get_height(Mat SourcePic) {
    int height = 0;

    height = SourcePic.rows;

    return height;
}

/*
 * Method: get_width(Mat SourcePic);
 * Usage: int height = get_width(SourcePic);
 * ------------------------
 * This method is to get the width of the input picture SourcePic.
 */
int PencilDrawing_Tone::get_width(Mat SourcePic) {
    int width = 0;

    width = SourcePic.cols;

    return width;
}

/*
 * Method: Add_Texture(Mat & SourcePic);
 * Usage: Mat ToneMap = Add_Texture(SourcePic);
 * ------------------------
 * This method is to add a layer of pencil texture to the input picture SourcePic.
 */
void PencilDrawing_Tone::Add_Texture(Mat & SourcePic) {

    //Read the texture stored inside the Texture folder in the root path.
    QImage TepTexture(":/new/Images/Texture/PencilTexture.png"); //This is in the Resources.

    //Because the convertion between Mat and QImage can result in many errors.
    //Therefore, we try to save a temporary file for directly read in different format.
    TepTexture.save(qpath + "/TempTexture.png");
    Mat texture = imread(path + "/TempTexture.png", 0);
    string temp_textpath = path + "/TempTexture.png";
    const char *text_path = temp_textpath.c_str();
    remove(text_path);

    //Resize the texture image to have the same size as the SourcePic it need to be added to.
    resize(texture, texture, Size(get_width(SourcePic), get_height(SourcePic)));

    //The proportion of 0.88 and 0.12 has relatively good effect and lucky number.
    addWeighted(SourcePic,0.88, texture,0.12,0, SourcePic);
}

/*
 * Method: Equalization_Image(IplImage * & image);
 * Usage: sourcePic_RGB = Equalization_Image(image);
 * ------------------------
 * This method is to equalize the input image to make its histogram distribute more uniformly.
 */
Mat PencilDrawing_Tone::Equalization_Image(IplImage * & SourceImage) {

    //Create 4 IplImage pointor variable.
    IplImage * EqualImage = cvCreateImage(cvGetSize(SourceImage), SourceImage->depth, 3);
    IplImage * BlueCha = cvCreateImage(cvGetSize(SourceImage), SourceImage->depth, 1);
    IplImage * RedCha = cvCreateImage(cvGetSize(SourceImage), SourceImage->depth, 1);
    IplImage * GreenCha = cvCreateImage(cvGetSize(SourceImage), SourceImage->depth, 1);

    //Split the sourceImage to 3 channels.
    cvSplit(SourceImage, BlueCha, GreenCha, RedCha, NULL);

    //Equalize the Blue, Green and Red channels respectively.
    cvEqualizeHist(BlueCha, BlueCha);
    cvEqualizeHist(GreenCha, GreenCha);
    cvEqualizeHist(RedCha,RedCha);

    //After the equalization, merge them to EqualImage.
    cvMerge(BlueCha, GreenCha, RedCha, NULL, EqualImage);

    //Convert the IplImage to the Mat image.
    Mat EqualRGB = cvarrToMat(EqualImage);

    return EqualRGB;
}

/*
 * Method: turnpictureintowhite(Mat & SourcePic);
 * Usage: Mat sourcePic_BW = turnpictureintowhite(SourcePic);
 * ------------------------
 * This method aims to turn the input SourcePic to black and white picture which has only one channel.
 */
Mat PencilDrawing_Tone::turnpictureintowhite(Mat & SourcePic) {

    Mat sourcePic_BW;

    //cvtColor is an OpenCV function that converts the color type of the image.
    cvtColor(SourcePic, sourcePic_BW, CV_BayerRG2GRAY);

    return sourcePic_BW;
}

/*
 * Method: Get_BGR_Tone_Queue;
 * Usage: queue BGRTone = pencil.Get_BGR_Tone_Queue();
 * ------------------------
 * This is to get the BGR Tone.
 */
queue<QImage> PencilDrawing_Tone::Get_BGR_Tone_Queue() {
    return BGR_Tone;
}

/*
 * Method: Clear_BGR_Tone_Queue;
 * Usage: pencil.Clear_BGR_Tone_Queue();
 * ------------------------
 * This is to clear the queue BGR_Tone.
 */
void PencilDrawing_Tone::Clear_BGR_Tone_Queue() {
    while(!BGR_Tone.empty()) {
        BGR_Tone.pop();
    }
}

/*
 * Method: Get_Gsource;
 * Usage: sourcePic = pencil.Get_Gsource();
 * ------------------------
 * This is to get G_SourceImage.
 */
Mat PencilDrawing_Tone::Get_Gsource() {
    return G_SourceImage;
}
