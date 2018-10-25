#include "effect.h"
#include "tone.h"
#include "training.h"
#include "queue"
#include "arc.h"
#include "cartoon.h"
#include <iostream>
#include "qmessagebox.h"
#include "qdir.h"
#include "qapplication.h"
#include "qstring.h"

using namespace std;

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
queue<QImage>P1_standard(queue<QImage> &Show_queue) {
    //The idea is to let the main function pass a quee named Show_queue including the original picture.
    //After the processing, because of call by reference, the queue will leave with the tone map, the stoke and the result.
    //Too show the intermediate step, I also pass a BGR tone map queue as the output of the function.

    //Use QString string and const char * to store the running path of the Application.
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    queue<QImage> BGR_queue;
    PencilDrawing_Tone pencil_Tone_1;

    //Get the original picture from the main function.
    QImage source_Q = Show_queue.front();

    //Use a temporary file saving to convert the format of the picture.
    source_Q.save(qpath + "/tempsource.png");
    Mat sourcePic = imread(path + "/tempsource.png");
    remove(C_path);

    //Get the tone map.
    pencil_Tone_1 = PencilDrawing_Tone(sourcePic);
    pencil_Tone_1.change_Tone_method1();
    Mat ToneMap = pencil_Tone_1.Start_function();

    //Get the stoke.
    Stroke pencil_stroke;
    pencil_stroke = Stroke(sourcePic);
    pencil_stroke.set_stroke();
    Mat stroke_image = pencil_stroke.get_matStroke();

    //Get the BGR tone map.
    BGR_queue = pencil_Tone_1.Get_BGR_Tone_Queue();
    pencil_Tone_1.Clear_BGR_Tone_Queue();

    //Get the final result.
    Mat Target_pic;
    addWeighted(stroke_image, 0.45, ToneMap, 0.55, 0, Target_pic);

    //Push the stoke into the Show_queue.
    QImage Q_Stroke = pencil_stroke.get_stroke();
    Show_queue.push(Q_Stroke);

    //Push the tone map into the Show_queue.
    pencil_Tone_1.confirmPic();
    QImage Q_ToneMap = pencil_Tone_1.getQImage();
    Show_queue.push(Q_ToneMap);

    //Push the result into the Show_queue.
    imwrite(path + "/tempTarget.png", Target_pic);
    charpath = path + "/tempTarget.png";
    const char *RPath = charpath.c_str();
    QImage Q_Final_Result;
    Q_Final_Result.load(qpath + "/tempTarget.png");
    Show_queue.push(Q_Final_Result);
    remove(RPath);

    return BGR_queue;
}

/*
 * Function: P1_advance
 * Usage: queue Result = P1_advance(Show_queue);
 * ------------------------
 * This is to use the standard method including the sobel stoke and BGR tone mapping with first normalizing the graph.
 * It is the slowest because it needs first to normalize the histogram of the original graph then to go through 3 color channels during tone mapping.
 * Because the histogram has already been normalized, the combination according to the weight is not idealy good.
 * Therefore, I combine the three color tone map according to the color dominance of the original graph.
 * The combinations of tone mapping and stroke have already been adjusted to a relatively good effect.
 * You want to use this if your original graph is too bright or too dark.
 */
queue <QImage> P1_advance(queue<QImage> &Show_queue) {
    //The idea is to let the main function pass a quee named Show_queue including the original picture.
    //After the processing, because of call by reference, the queue will leave with the tone map, the sketch and the result.
    //Too show the intermediate step, I also pass a BGR tone map queue as the output of the function.

    //Use QString string and const char * to store the running path of the Application.
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    queue<QImage> BGR_queue;
    PencilDrawing_Tone pencil_Tone_2;

    //Get the original picture from the main function.
    QImage source = Show_queue.front();

    //Use a temporary file saving to convert the format of the picture.
    source.save(qpath + "/tempsource.png");
    Mat sourcePic = imread(path + "/tempsource.png");
    remove(C_path);

    //Get the tone map.
    pencil_Tone_2 = PencilDrawing_Tone(sourcePic);
    pencil_Tone_2.change_Tone_method2();
    Mat ToneMap = pencil_Tone_2.Start_function();

    //Get the stroke.
    Stroke pencil_stroke;
    pencil_stroke = Stroke(sourcePic);
    pencil_stroke.set_stroke();
    Mat stroke_image = pencil_stroke.get_matStroke();

    //Get the BGR tone map.
    BGR_queue = pencil_Tone_2.Get_BGR_Tone_Queue();
    pencil_Tone_2.Clear_BGR_Tone_Queue();

    //Get the final result.
    Mat Target_pic;
    addWeighted(stroke_image, 0.40, ToneMap, 0.60, 0, Target_pic);

    //Push the stroke into the Show_queue.
    QImage Q_Stroke = pencil_stroke.get_stroke();
    Show_queue.push(Q_Stroke);

    //Push the tone map into the Show_queue.
    pencil_Tone_2.confirmPic();
    QImage Q_ToneMap = pencil_Tone_2.getQImage();
    Show_queue.push(Q_ToneMap);

    //Push the result into the Show_queue.
    imwrite(path + "/tempTarget.png", Target_pic);
    charpath = path + "/tempTarget.png";
    const char *RPath = charpath.c_str();
    QImage Q_Final_Result;
    Q_Final_Result.load(qpath + "/tempTarget.png");
    Show_queue.push(Q_Final_Result);
    remove(RPath);

    return BGR_queue;
}

