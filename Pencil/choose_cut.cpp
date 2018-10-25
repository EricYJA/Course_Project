#include "choose_cut.h"

// ========================= Implementation Part ========================= //

//store the starting point of the mouse
Point g_pt1 = Point(0, 0);

//store the ending point of the mouse
Point g_pt2 = Point(0, 0);

//tell the mouse event to selsct or not
bool g_selecting = false;

Choose_Cut::Choose_Cut() {
    /* empty */
}

Choose_Cut::Choose_Cut(QImage * image, double min, double max)
    :Pic(image) {
    minSize = min;
    maxSize = max;
    Mat tmp = getsrc();

    // this part is used to set the size of the image to fit the window

    colsize = tmp.cols;
    rowsize = tmp.rows;
    double col_row = (double) colsize / rowsize;

    if (colsize > maxSize || rowsize > minSize) {
        if (col_row >= (double) maxSize / minSize) {
            colsize = maxSize;
            rowsize = colsize / col_row;
        }

        else {
            rowsize = minSize;
            colsize = rowsize * col_row;
        }

        resize(tmp, srcImage, Size(colsize, rowsize), INTER_AREA);
    }
    else srcImage = tmp;
}

Choose_Cut::Choose_Cut(Mat src, double min, double max)
    :Pic(src) {
    minSize = min;
    maxSize = max;
    Mat tmp = getsrc();

    // this part is used to set the size of the image to fit the window

    colsize = tmp.cols;
    rowsize = tmp.rows;
    double col_row = (double) colsize / rowsize;

    if (colsize > maxSize || rowsize > minSize) {
        if (col_row >= (double) maxSize / minSize) {
            colsize = maxSize;
            rowsize = colsize / col_row;
        }

        else {
            rowsize = minSize;
            colsize = rowsize * col_row;
        }

        resize(tmp, srcImage, Size(colsize, rowsize), INTER_AREA);
    }
    else srcImage = tmp;
}

Choose_Cut::Choose_Cut(string route, double min, double max)
    :Pic(route) {
    minSize = min;
    maxSize = max;
    Mat tmp = getsrc();

    // this part is used to set the size of the image to fit the window

    colsize = tmp.cols;
    rowsize = tmp.rows;
    double col_row = (double) colsize / rowsize;

    if (colsize > maxSize || rowsize > minSize) {
        if (col_row >= (double) maxSize / minSize) {
            colsize = maxSize;
            rowsize = colsize / col_row;
        }

        else {
            rowsize = minSize;
            colsize = rowsize * col_row;
        }

        resize(tmp, srcImage, Size(colsize, rowsize), INTER_AREA);
    }
    else srcImage = tmp;
}


//void Choose_Cut::operating() {

//    imshow("operating", srcImage);
//    recordBegin.push(srcImage);
//    tmpImage = srcImage;
//    int key = 0;
//    while(1) {

//        key = waitKey(9);

//        switch(key) {


//        case'j':
//            destroyWindow("operating");
//            recordBegin.push(change(tmpImage));
//            break;
//        case'k':
//            destroyWindow("operating");
//            recordBegin.push(getCut(tmpImage));
//            break;
//        case'l':
//            destroyWindow("operating");
//            recordBegin.push(rotate(tmpImage));
//            break;


//        case'r':
//            recordEnd.push(recordBegin.top());
//            recordBegin.pop();

//            if (recordBegin.empty()) {
//                recordBegin.push(recordEnd.top());
//                recordEnd.pop();
//            }

//            break;

//        case't':
//            if (recordEnd.empty() == false) {
//                recordBegin.push(recordEnd.top());
//                recordEnd.pop();
//            }
//            break;
//        }

//        tmpImage = recordBegin.top();

//        imshow("operating", tmpImage);
//        if (key == '\r') {
//            dstImage = tmpImage;
//            settmp(dstImage);
//            destroyWindow("operating");
//            break;
//        }

//        if (key == 27) {
//            destroyAllWindows();
//            exit(0);
//            break;
//        }

//    }


//}


