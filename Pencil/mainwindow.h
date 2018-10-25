#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QImage>
#include <QIcon>
#include <QMovie>
#include <QFileDialog>
#include <QFileDevice>
#include <QDesktopWidget>
#include <iostream>
#include <QMouseEvent>
#include <stack>
#include <queue>
#include <QThread>
#include "effect.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void play();

private:
    /*private methods*/
    void wheelEvent(QWheelEvent *w);
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void Initialize_Title();
    void Initialize_GW();
    void Initialize_TW();
    void Initialize_BW();
    void Initialize_Load();
    void Initialize_EW();
    void Initialize_SW();
    void Initialize_MW();
    void Initialize_RW();
    void StylizeButton(QPushButton* button, QString text, QFont ft, int x, int y, int w, int h);
    void StylizeLable(QLabel *label, QString text, Qt::AlignmentFlag alignment, QFont ft, int x, int y, int w, int h);
    void StylizeWidget(QWidget *widget, int x, int y, int w, int h, int r = 255, int g = 245, int b = 250, int a = 255);
    QImage scaleImage(QImage img, int w_limit, int h_limit);
    void displayTmpImage(QImage * tmpI);

    /*private variables*/
    Ui::MainWindow *ui;
    int count;
    int language = 0;
    int mode = -1;
    int Screen_w;
    int Screen_h;
    int MeasureD = 0;
    int MeasureR = 0;
    double crop = -1;

    bool show_style_status = false;
    bool show_edit_status = false;
    bool Drag = false;
    bool zoom = false;
    bool rotate = false;
    bool show_ratio_status= false;
    bool show_mode_status = false;

    queue<QImage> allImages;
    queue<QImage> imagesToShow;

    stack<QImage> unStack;
    stack<QImage> reStack;

    QPoint move_pos;

    QWidget *Greeting_Widget;
    QWidget *Top_Widget;
    QWidget *Bottom_Widget;
    QWidget *language_S;
    QWidget *language_R;
    QWidget *Load;
    QWidget *EditWidget;
    QWidget *StyleWidget;
    QWidget *ResultShowing;
    QWidget *ModeWidget;
    QWidget *RatioWidget;

    QLabel *Name;
    QLabel *MainImage;
    QLabel * result;
    QLabel first;
    QLabel second;
    QLabel third;
    QLabel last;
    QLabel *beforeShowing;
    QLabel *Credit;

    QPushButton *Mini;
    QPushButton *Close;
    QPushButton *Redo;
    QPushButton *Undo;
    QPushButton *Set;
    QPushButton *Begin;
    QPushButton *select_E;
    QPushButton *select_C;
    QPushButton *ChooseFile;
    QPushButton *Menu;
    QPushButton *Cancel;
    QPushButton *Edit;
    QPushButton *Crop;
    QPushButton *Spin;
    QPushButton *Zoom;
    QPushButton *Ratio;
    QPushButton *Free;
    QPushButton *W1H1;
    QPushButton *W16H9;
    QPushButton *W9H16;
    QPushButton *W4H3;
    QPushButton *W3H4;
    QPushButton *Confirm;
    QPushButton *Back;
    QPushButton *AllStyles;
    QPushButton *Style_1;
    QPushButton *Style_2;
    QPushButton *Style_3;
    QPushButton *Style_4;
    QPushButton *Mode;
    QPushButton *Quick;
    QPushButton *Standard;
    QPushButton *Advanced;
    QPushButton *White_1;
    QPushButton *White_2;
    QPushButton *Color;
    QPushButton *Apply;
    QPushButton *Save;

    QFont ft_E_Normal_12;
    QFont ft_C_Normal_12;
    QFont ft_E_Normal_14;
    QFont ft_C_Normal_14;
    QFont ft_E_Normal_16;
    QFont ft_C_Normal_16;
    QFont ft_E_Title_14;
    QFont ft_E_Title_32;
    QFont ft_C_Title_32;

    QPalette wg_Normal;

    QString Path;
    QString ChosenStyle;

    QTimer *delay;

    QImage *img;
    QImage * TmpQI;

public slots:
    void select_language();
    void select_end();
    void change_language();
    void loading();
    void backToMenu();
    void openImage();
    void closeImage();
    void undoing();
    void redoing();
    void editImage();
    void selectRatio();
    void updateImage();
    void nextStep();
    void lastStep();
    void showStyles();
    void selectMode();
    void preApply();
    void applyStyle();
    void showResult_1_2();
    void showResult_3_4();
    void saveAs();
};



#endif // MAINWINDOW_H