/*
 * Function: P1_quick
 * Usage: queue Result = P1_quick(Show_queue);
 * ------------------------
 * This is to use the standard method including the sobel stoke and Black and White tone mapping without first normalizing the graph.
 * It is realatively fast because when mapping the graph is handled only in one color channels.
 * The combinations of tone mapping and stroke have already been adjusted to a relatively good effect.
 * This is for the graph that is very large and can give you feedback in a short time.
 */
queue <QImage> P1_quick(queue<QImage> &Show_queue) {
    //The idea is to let the main function pass a quee named Show_queue including the original picture.
    //After the processing, because of call by reference, the queue will leave with the tone map, the sketch and the result.
    //Too show the intermediate step, I also pass a BGR tone map queue as the output of the function.

    //Use QString string and const char * to store the running path of the Application.
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    PencilDrawing_Tone pencil_Tone_3;

    //Get the original picture from the main function.
    QImage source = Show_queue.front();

    //Use a temporary file saving to convert the format of the picture.
    source.save(qpath + "/tempsource.png");
    Mat sourcePic_BGR = imread(path + "/tempsource.png");
    remove(C_path);
    Mat sourcePic;
    //Convert the graph to Grey. (1 channel)
    cvtColor(sourcePic_BGR, sourcePic, COLOR_BGR2GRAY);

    //Get the tone map. (Pass in the grey picture)
    pencil_Tone_3 = PencilDrawing_Tone(sourcePic);
    pencil_Tone_3.change_Tone_method1();
    Mat ToneMap = pencil_Tone_3.Start_function();

    //Get the stroke.
    Stroke pencil_stroke;
    pencil_stroke = Stroke(sourcePic_BGR);
    pencil_stroke.set_stroke();
    Mat stroke_image = pencil_stroke.get_matStroke();

    //Get the final result.
    Mat Target_pic;
    addWeighted(stroke_image, 0.45, ToneMap, 0.55, 0, Target_pic);

    //Push the stroke into the Show_queue.
    QImage Q_Stroke = pencil_stroke.get_stroke();
    Show_queue.push(Q_Stroke);

    //Push the tone map into the Show_queue.
    pencil_Tone_3.confirmPic();
    QImage Q_ToneMap = pencil_Tone_3.getQImage();
    Show_queue.push(Q_ToneMap);

    //Push the result into the Show_queue.
    imwrite(path + "/tempTarget.png", Target_pic);
    charpath = path + "/tempTarget.png";
    const char *RPath = charpath.c_str();
    QImage Q_Final_Result;
    Q_Final_Result.load(qpath + "/tempTarget.png");
    Show_queue.push(Q_Final_Result);
    remove(RPath);

    //Return an empty BGR_Tone because there is none.
    queue<QImage> BGR_Tone;
    return BGR_Tone;
}

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
queue<QImage> P2_standard(queue<QImage> &Show_queue) {
    //The idea is to let the main function pass a quee named Show_queue including the original picture.
    //After the processing, because of call by reference, the queue will leave with the tone map, the sketch and the result.
    //Too show the intermediate step, I also pass a BGR tone map queue as the output of the function.

    //Use QString string and const char * to store the running path of the Application.
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    queue<QImage> BGR_queue;
    PencilDrawing_Tone pencil_Tone_4;

    //Get the original picture from the main function.
    QImage source_Q = Show_queue.front();

    //Use a temporary file saving to convert the format of the picture.
    source_Q.save(qpath + "/tempsource.png");
    Mat sourcePic = imread(path + "/tempsource.png");
    remove(C_path);

    //Get the tone map.
    pencil_Tone_4 = PencilDrawing_Tone(sourcePic);
    pencil_Tone_4.change_Tone_method1();
    Mat ToneMap = pencil_Tone_4.Start_function();

    //Get the sketch.
    Stroke pencil_sketch;
    pencil_sketch = Stroke(sourcePic);
    pencil_sketch.set_sketch();
    Mat stroke_image = pencil_sketch.get_matSketch();

    //Get the BGR tone map.
    BGR_queue = pencil_Tone_4.Get_BGR_Tone_Queue();
    pencil_Tone_4.Clear_BGR_Tone_Queue();

    //Get the final result.
    Mat Target_pic;
    addWeighted(stroke_image, 0.55, ToneMap, 0.45, 0, Target_pic);

    //Push the sketch into the Show_queue.
    QImage Q_Sketch = pencil_sketch.get_sketch();
    Show_queue.push(Q_Sketch);

    //Push the tone map into the Show_queue.
    pencil_Tone_4.confirmPic();
    QImage Q_ToneMap = pencil_Tone_4.getQImage();
    Show_queue.push(Q_ToneMap);

    //Push the result into the Show_queue.
    imwrite(path + "/tempTarget.png", Target_pic);
    charpath = path + "/tempTarget.png";
    const char *RPath = charpath.c_str();
    QImage Q_Final_Result;
    Q_Final_Result.load(qpath + "/tempTarget.png");
    Show_queue.push(Q_Final_Result);
    remove(RPath);

    return BGR_queue;
}

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
queue <QImage> P2_advance(queue<QImage> &Show_queue) {
    //The idea is to let the main function pass a quee named Show_queue including the original picture.
    //After the processing, because of call by reference, the queue will leave with the tone map, the sketch and the result.
    //Too show the intermediate step, I also pass a BGR tone map queue as the output of the function.

    //Use QString string and const char * to store the running path of the Application.
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    queue<QImage> BGR_queue;
    PencilDrawing_Tone pencil_Tone_5;

    //Get the original picture from the main function.
    QImage source = Show_queue.front();

    //Use a temporary file saving to convert the format of the picture.
    source.save(qpath + "/tempsource.png");
    Mat sourcePic = imread(path + "/tempsource.png");
    remove(C_path);

    //Get the tone map.
    pencil_Tone_5 = PencilDrawing_Tone(sourcePic);
    pencil_Tone_5.change_Tone_method2();
    Mat ToneMap = pencil_Tone_5.Start_function();

    //Get the sketch.
    Stroke pencil_sketch;
    pencil_sketch = Stroke(sourcePic);
    pencil_sketch.set_sketch();
    Mat stroke_image = pencil_sketch.get_matSketch();

    //Get the BGR tone map.
    BGR_queue = pencil_Tone_5.Get_BGR_Tone_Queue();
    pencil_Tone_5.Clear_BGR_Tone_Queue();

    //Get the final result.
    Mat Target_pic;
    addWeighted(stroke_image, 0.55, ToneMap, 0.45, 0, Target_pic);

    //Push the sketch into the Show_queue.
    QImage Q_Sketch = pencil_sketch.get_sketch();
    Show_queue.push(Q_Sketch);

    //Push the tone map into the Show_queue.
    pencil_Tone_5.confirmPic();
    QImage Q_ToneMap = pencil_Tone_5.getQImage();
    Show_queue.push(Q_ToneMap);

    //Push the result into the Show_queue.
    imwrite(path + "/tempTarget.png", Target_pic);
    charpath = path + "/tempTarget.png";
    const char *RPath = charpath.c_str();
    QImage Q_Final_Result;
    Q_Final_Result.load(qpath + "/tempTarget.png");
    Show_queue.push(Q_Final_Result);
    remove(RPath);

    return BGR_queue;
}