Mat Choose_Cut::getCut(/*Mat tmp*/ double ratio = 0) {

    // letting tmpImage equal to srcImage
    tmpImage = srcImage;
    Mat tmp;
    tmp = tmpImage;

    namedWindow(WINDOW_NAME_1, WINDOW_AUTOSIZE);
    setMouseCallback(WINDOW_NAME_1, on_mouse);
    Mat dst;
    Mat ROIdst;
    int RectX;
    int RectY;

    dst = tmp.clone();

    bool shift = false;

    while(1) {
        tmp.copyTo(dst);

        // setting the ratio of the R
        Point t_pt;
        if (ratio == 0) {
            // free if ratio is 0
            t_pt.x = g_pt2.x;
            t_pt.y = g_pt2.y;
        }

        if (ratio == 1) {
            //square if ratio is 1
            if ( (g_pt2.x - g_pt1.x) > (g_pt2.y - g_pt1.y) ) {
                t_pt.x = g_pt2.x;
                t_pt.y = g_pt1.y + g_pt2.x - g_pt1.x;
            }

            else if ( (g_pt2.x - g_pt1.x) < (g_pt2.y - g_pt1.y) ) {
                t_pt.x = g_pt1.x + g_pt2.y - g_pt1.y;
                t_pt.y = g_pt2.y;
            }
            else {
                t_pt.x = g_pt2.x;
                t_pt.y = g_pt2.x;
            }
        }

        else if (ratio == (double) 16 / 9 ) {
            //change the width / hight == 16 / 9
            t_pt.x = g_pt2.x;
            t_pt.y = (int) g_pt1.y + ( (double) ( (g_pt2.x - g_pt1.x) / ratio ) );
        }

        else if (ratio == (double) 9 / 16 ) {
            //change the hight / width == 16 / 9
            t_pt.y = g_pt2.y;
            t_pt.x = (int) g_pt1.x + ( (double) ( (g_pt2.y - g_pt1.y) * ratio ) );
        }

        else if (ratio == (double) 4 / 3 ) {
            //change the width / hight == 4/3
            t_pt.x = g_pt2.x;
            t_pt.y = (int) g_pt1.y + ( (double) ( (g_pt2.x - g_pt1.x) / ratio ) );
        }

        else if (ratio == (double) 3 / 4 ) {
            //change the width / hight == 3/4
            t_pt.y = g_pt2.y;
            t_pt.x = (int) g_pt1.x + ( (double) ( (g_pt2.y - g_pt1.y) * ratio ) );
        }

        rectangle( dst, g_pt1, t_pt, Scalar(255, 255, 255) );

        imshow(WINDOW_NAME_1, dst);
        char key = waitKey(10);

        switch(key) {

        // using w, a, s, d to move the selected retangle
        case '\t':
            shift = !shift;
            break;

        case 'a':
            g_pt1.x--;
            g_pt2.x--;
            break;
        case 's':
            g_pt1.y++;
            g_pt2.y++;
            break;
        case 'd':
            g_pt1.x++;
            g_pt2.x++;
            break;
        case 'w':
            g_pt1.y--;
            g_pt2.y--;
            break;

        //using z, x to change the size of the retangle
        case 'z': //big
            if (g_pt1.x > g_pt2.x && g_pt1.y > g_pt2.y) {
                g_pt2.x--;
                g_pt1.y++;
                g_pt1.x++;
                g_pt2.y--;
            }

            else if (g_pt1.x < g_pt2.x && g_pt1.y > g_pt2.y) {
                g_pt1.x--;
                g_pt2.y--;
                g_pt2.x++;
                g_pt1.y++;
            }

            else if (g_pt1.x > g_pt2.x && g_pt1.y < g_pt2.y) {
                g_pt1.x++;
                g_pt2.y++;
                g_pt2.x--;
                g_pt1.y--;
            }

            else {
                g_pt1.x--;
                g_pt2.y++;
                g_pt2.x++;
                g_pt1.y--;
            }

            break;

        case 'x': // small
            if (g_pt1.x > g_pt2.x && g_pt1.y > g_pt2.y){
                g_pt1.x--;
                g_pt2.y++;
                g_pt2.x++;
                g_pt1.y--;
            }

            else if ((g_pt1.x > g_pt2.x && g_pt1.y < g_pt2.y)) {
                g_pt1.x--;
                g_pt2.y--;
                g_pt2.x++;
                g_pt1.y++;
            }

            else if (g_pt1.x < g_pt2.x && g_pt1.y > g_pt2.y) {
                g_pt1.x++;
                g_pt2.y++;
                g_pt2.x--;
                g_pt1.y--;
            }

            else {
                g_pt2.x--;
                g_pt1.y++;
                g_pt1.x++;
                g_pt2.y--;
            }

            break;

        //using return to confirm the retangle area
        case '\r':
            destroyWindow(WINDOW_NAME_1);


            if (g_pt1.x < g_pt2.x && g_pt1.y > g_pt2.y) {
                RectX = g_pt1.x;
                RectY = g_pt2.y;
            }

            else if (g_pt1.x <= g_pt2.x && g_pt1.y <= g_pt2.y) {
                RectX = g_pt1.x;
                RectY = g_pt1.y;
            }

            else if (g_pt1.x >= g_pt2.x && g_pt1.y >= g_pt2.y) {
                RectX = g_pt2.x;
                RectY = g_pt2.y;
            }

            else /*(g_pt1.x > g_pt2.x && g_pt1.y < g_pt2.y)*/ {
                RectX = g_pt2.x;
                RectY = g_pt1.y;
            }

            if (t_pt.x > tmp.cols) t_pt.x = tmp.cols;
            if (t_pt.y > tmp.rows) t_pt.y = tmp.rows;

            ROIdst = tmp(Rect(RectX, RectY, abs(t_pt.x-g_pt1.x), abs(t_pt.y-g_pt1.y)));
            if (ROIdst.data) {
                settmp(ROIdst);
                return ROIdst;
            }

            else {
                settmp(dst);
                return dst;
            }


        }

        if (key == 27) {
            destroyAllWindows();
            exit(0);
            break;
        }

    }
}

