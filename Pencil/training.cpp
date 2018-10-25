#include "training.h"
#include "QImage"
#include "qdebug.h"

// ===================== Implementation Part ===================== //

Pic::Pic() {
    flag = false; //creat a flag to record if the photo is confirmed
}


Pic::Pic(QImage * image) {
    cv::Mat mat;// creat a Mat to store the data

    switch(image->format())
    {

    // return error and exit if the QImage is wrong input
    case QImage::Format_Invalid:
        cout<<"Loading error"<<endl;
        exit(0);
        break;

    // Include all the cases of image.format()
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
    case QImage::Format_RGB16:
    case QImage::Format_ARGB8565_Premultiplied:
    case QImage::Format_RGB666:
    case QImage::Format_ARGB6666_Premultiplied:
    case QImage::Format_RGB555:
    case QImage::Format_ARGB8555_Premultiplied:
    case QImage::Format_RGB444:
    case QImage::Format_ARGB4444_Premultiplied:
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
    case QImage::Format_RGBA8888_Premultiplied:
    case QImage::Format_A2BGR30_Premultiplied:
    case QImage::Format_A2RGB30_Premultiplied:
    case QImage::Format_RGB30:
    case QImage::Format_BGR30:
    case QImage::Format_Grayscale8:
    case QImage::Format_Alpha8:
    case QImage::NImageFormats:
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:

    // for some specific cases use different implementation
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image->height(), image->width(), CV_8UC4, (void*)image->bits(), image->bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image->height(), image->width(), CV_8UC3, (void*)image->bits(), image->bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image->height(), image->width(), CV_8UC1, (void*)image->bits(), image->bytesPerLine());
        break;
    }

    srcImage = mat; // set the srcImage
    tmpImage = srcImage; // set the tmpImage
}


Pic::Pic(Mat src) {
    if(!src.data || src.empty()){
        cout<<"Loading error"<<endl;
        exit(0);
    }

// testing part
//    cout << src.cols << endl;
//    cout << src.rows << endl;
//    cout << src.cols / src.rows << endl;

    srcImage = src; // set the srcImage
    tmpImage = srcImage; // set the tmpImage
}

Pic::Pic(string route) {
    Mat src;
    src = imread(route);

    if(!src.data || src.empty()){
        cout<<"Loading error"<<endl;
        exit(0);
    }

    srcImage = src;
    tmpImage = srcImage;
}

bool Pic::getFlag() {
    return flag;
}

Mat Pic::getdst() {
    return dstImage;
}

Mat Pic::getsrc() {
    return srcImage;
}

Mat Pic::gettmp() {
    return tmpImage;
}

void Pic::settmp(Mat tmp) {
    tmpImage = tmp;
}

void Pic::standardSize() {

    Mat tmp;
    double ratio =(double) srcImage.cols / srcImage.rows;

// testing part
//    cout << srcImage.cols << endl;
//    cout << srcImage.rows << endl;
//    cout << ratio << "\t" << (double) 1080 / 680 << endl;
    if (ratio >= ((double) 1080/680)) {
        resize(srcImage, tmp, Size(1080, (srcImage.rows * ((double) 1080 / srcImage.cols))));
    }

    else if (ratio < ((double) 1080/680)) {
        resize(srcImage, tmp, Size((srcImage.cols * ((double) 680 / srcImage.rows)), 680));
    }

    tmpImage = tmp;

// testing part
//    cout << srcImage.cols << endl;
//    cout << srcImage.rows << endl;
}



void Pic::showsrc() {
    imshow("original_image", srcImage);
    waitKey(0);
    destroyWindow("original_image");
}

void Pic::showtmp() {
    imshow("temporary_image", tmpImage);
    waitKey(0);
    destroyWindow("temporary_image");

}

void Pic::showdst() {
    imshow("destination_image", dstImage);
    waitKey(0);
    destroyWindow("destination_image");

}

void Pic::confirmPic() {
    dstImage = tmpImage;

    flag = true;
}


QImage Pic::getQImage() {
    Mat mat;
//    cout << tmpImage.cols << endl;
    mat = tmpImage;

//    cout << mat.cols << endl;
    // 8-bits unsigned, NO. OF CHANNELS = 1
     if(mat.type() == CV_8UC1)
     {
         QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
         // Set the color table (used to translate colour indexes to qRgb values)
         image.setColorCount(256);
         for(int i = 0; i < 256; i++)
         {
             image.setColor(i, qRgb(i, i, i));
         }
         // Copy input Mat
         uchar *pSrc = mat.data;
         for(int row = 0; row < mat.rows; row ++)
         {
             uchar *pDest = image.scanLine(row);
             memcpy(pDest, pSrc, mat.cols);
             pSrc += mat.step;
         }
         return image;
     }
     // 8-bits unsigned, NO. OF CHANNELS = 3
     else if(mat.type() == CV_8UC3)
     {
         // Copy input Mat
         const uchar *pSrc = (const uchar*)mat.data;
         // Create QImage with same dimensions as input Mat
         QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
         return image.rgbSwapped();
     }
     else if(mat.type() == CV_8UC4)
     {
         qDebug() << "CV_8UC4";
         // Copy input Mat
         const uchar *pSrc = (const uchar*)mat.data;
         // Create QImage with same dimensions as input Mat
         QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
         return image.copy();
     }
     else
     {
         qDebug() << "ERROR: Mat could not be converted to QImage.";
         return QImage();
     }
}

Mat QImage2cvMat(QImage image)
{
    cv::Mat mat;
    qDebug() << image.format();
    switch(image.format())
    {

    // return error and exit if the QImage is wrong input
    case QImage::Format_Invalid:
        cout << "Wrong QImage" << endl;
        exit(0);
        break;

    // Include all the cases of image.format()
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
    case QImage::Format_RGB16:
    case QImage::Format_ARGB8565_Premultiplied:
    case QImage::Format_RGB666:
    case QImage::Format_ARGB6666_Premultiplied:
    case QImage::Format_RGB555:
    case QImage::Format_ARGB8555_Premultiplied:
    case QImage::Format_RGB444:
    case QImage::Format_ARGB4444_Premultiplied:
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
    case QImage::Format_RGBA8888_Premultiplied:
    case QImage::Format_A2BGR30_Premultiplied:
    case QImage::Format_A2RGB30_Premultiplied:
    case QImage::Format_RGB30:
    case QImage::Format_BGR30:
    case QImage::Format_Grayscale8:
    case QImage::Format_Alpha8:
    case QImage::NImageFormats:
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:

    // for some specific cases use different implementation
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}