/*
 * Function: P2_quick
 * Usage: queue Result = P2_quick(Show_queue);
 * ------------------------
 * This is to use the standard method including the Gamma sketch and Black and White tone mapping without first normalizing the graph.
 * It is realatively fast because when mapping the graph is handled only in one color channels.
 * The combinations of tone mapping and sketch have already been adjusted to a relatively good effect.
 * This is for the graph that is very large and can give you feedback in a short time.
 */
queue<QImage> P2_quick(queue<QImage> &Show_queue) {
    //The idea is to let the main function pass a quee named Show_queue including the original picture.
    //After the processing, because of call by reference, the queue will leave with the tone map, the sketch and the result.
    //Too show the intermediate step, I also pass a BGR tone map queue as the output of the function.

    //Use QString string and const char * to store the running path of the Application.
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    PencilDrawing_Tone pencil_Tone_6;

    //Get the original picture from the main function.
    QImage source = Show_queue.front();

    //Use a temporary file saving to convert the format of the picture.
    source.save(qpath + "/tempsource.png");
    Mat sourcePic_BGR = imread(path + "/tempsource.png");
    remove(C_path);
    Mat sourcePic;
    //Convert the graph to Grey. (1 channel)
    cvtColor(sourcePic_BGR, sourcePic, COLOR_BGR2GRAY);

    //Get the tone map. (Pass in the grey picture)
    pencil_Tone_6 = PencilDrawing_Tone(sourcePic);
    pencil_Tone_6.change_Tone_method1();
    Mat ToneMap = pencil_Tone_6.Start_function();

    //Get the sketch.
    Stroke pencil_sketch;
    pencil_sketch = Stroke(sourcePic_BGR);
    pencil_sketch.set_sketch();
    Mat stroke_image = pencil_sketch.get_matSketch();

    //Get the final result.
    Mat Target_pic;
    addWeighted(stroke_image, 0.55, ToneMap, 0.45, 0, Target_pic);

    //Push the sketch into the Show_queue.
    QImage Q_Sketch = pencil_sketch.get_sketch();
    Show_queue.push(Q_Sketch);

    //Push the tone map into the Show_queue.
    pencil_Tone_6.confirmPic();
    QImage Q_ToneMap = pencil_Tone_6.getQImage();
    Show_queue.push(Q_ToneMap);

    //Push the result into the Show_queue.
    imwrite(path + "/tempTarget.png", Target_pic);
    charpath = path + "/tempTarget.png";
    const char *RPath = charpath.c_str();
    QImage Q_Final_Result;
    Q_Final_Result.load(qpath + "/tempTarget.png");
    Show_queue.push(Q_Final_Result);
    remove(RPath);

    //Return an empty BGR_Tone because there is none.
    queue<QImage> BGR_Tone;
    return BGR_Tone;

}