Mat Choose_Cut::change(/*Mat tmp*/ double ratio) {

    tmpImage = srcImage;
    Mat tmp;
    tmp = tmpImage;

    Mat dst;
    dst = tmp;

    colsize = tmp.cols;
    rowsize = tmp.rows;
    double col_row = (double) colsize / rowsize;

    //using the sign of the number to judge zooming in or out
    if (ratio > 0 && ratio > 125) {
        colsize = (int) colsize * (1.01) * (ratio / 125);
        rowsize = (int) rowsize * (1.01) * (ratio / 125);
    }

    else if (ratio < 0 && abs(ratio) > 125) {
        ratio = abs(ratio);
        colsize = (int) colsize / ((1.01) * (ratio / 125));
        rowsize = (int) rowsize / ((1.01) * (ratio / 125));
    }


    //the smallest size of the Image
    if (colsize <= 100 || rowsize <= 100) {

        tinyFlag = true;

        if (colsize <= rowsize) {
            colsize = 100;
            rowsize = colsize / col_row;
        }

        else {
            rowsize = 100;
            colsize = rowsize * col_row;
        }
    }

    else {
        tinyFlag = false;
    }

    //the largest size of the Image
    if (colsize >= maxSize || rowsize >= minSize) {

        bigFlag = true;

        if (col_row >= (double) maxSize / minSize) {
            colsize = maxSize;
            rowsize = colsize / col_row;
        }

        else {
            rowsize = minSize;
            colsize = rowsize * col_row;
        }
    }

    else {
        bigFlag = false;
    }

    resize(tmp, dst, Size(colsize, rowsize), INTER_AREA);
    settmp(dst);
    return dst;

}

bool Choose_Cut::getBigFlag() {
    return bigFlag;
}

bool Choose_Cut::getTinyFlag() {
    return tinyFlag;
}

Mat Choose_Cut::rotate(/*Mat tmp*/ double ratio) {

    tmpImage = srcImage;
    Mat tmp;
    tmp = tmpImage;

    Point center((int) tmp.cols / 2, (int) tmp.rows / 2);

    Mat dst;
    dst = tmp.clone();

    tmp.copyTo(dst);


    double angle = (double) ratio / 100;


    Mat rot = getRotationMatrix2D(center, angle, 1);
    Rect bbox = RotatedRect(center, tmp.size(), angle).boundingRect();

    rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;

    warpAffine(tmp, dst, rot, bbox.size());

    settmp(dst);
    return dst;
}



void on_mouse(int mouseEvent, int x, int y, int, void*) {
    switch(mouseEvent) {
    case EVENT_LBUTTONDOWN:
        g_pt1 = cvPoint(x, y);
        g_pt2 = cvPoint(x, y);
        g_selecting = true;
        break;

    case EVENT_MOUSEMOVE:
        if (g_selecting) {
            g_pt2 = cvPoint(x, y);
        }
        break;

    case EVENT_RBUTTONDOWN:
        g_pt2 = cvPoint(x, y);
        g_selecting = false;
        break;
    }
}