/*
 * Implementation notes: P3_1
 * -----------------------------------
 * This function get QImage from parameter and transfer it to Mat.
 * Process the mat image and return QImage back. Push it by call
 * by reference.
 */

void P3_1(queue<QImage> & Show_queue) {
    // get the path of input file
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    // get the image of input file
    QImage tmpImage = Show_queue.front();
    tmpImage.save(qpath + "/tempsource.png");
    Mat srcImage = imread(path + "/tempsource.png");
    remove(C_path);

    // image processing
    Arc cannyColorProcess = Arc(srcImage);
    cannyColorProcess.set_canny_color();
    QImage cannyColorImage = cannyColorProcess.get_canny_color();
    Show_queue.push(cannyColorImage);
}

/*
 * Implementation notes: P3_2
 * -----------------------------------
 * This function get QImage from parameter and transfer it to Mat.
 * Process the mat image and return QImage back. Push it by call
 * by reference.
 */

void P3_2(queue<QImage> & Show_queue) {
    // get the path of input file
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    // get the image of input file
    QImage tmpImage = Show_queue.front();
    tmpImage.save(qpath + "/tempsource.png");
    Mat srcImage = imread(path + "/tempsource.png");
    remove(C_path);

    // image processing
    Arc cannyWhiteProcess = Arc(srcImage);
    cannyWhiteProcess.set_canny_white();
    QImage cannyWhiteImage = cannyWhiteProcess.get_canny_white();
    Show_queue.push(cannyWhiteImage);
}

/*
 * Implementation notes: P3_3
 * -----------------------------------
 * This function get QImage from parameter and transfer it to Mat.
 * Process the mat image and return QImage back. Push it by call
 * by reference.
 */

void P3_3(queue<QImage> & Show_queue) {
    // get the path of input file
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    // get the image of input file
    QImage tmpImage = Show_queue.front();
    tmpImage.save(qpath + "/tempsource.png");
    Mat srcImage = imread(path + "/tempsource.png");
    remove(C_path);

    // image processing
    Arc laplacianWhiteProcess = Arc(srcImage);
    laplacianWhiteProcess.set_laplacian_white();
    QImage laplacianWhiteImage = laplacianWhiteProcess.get_laplacian_white();
    Show_queue.push(laplacianWhiteImage);
}

/*
 * Implementation notes: P4
 * -----------------------------------
 * This function get QImage from parameter and transfer it to Mat.
 * Process the mat image and return QImage back. Push it by call
 * by reference.
 */

void P4(queue<QImage> & Show_queue) {
    // get the path of input file
    QString qpath = QApplication::applicationDirPath();
    string path = qpath.toStdString();
    string charpath = path + "/tempsource.png";
    const char *C_path = charpath.c_str();

    // get the image of input file
    QImage tmpImage = Show_queue.front();
    tmpImage.save(qpath + "/tempsource.png");
    Mat srcImage = imread(path + "/tempsource.png");
    remove(C_path);

    // image processing
    Cartoon cartoonProcess = Cartoon(srcImage);
    cartoonProcess.set_cartoon();
    QImage cartoonImage = cartoonProcess.get_cartoon();
    Show_queue.push(cartoonImage);
}

