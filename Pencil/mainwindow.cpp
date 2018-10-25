#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "choose_cut.h"
#include "effect.h"
#include "training.h"
#include "arc.h"
#include "cartoon.h"
#include "math.h"
/* First
 * ————————————————————————————————————
 * |---------------------------------------------------------------------------|
 * |                                                                                                         |
 * |                                                                                                         |
 * |                                                                                                         |
 * |  ——————————————————————————————————  |
 * |  |                                                                                                  |  |
 * |  |                                     UNTITLED                                           |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  |                          Begin                          Set                                |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  ——————————————————————————————————  |
 * |                                                                                                         |
 * |                                                                                                         |
 * |                                                                                                         |
 * |                                                                                                         |
 * ————————————————————————————————————
 * Second
 * ————————————————————————————————————
 * |  ——————————————————————————————————  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  |                                     Open Image                                          |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  |                                                                                                  |  |
 * |  ——————————————————————————————————  |
 * |  ——————————————————————————————————  |
 * |  |                                                                                                  |  |
 * |  |       Menu       Cancel        Edit       Choose Ratio       Continue    |  |
 * |  |                                                                                                  |  |
 * |  ——————————————————————————————————  |
 * ————————————————————————————————————
*/

/*
 * Constructor: MainWindow
 * --------------------------------------------------------------------------------------------------
 * First set up some basic settings of the main window:  set the framless window style the
 * window icon and the palette and stylesheet of the main window , take out the menu bar,
 * detect the screen resolution then scale the size of the main window and the size of fonts
 * in order to adapt to different screens of different computers, and set pattern to several
 * common used fonts.
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMenuBar(NULL);
    this->setWindowIcon(QIcon(":/new/Images/Texture/brush-128.png"));
    QPalette this_pal = this->palette();
    this_pal.setColor(QPalette::WindowText, Qt::black);
    this_pal.setColor(QPalette::Window, QColor(255,220,226,255));
    this_pal.setColor(QPalette::Base, Qt::white);
    this->setPalette(this_pal);
    this->setStyleSheet("QPushButton{background-color:white}");
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    Screen_w = deskRect.width();
    Screen_w *= 0.7;
    Screen_h = deskRect.height();
    Screen_h *= 0.95;
    this -> move(Screen_w*1.5/7, Screen_h *0.25/9.5);
    this->setFixedSize(Screen_w, Screen_h);

    QTextCodec:: setCodecForLocale(QTextCodec:: codecForName("UTF8"));
    int base_font_size = pow(Screen_w, 1.0/3.0) ;
    ft_E_Normal_14.setFamily("Calibri");
    ft_E_Normal_14.setPointSize(base_font_size + 1);
    ft_C_Normal_14.setFamily("幼圆");
    ft_C_Normal_14.setPointSize(base_font_size);
    ft_E_Normal_16.setFamily("Calibri");
    ft_E_Normal_16.setPointSize(base_font_size + 3);
    ft_C_Normal_16.setFamily("幼圆");
    ft_C_Normal_16.setPointSize(base_font_size + 2);
    ft_E_Title_14.setFamily("Calibri");
    ft_E_Title_14.setBold(true);
    ft_E_Title_14.setPointSize(base_font_size + 3);
    ft_E_Title_32.setFamily("Calibri");
    ft_E_Title_32.setBold(true);
    ft_E_Title_32.setPointSize(base_font_size +14);
    ft_C_Title_32.setFamily("幼圆");
    ft_C_Title_32.setBold(true);
    ft_C_Title_32.setPointSize(base_font_size +11);
}

/*Destructor: ~MainWindow
------------------------------------------------------------------------------
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Method: play
 * Usage: w.play();
 * ------------------------------------------------------------------------------
 * play() integrets the initializations of all subwidgets and the labels and pushbuttons
 * inside the subwidgets. play() is the only public method because, as an interface, most
 * of the methods will be called when the user gives order by clicking button, pressing
 * at a special area or special key and scrolling the wheel.
 */
void MainWindow::play(){
    Credit = new QLabel(this);
    StylizeLable(Credit, "Credit:\nJiang Yangyadi\nYuan Changcheng\nZeng Yuchen\nZhang Hongbo", Qt::AlignLeft, ft_E_Normal_16, Screen_w * 0.05, Screen_h * 0.77, Screen_w * 0.4, Screen_h * 0.3);
    Credit->show();
    Initialize_Title();
    Initialize_GW();
    Initialize_TW();
    Initialize_BW();
    Initialize_Load();
    Initialize_EW();
    Initialize_RW();
    Initialize_SW();
    Initialize_MW();

    this->show();
}

/*
 * Method: wheelEvent
 * ------------------------------------------------------------------------------
 * wheelEvent will detect the wheel rolling event . This method is specially
 * for when the  zoom editing method or the spin editing method is triggered.
 * It can read the wheel rolling value to the function inside this method and
 * do the operation based on the rolling value.
 */
void MainWindow::wheelEvent(QWheelEvent *w) {
    if (zoom) {
        MeasureD = w->delta() + MeasureD;
        Choose_Cut zoomPic = Choose_Cut(img, Screen_h*0.78, Screen_w*0.96);
        zoomPic.change(MeasureD);

        if (zoomPic.getBigFlag() || zoomPic.getTinyFlag()) {
            MeasureD = MeasureD - w->delta();
        }
        zoomPic.change(MeasureD);
        zoomPic.confirmPic();
        TmpQI = new QImage;
        *TmpQI = zoomPic.getQImage();
        displayTmpImage(TmpQI);
    }

    if (rotate) {
        MeasureR = w->delta() + MeasureR;

        Choose_Cut rotatePic = Choose_Cut(img, Screen_h*0.78, Screen_w*0.96);
        rotatePic.rotate(MeasureR);
        rotatePic.confirmPic();
        TmpQI = new QImage;
        *TmpQI = rotatePic.getQImage();
        displayTmpImage(TmpQI);
    }
}

/*
 * Method:keyPressEvent
 * ------------------------------------------------------------------------------
 * keyPressEvent will detect the key pressing and response to it. In this
 * function, only pressing Enter will be responsed. it will confirm the picture
 * after rotating and zooming and reset the variables used in rotating and
 * zooming.
 */
void MainWindow::keyPressEvent(QKeyEvent *e) {
    // trigger enter
    if (e->key() == Qt::Key_Return) {
        //check the zoom state
        if (zoom) {
            zoom = false; //turn off zoom state
            if (MeasureD == 0) unStack.push(*img);
            else {
                unStack.push(*TmpQI); //push in the result
                TmpQI = NULL; //reset the pointer
            }
            *img = unStack.top();//reset the dst image
            updateImage();

            Menu->setEnabled(true);
            Cancel->setEnabled(true);
            Edit->setEnabled(true);
            Confirm->setEnabled(true);
            Undo->setEnabled(true);
            Redo->setEnabled(true);
        }
        //check the rotate state
        if (rotate) {
            rotate = false;
            if (MeasureR == 0) unStack.push(*img);
            else {
                unStack.push(*TmpQI); //push in the result
                TmpQI = NULL; //reset the pointer
            }
            *img = unStack.top();//reset the dst image
            updateImage();

            Menu->setEnabled(true);
            Cancel->setEnabled(true);
            Edit->setEnabled(true);
            Confirm->setEnabled(true);
            Undo->setEnabled(true);
            Redo->setEnabled(true);
        }
    }
}


/*
 * Method: mousePressEvent
 * ------------------------------------------------------------------------------
 * mousePressEvent() will detect the mouse press event. This method is
 * specially for when the left button of the mouse is pressed on the title
 * area which indicates the user wants to move the main window, and will
 * record the initial mouse position.
 */
void MainWindow::mousePressEvent(QMouseEvent *e){
    QRect title(0,0,Screen_w, Screen_h*0.05);
    if(e->button() == Qt::LeftButton && title.contains(e->globalPos() - this->pos())) {
        Drag = true;
        move_pos = e->globalPos() - this->pos();
        e->accept();
    }
}

/*
 * Method: mouseMoveEvent
 * ------------------------------------------------------------------------------
 * mouseMoveEvent() will detect the mouse move event. This method is
 * specially for when the user are moving the main window, and will
 * gain the displacement of the main window by comparing the present
 * mouse position to the initial mouse position recorded in the above method.
 */
void MainWindow::mouseMoveEvent(QMouseEvent *e){
    if(Drag && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - move_pos);
        e->accept();
    }
}

/*
 * Method: mouseReleaseEvent
 * ------------------------------------------------------------------------------
 * mouseMoveEvent() will detect the mouse release event. This method is
 * specially for when the user finishes the moving of the main window.
 */
void MainWindow::mouseReleaseEvent(QMouseEvent *){
    Drag = false;
}

/*
 * Method: Initialize_Title
 * Usage: Initialize_Title();
 * ------------------------------------------------------------------------------
 * Initialize_Title() is a private method that intializes all variables locating on
 * the title area, sets the pattern and content of varaibles and builds up connection
 * among senders like buttons, signals, receivers and slots.
 */
void MainWindow::Initialize_Title(){
    QLabel * Title_text = new QLabel(this);
    StylizeLable(Title_text, "Pencil            Brush", Qt::AlignCenter, ft_E_Title_14, 0, 0, Screen_w, Screen_h*0.05);
    Title_text->setStyleSheet("background-color: white");
    QLabel * Title_icon = new QLabel(this);
    QImage icon_Image = QImage(":/new/Images/Texture/brush-128.png");
    icon_Image = icon_Image.scaled(Screen_h*0.04, Screen_h*0.04);
    Title_icon->setPixmap(QPixmap::fromImage(icon_Image));
    Title_icon->setAlignment(Qt::AlignCenter);
    Title_icon->setGeometry(0, 0, Screen_w, Screen_h*0.05);

    Close = new QPushButton(this);
    Close->setGeometry(Screen_w-40, (Screen_h*0.05-32)/2, 32, 32);
    QIcon close_Icon (":/new/Images/Texture/close.png");
    Close->setStyleSheet("background-color: transparent");
    Close->setAutoFillBackground(true);
    Close->setFlat(true);
    Close->setIcon(close_Icon);
    QObject::connect(Close, SIGNAL(clicked(bool)), this, SLOT(close()));

    Mini = new QPushButton(this);
    Mini->setGeometry(Screen_w-80, (Screen_h*0.05-32)/2, 32, 32);
    QIcon mini_Icon (":/new/Images/Texture/minimize.png");
    Mini->setStyleSheet("background-color: transparent");
    Mini->setAutoFillBackground(true);
    Mini->setFlat(true);
    Mini->setIcon(mini_Icon);
    QObject::connect(Mini, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));

    Undo = new QPushButton(this);
    Undo->setGeometry(12, (Screen_h*0.05-32)/2, 32, 32);
    QIcon undo_Icon (":/new/Images/Texture/left.png");
    Undo->setStyleSheet("background-color: transparent");
    Undo->setAutoFillBackground(true);
    Undo->setFlat(true);
    Undo->setIcon(undo_Icon);
    Undo->hide();
    QObject::connect(Undo, SIGNAL(clicked(bool)), this, SLOT(undoing()));

    Redo = new QPushButton(this);
    Redo->setGeometry(50, (Screen_h*0.05-32)/2, 32, 32);
    QIcon redo_Icon (":/new/Images/Texture/right.png");
    Redo->setStyleSheet("background-color: transparent");
    Redo->setAutoFillBackground(true);
    Redo->setFlat(true);
    Redo->setIcon(redo_Icon);
    Redo->hide();
    QObject::connect(Redo, SIGNAL(clicked(bool)), this, SLOT(redoing()));
}

/*
 * Method: Initialize_GW
 * Usage: Initialize_GW();
 * ------------------------------------------------------------------------------
 * Initialize_GW() is a private method that initializes all variables locating
 * on the greeting area, sets the pattern and content of varaibles and builds
 * up connection among senders like buttons, signals, receivers and slots.
 */
void MainWindow:: Initialize_GW(){
    Greeting_Widget = new QWidget(this);
    StylizeWidget(Greeting_Widget, Screen_w*0.05, Screen_h*0.3, Screen_w*0.9, Screen_h*0.4);

    int font_size = ft_E_Title_32.pointSize();
    ft_E_Title_32.setPointSize(font_size + 17);
    Name = new QLabel(Greeting_Widget);
    StylizeLable(Name, "Pencil Brush", Qt::AlignCenter,ft_E_Title_32, 0, Screen_h*0.075, Screen_w*0.9, Screen_h*0.1);
    ft_E_Title_32.setPointSize(font_size);

    Begin = new QPushButton(Greeting_Widget);
    StylizeButton(Begin, "Begin", ft_E_Title_32, Screen_w*0.2, Screen_h*0.225, Screen_w*0.15, Screen_h*0.1);

    Set = new QPushButton(Greeting_Widget);
    StylizeButton(Set, "Set", ft_E_Title_32, Screen_w*0.55, Screen_h*0.225, Screen_w*0.15, Screen_h*0.1);

    QObject::connect(Set, SIGNAL(clicked(bool)), this, SLOT(select_language()));
    QObject::connect(Begin, SIGNAL(clicked(bool)), this, SLOT(loading()));
    QObject::connect(Begin, SIGNAL(clicked(bool)), Credit, SLOT(hide()));
}

/*
 * Method: Initialize_TW
 * Usage: Initialize_TW();
 * ------------------------------------------------------------------------------
 * Initialize_TM() is a private method that initializes all variables locating on
 * the central top area, sets the pattern and content of varaibles and builds up
 * connection among senders like buttons, signals, receivers and  slots.
 */
void MainWindow:: Initialize_TW(){
    Top_Widget = new QWidget(this);
    StylizeWidget(Top_Widget, Screen_w*0.01, Screen_h*0.06, Screen_w*0.98, Screen_h*0.8);


    ChooseFile = new QPushButton(Top_Widget);
    StylizeButton(ChooseFile, "Open Image", ft_E_Normal_16, Screen_w*0.39, Screen_h*0.37, Screen_w*0.2, Screen_h*0.06);
    QObject::connect(ChooseFile, SIGNAL(clicked(bool)), this, SLOT(openImage()));

    Top_Widget->hide();
}

/*
 * Method: Initialize_BW
 * Usage: Initialize_BW();
 * ------------------------------------------------------------------------------
 * Initialize_CWM() is a private method that initializes all variables locating on
 * the central bottom area, sets the pattern and content of varaibles and builds
 * up connection among senders like buttons, signals, receivers and slots.
 */
void MainWindow:: Initialize_BW(){
    Bottom_Widget = new QWidget(this);
    StylizeWidget(Bottom_Widget, Screen_w*0.01, Screen_h*0.87, Screen_w*0.98, Screen_h*0.12);
/*eeeeeeeeeeedit*/
    result = new QLabel(Bottom_Widget);
    StylizeLable(result, " ", Qt::AlignCenter, ft_E_Normal_14, Screen_w*0.65, 0, Screen_w*0.19, Screen_h*0.12);

    Menu = new QPushButton(Bottom_Widget);
    StylizeButton(Menu, "Menu", ft_E_Normal_16, Screen_w*0.06, Screen_h*0.03, Screen_w*0.1, Screen_h*0.06);

    Cancel = new QPushButton(Bottom_Widget);
    StylizeButton(Cancel, "Delete", ft_E_Normal_16, Screen_w*0.21, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
    Cancel->setEnabled(false);

    Edit  = new QPushButton(Bottom_Widget);
    StylizeButton(Edit, "Edit", ft_E_Normal_16, Screen_w*0.38, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
    Edit->setEnabled(false);

    Ratio = new QPushButton(Bottom_Widget);
    StylizeButton(Ratio, "-- Choose Ratio --", ft_E_Normal_16, Screen_w*0.55, Screen_h*0.03, Screen_w*0.19, Screen_h*0.06);
    Ratio->hide();

    Confirm = new QPushButton(Bottom_Widget);
    StylizeButton(Confirm, "Continue", ft_E_Normal_16, Screen_w*0.79, Screen_h*0.03, Screen_w*0.13, Screen_h*0.06);
    Confirm->setEnabled(false);

    Back = new QPushButton(Bottom_Widget);
    StylizeButton(Back, "Back", ft_E_Normal_16, Screen_w*0.2, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
    Back->hide();

    AllStyles = new QPushButton(Bottom_Widget);
    StylizeButton(AllStyles, "-- All Styles --", ft_E_Normal_16, Screen_w*0.33, Screen_h*0.03, Screen_w*0.15, Screen_h*0.06);
    AllStyles->hide();

    Mode = new QPushButton(Bottom_Widget);
    StylizeButton(Mode, "Mode", ft_E_Normal_16, Screen_w*0.52, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
    Mode->hide();

    Apply = new QPushButton(Bottom_Widget);
    StylizeButton(Apply, "Apply", ft_E_Normal_16, Screen_w*0.85, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
    Apply->setEnabled(false);
    Apply->hide();

    Save = new QPushButton(Bottom_Widget);
    StylizeButton(Save, "Save As", ft_E_Normal_16, Screen_w*0.85, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
    Save->hide();

    QObject::connect(Menu, SIGNAL(clicked(bool)), this, SLOT(backToMenu()));
    QObject::connect(Cancel, SIGNAL(clicked(bool)), this, SLOT(closeImage()));
    QObject::connect(Edit, SIGNAL(clicked(bool)), this, SLOT(editImage()));
    QObject::connect(Ratio, SIGNAL(clicked(bool)), this, SLOT(selectRatio()));
    QObject::connect(Confirm, SIGNAL(clicked(bool)), this, SLOT(nextStep()));
    QObject::connect(Back, SIGNAL(clicked(bool)), this, SLOT(lastStep()));
    QObject::connect(Mode, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    QObject::connect(AllStyles, SIGNAL(clicked(bool)), this, SLOT(showStyles()));
    QObject::connect(Apply, SIGNAL(clicked(bool)), this, SLOT(preApply()));
    QObject::connect(Save, SIGNAL(clicked(bool)), this, SLOT(saveAs()));
    Bottom_Widget->hide();
}

/*
 * Method: Initialize_Load
 * Usage: Initialize_Load();
 * ------------------------------------------------------------------------------
 * Initialize_Load() is a private method that initializes all variables locating on
 * the loading page and sets the pattern and content of varaibles.
 */
void MainWindow:: Initialize_Load(){
    Load = new QWidget(this);
    StylizeWidget(Load, 0, Screen_h*0.05, Screen_w, Screen_h*0.95, 255, 240, 246, 180);

    QLabel *loading_1 = new QLabel(Load);
    QImage *image = new QImage(":/new/Images/Texture/brush-gray.png");
    *image = image->scaled(100,100,Qt::KeepAspectRatio);
    loading_1->setAlignment(Qt::AlignCenter);
    loading_1->setPixmap(QPixmap::fromImage(*image));
    loading_1->setGeometry((Screen_w-100)/2, (Screen_h-120)/2, 100, 100);

    QLabel *loading_2 = new QLabel(Load);
    QMovie *gif = new QMovie(":/new/Images/Texture/loadingGif.gif");
    gif->start();
    loading_2->setMovie(gif);
    loading_2->setAlignment(Qt::AlignCenter);
    loading_2->move((Screen_w-100)/2, Screen_h/2+55);

    Load->hide();

    ResultShowing = new QWidget(this);
    StylizeWidget(ResultShowing, 0, Screen_h*0.05, Screen_w, Screen_h*0.95, 255, 240, 246, 220);

    beforeShowing = new QLabel(ResultShowing);
    StylizeLable(beforeShowing, "Your picture is processing...", Qt::AlignCenter, ft_E_Title_32, 0, 0, Screen_w, Screen_h*0.85);

    ResultShowing->hide();
}

/*
 * Method: Initialize_EW
 * Usage: Initialize_EW();
 * ------------------------------------------------------------------------------
 * Initialize_EW() is a private method that initializes all variables locating on
 * the edit widget that will pop out when the user want to edit the image, sets
 * the pattern and content of varaibles and builds up connection among sender
 *  buttons, signals, receivers and slots.
 */
void MainWindow::Initialize_EW(){
    EditWidget = new QWidget(this);
    StylizeWidget(EditWidget, Screen_w*0.39, Screen_h*0.68, Screen_w*0.12, Screen_h*0.22, 255, 230, 236, 120);

    Crop = new QPushButton(EditWidget);
    StylizeButton(Crop, "Crop", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);

    Spin = new QPushButton(EditWidget);
    StylizeButton(Spin, "Spin", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);

    Zoom = new QPushButton(EditWidget);
    StylizeButton(Zoom, "Zoom\nIn / Out", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.08);

    QObject::connect(Crop, SIGNAL(clicked(bool)), this, SLOT(editImage()));
    QObject::connect(Spin, SIGNAL(clicked(bool)), this, SLOT(editImage()));
    QObject::connect(Zoom, SIGNAL(clicked(bool)), this, SLOT(editImage()));

    EditWidget->hide();
}

/*
 * Method: Initialize_RW
 * Usage: Initialize_RW();
 * ------------------------------------------------------------------------------
 * Initialize_RW() is a private method that initializes all variables locating on
 * the ratio widget that will pop out when the user need to select ratio for the
 * image cropping, sets the pattern and content of varaibles and builds up
 * connection among sender buttons, signals, receivers and slots.
 */
void MainWindow:: Initialize_RW(){
    RatioWidget = new QWidget(this);
    StylizeWidget(RatioWidget, Screen_w*0.56, Screen_h*0.74 , Screen_w*0.19, Screen_h*0.16, 255, 230, 236, 120);

    Free = new QPushButton(RatioWidget);
    StylizeButton(Free, "Free", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.08, Screen_h*0.04);

    W1H1 = new QPushButton(RatioWidget);
    StylizeButton(W1H1, "1 : 1", ft_E_Normal_14, Screen_w*0.1, Screen_h*0.01, Screen_w*0.08, Screen_h*0.04);

    W16H9 = new QPushButton(RatioWidget);
    StylizeButton(W16H9, "16 : 9", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.06, Screen_w*0.08, Screen_h*0.04);

    W9H16 = new QPushButton(RatioWidget);
    StylizeButton(W9H16, "9 : 16", ft_E_Normal_14, Screen_w*0.1, Screen_h*0.06, Screen_w*0.08, Screen_h*0.04);

    W4H3 = new QPushButton(RatioWidget);
    StylizeButton(W4H3, "4 : 3", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.11, Screen_w*0.08, Screen_h*0.04);

    W3H4 = new QPushButton(RatioWidget);
    StylizeButton(W3H4, "3 : 4", ft_E_Normal_14, Screen_w*0.1, Screen_h*0.11, Screen_w*0.08, Screen_h*0.04);

    QObject::connect(Free, SIGNAL(clicked(bool)), this, SLOT(selectRatio()));
    QObject::connect(W1H1, SIGNAL(clicked(bool)), this, SLOT(selectRatio()));
    QObject::connect(W16H9, SIGNAL(clicked(bool)), this, SLOT(selectRatio()));
    QObject::connect(W9H16, SIGNAL(clicked(bool)), this, SLOT(selectRatio()));
    QObject::connect(W4H3, SIGNAL(clicked(bool)), this, SLOT(selectRatio()));
    QObject::connect(W3H4, SIGNAL(clicked(bool)), this, SLOT(selectRatio()));

    RatioWidget->hide();
}

/*
 * Method: Initialize_SW
 * Usage: Initialize_SW();
 * ------------------------------------------------------------------------------
 * Initialize_SW() is a private method that initializes all variables locating on
 * the style widget that will pop out when the user want to select styles for
 * the image, sets the pattern and content of varaibles and builds up connection
 * among sender buttons, signals, receivers and slots.
 */
void MainWindow:: Initialize_SW(){
    StyleWidget = new QWidget(this);
    StylizeWidget(StyleWidget, Screen_w*0.34, Screen_h*0.65, Screen_w*0.15, Screen_h*0.25, 255, 230, 236, 120);

    Style_1 = new QPushButton(StyleWidget);
    StylizeButton(Style_1, "Style 1", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.13, Screen_h*0.05);

    Style_2 = new QPushButton(StyleWidget);
    StylizeButton(Style_2, "Style 2", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.13, Screen_h*0.05);

    Style_3 = new QPushButton(StyleWidget);
    StylizeButton(Style_3, "Style 3", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.13, Screen_h*0.05);

    Style_4 = new QPushButton(StyleWidget);
    StylizeButton(Style_4, "Style 4", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.19, Screen_w*0.13, Screen_h*0.05);

    QObject::connect(Style_1, SIGNAL(clicked(bool)), this, SLOT(showStyles()));
    QObject::connect(Style_2, SIGNAL(clicked(bool)), this, SLOT(showStyles()));
    QObject::connect(Style_3, SIGNAL(clicked(bool)), this, SLOT(showStyles()));
    QObject::connect(Style_4, SIGNAL(clicked(bool)), this, SLOT(showStyles()));

    StyleWidget->hide();
}

/*
 * Method: Initialize_MW
 * Usage: Initialize_MW();
 * ------------------------------------------------------------------------------
 * Initialize_MW() is a private method that initializes all variables locating on
 * the mode widget that will pop out when the user need to select specific mode
 * of some styles, sets the pattern and content of varaibles and builds up connection
 * among sender buttons, signals, receivers and slots.
 */
void MainWindow::Initialize_MW(){
    ModeWidget = new QWidget(this);
    StylizeWidget(ModeWidget, Screen_w*0.53, Screen_h*0.71, Screen_w*0.12, Screen_h*0.19, 255, 230, 236, 120);

    Quick = new QPushButton(ModeWidget);
    StylizeButton(Quick, "Quick", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);

    Standard = new QPushButton(ModeWidget);
    StylizeButton(Standard, "Standard *", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);

    Advanced = new QPushButton(ModeWidget);
    StylizeButton(Advanced, "Advanced", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.05);

    White_1 = new QPushButton(ModeWidget);
    StylizeButton(White_1, "B-W 1", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);
    White_1->hide();

    White_2 = new QPushButton(ModeWidget);
    StylizeButton(White_2, "B-W 2", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);
    White_2->hide();

    Color = new QPushButton(ModeWidget);
    StylizeButton(Color, "Color", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.05);
    Color -> hide();

    QObject::connect(Quick, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    QObject::connect(Standard, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    QObject::connect(Advanced, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    QObject::connect(White_1, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    QObject::connect(White_2, SIGNAL(clicked(bool)), this, SLOT(selectMode()));
    QObject::connect(Color, SIGNAL(clicked(bool)), this, SLOT(selectMode()));

    ModeWidget->hide();
}

/*
 * Method: StylizeLabel
 * Usage: StylizeLable(label, "hahaaha", Qt::AlignCenter, ft_E_Normal_16, 0, 0, 20, 30);
 * ------------------------------------------------------------------------------
 * StylizeLable() is a private methods that serves as a tool to elimate the lines
 * needed to set up for a single label.
 */
void MainWindow:: StylizeLable(QLabel *label, QString text, Qt::AlignmentFlag alignment, QFont ft, int x, int y, int w, int h){
    label->setText(text);
    label->setFont(ft);
    label->setAlignment(alignment);
    label->setGeometry(x,y,w,h);
}

/*
 * Method: StylizeButton
 * Usage: StylizeButton(button, "hahaaha", ft_E_Normal_16, 0, 0, 20, 30);
 * ------------------------------------------------------------------------------
 * Stylizebutton() is a private methods that serves as a tool to elimate the lines
 * needed to set up for a single button.
 */
void MainWindow:: StylizeButton(QPushButton* button, QString text, QFont ft, int x, int y, int w, int h){
    button->setText(text);
    button->setFont(ft);
    button->setGeometry(x,y,w,h);
}

/*
 * Method: StylizeWidget
 * Usage: StylizeWidget(Widget, 0, 0, 20, 30, 255, 255, 255, 255);
 * ------------------------------------------------------------------------------
 * StylizeWidget() is a private methods that serves as a tool to elimate the lines
 * needed to set up for a single widget.
 */
void MainWindow:: StylizeWidget(QWidget *widget, int x, int y, int w, int h, int r, int g, int b, int a){
    widget->setGeometry(x,y,w,h);
    QPalette pal;
    pal.setColor(QPalette::Window, QColor(r, g, b, a));
    widget->setAutoFillBackground(true);
    widget->setPalette(pal);
}

/*
 * Method: scaleImage
 * Usage: scaleImage(image, 1280, 960);
 * ------------------------------------------------------------------------------
 * StylizeWidget() is a private methods that serves as a tool to scale
 * the image to adapt to the showing window because the window size
 * is fixed and sometimes different and the image size is also different.
 */
QImage MainWindow::scaleImage(QImage img, int w_limit, int h_limit){
    int w = img.width();
    int h = img.height();
    while (w > w_limit || h > h_limit){
        if (w > w_limit){
            h = h * w_limit / w;
            w = w_limit;
        }
        if( h > h_limit){
            w = w*h_limit/h;
            h = h_limit;
        }
    }
    return img.scaled(w,h);
}

/*
 * Method: displayTmpImage
 * Usage:displayTmpImage(tmpI)
 * ------------------------------------------------------------------------------
 * this method is used inside the class to display the temporary Image generated by the
 * editing method,
 */
void MainWindow::displayTmpImage(QImage * tmpI) {
    MainImage->setPixmap(QPixmap::fromImage(*tmpI));
}

/*
 * Method: select_language
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(select_language()));
 * ------------------------------------------------------------------------------
 * select_language() is a private slot that will be called when the set button
 * in the greeting page is clicked. This method will let the user to choose
 * English or Chinese as the language of the program. Once one language selecting
 * button was clicked, the select_end() will be called.
 */
void MainWindow:: select_language(){
    Credit->hide();
    language_S = new QWidget(this);
    StylizeWidget(language_S, Screen_w*0.05, Screen_h*0.3, Screen_w*0.9, Screen_h*0.4);

    QLabel *Ask_C = new QLabel(language_S);
    StylizeLable(Ask_C, "你好，请选择语言", Qt::AlignCenter, ft_E_Normal_16, 0, Screen_h*0.06, Screen_w*0.9, Screen_h*0.06);

    QLabel *Ask_E = new QLabel(language_S);
    StylizeLable(Ask_E, "Hello, Please Select Language", Qt::AlignCenter, ft_E_Normal_16, 0, Screen_h*0.12, Screen_w*0.9, Screen_h*0.06);

    select_E = new QPushButton(language_S);
    StylizeButton(select_E,"English",ft_E_Normal_16, Screen_w*0.2, Screen_h*0.225, Screen_w*0.15, Screen_h*0.1);

    select_C = new QPushButton(language_S);
    StylizeButton(select_C,"中文",ft_C_Normal_16, Screen_w*0.55, Screen_h*0.225, Screen_w*0.15, Screen_h*0.1);

    language_S->show();

    connect(select_E, SIGNAL(clicked(bool)), this, SLOT(select_end()));
    connect(select_C, SIGNAL(clicked(bool)), this, SLOT(select_end()));
}

/*
 * Method: select_end
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(select_end()));
 * ------------------------------------------------------------------------------
 * select_end() is a private slot that will be called when the language selecting
 * button (English/中文) is clicked. This method will show user which language
 * is selected and call the change_language() to change the text language of all
 * the components in this programme if the chosen language differs from the
 * current language.
 */
void MainWindow:: select_end(){
    QObject *sender = this->sender();
    language_S->close();

    language_R = new QWidget(this);
    StylizeWidget(language_R, Screen_w*0.05, Screen_h*0.3, Screen_w*0.9, Screen_h*0.4 );

    delay = new QTimer;
    delay->setInterval(1000);
    QObject::connect(delay, SIGNAL(timeout()), Credit, SLOT(show()));
    QObject::connect(delay,SIGNAL(timeout()), language_R, SLOT(close()));
    QObject::connect(delay, SIGNAL(timeout()), delay, SLOT(deleteLater()));

    if ( select_E == sender){
        QLabel *E_selected = new QLabel(language_R);
        StylizeLable(E_selected, "You chose English", Qt::AlignCenter, ft_E_Title_32, 0, 0, Screen_w*0.9, Screen_h*0.4);
        language_R->show();
        QObject::connect(delay,SIGNAL(timeout()),E_selected,SLOT(close()));
        if (language == 1 ){
            language = 0;
            QObject::connect(delay,SIGNAL(timeout()),this, SLOT(change_language()));
        }
    }else{
        QLabel *C_selected = new QLabel(language_R);
        StylizeLable(C_selected, "你选择了中文", Qt::AlignCenter, ft_C_Title_32, 0, 0, Screen_w*0.9, Screen_h*0.4);
        language_R->show();
        QObject::connect(delay,SIGNAL(timeout()),C_selected,SLOT(close()));
        if (language == 0 ){
            language = 1;
            QObject::connect(delay,SIGNAL(timeout()),this, SLOT(change_language()));
        }
    }

    delay->start();
}

/*
 * Method: change_language
 * Usage: QObject::connect(timer, SIGNAL(timeout()), this, SLOT(change_language()));
 * ------------------------------------------------------------------------------
 * change_language is a private slot that will be called when the chosen language
 * differs from the current language to change the text language of all the
 * components in this programme.
 */
void MainWindow::change_language(){
    if (language == 0){
        StylizeButton(Begin, "Begin", ft_E_Title_32, Screen_w*0.2, Screen_h*0.225, Screen_w*0.15, Screen_h*0.1);
        StylizeButton(Set, "Set", ft_E_Title_32, Screen_w*0.55, Screen_h*0.225, Screen_w*0.15, Screen_h*0.1);
        StylizeButton(ChooseFile, "Open Image", ft_E_Normal_16, Screen_w*0.39, Screen_h*0.37, Screen_w*0.2, Screen_h*0.06);
        StylizeButton(Menu, "Menu", ft_E_Normal_16, Screen_w*0.06, Screen_h*0.03, Screen_w*0.1, Screen_h*0.06);
        StylizeButton(Cancel, "Delete", ft_E_Normal_16, Screen_w*0.21, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
        StylizeButton(Edit, "Edit", ft_E_Normal_16, Screen_w*0.38, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
        StylizeButton(Crop, "Crop", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Spin, "Spin", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Zoom, "Zoom\nIn/Out", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.08);
        StylizeButton(Ratio, "-- Choose Ratio --", ft_E_Normal_16, Screen_w*0.55, Screen_h*0.03, Screen_w*0.19, Screen_h*0.06);
        StylizeButton(Free, "Free", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.08, Screen_h*0.04);
        StylizeButton(Confirm, "Continue", ft_E_Normal_16, Screen_w*0.79, Screen_h*0.03, Screen_w*0.13, Screen_h*0.06);
        StylizeButton(Back, "Back", ft_E_Normal_16, Screen_w*0.2, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
        StylizeButton(AllStyles, "-- All Styles --", ft_E_Normal_16, Screen_w*0.33, Screen_h*0.03, Screen_w*0.15, Screen_h*0.06);
        StylizeButton(Style_1, "Style 1", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.13, Screen_h*0.05);
        StylizeButton(Style_2, "Style 2", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.13, Screen_h*0.05);
        StylizeButton(Style_3, "Style 3", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.13, Screen_h*0.05);
        StylizeButton(Style_4, "Style 4", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.19, Screen_w*0.13, Screen_h*0.05);
        StylizeButton(Mode, "Mode", ft_E_Normal_16, Screen_w*0.52, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
        StylizeButton(Quick, "Quick", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Standard, "Standard *", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Advanced, "Advanced", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Apply, "Apply", ft_E_Normal_16, Screen_w*0.85, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
        StylizeButton(Save, "Save As", ft_E_Normal_16, Screen_w*0.85, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
        StylizeLable(beforeShowing, "Your picture is processing...", Qt::AlignCenter, ft_E_Title_32, 0, 0, Screen_w, Screen_h*0.85);
        StylizeLable(Credit, "Credit:\nJiang Yangyadi\nYuan Changcheng\nZeng Yuchen\nZhang Hongbo", Qt::AlignLeft, ft_E_Normal_16, Screen_w * 0.05, Screen_h * 0.77, Screen_w * 0.4, Screen_h * 0.3);
        StylizeButton(White_1, "B-W 1", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(White_2, "B-W 2", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Color, "Color", ft_E_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.05);

    }else{
        StylizeButton(Begin, "开始", ft_C_Title_32, Screen_w*0.2, Screen_h*0.225, Screen_w*0.15, Screen_h*0.1);
        StylizeButton(Set, "设置", ft_C_Title_32, Screen_w*0.55, Screen_h*0.225, Screen_w*0.15, Screen_h*0.1);
        StylizeButton(ChooseFile, "打开图片", ft_C_Normal_16, Screen_w*0.39, Screen_h*0.37, Screen_w*0.2, Screen_h*0.06);
        StylizeButton(Menu, "菜单", ft_C_Normal_16, Screen_w*0.06, Screen_h*0.03, Screen_w*0.1, Screen_h*0.06);
        StylizeButton(Cancel, "重选", ft_C_Normal_16, Screen_w*0.21, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
        StylizeButton(Edit, "编辑", ft_C_Normal_16, Screen_w*0.38, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
        StylizeButton(Crop, "剪切", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Spin, "旋转", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Zoom, "更改\n尺寸", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.08);
        StylizeButton(Ratio, "--选择比例--", ft_C_Normal_16, Screen_w*0.55, Screen_h*0.03, Screen_w*0.19, Screen_h*0.06);
        StylizeButton(Free, "自由", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.08, Screen_h*0.04);
        StylizeButton(Confirm, "下一步", ft_C_Normal_16, Screen_w*0.79, Screen_h*0.03, Screen_w*0.14, Screen_h*0.06);
        StylizeButton(Back, "上一步", ft_C_Normal_16, Screen_w*0.2, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
        StylizeButton(AllStyles, "--全部风格--", ft_C_Normal_16, Screen_w*0.33, Screen_h*0.03, Screen_w*0.15, Screen_h*0.06);
        StylizeButton(Style_1, "风格 1", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.13, Screen_h*0.05);
        StylizeButton(Style_2, "风格 2", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.13, Screen_h*0.05);
        StylizeButton(Style_3, "风格 3", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.13, Screen_h*0.05);
        StylizeButton(Style_4, "风格 4", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.19, Screen_w*0.13, Screen_h*0.05);
        StylizeButton(Mode, "模式", ft_C_Normal_16, Screen_w*0.52, Screen_h*0.03, Screen_w*0.12, Screen_h*0.06);
        StylizeButton(Quick, "快速", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Standard, "标准 *", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Advanced, "高级", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Apply, "应用", ft_C_Normal_16, Screen_w*0.85, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
        StylizeButton(Save, "保存为", ft_C_Normal_16, Screen_w*0.85, Screen_h*0.03, Screen_w*0.09, Screen_h*0.06);
        StylizeLable(beforeShowing, "精彩结果即将呈现。。。", Qt::AlignCenter, ft_C_Title_32, 0, 0, Screen_w, Screen_h*0.85);
        StylizeLable(Credit, "制作组:\n江杨雅迪\n袁常成\n曾宇晨\n张洪波", Qt::AlignLeft, ft_C_Normal_16, Screen_w * 0.05, Screen_h * 0.8, Screen_w * 0.4, Screen_h * 0.3);
        StylizeButton(White_1, "黑白 1", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.01, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(White_2, "黑白 2", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.07, Screen_w*0.1, Screen_h*0.05);
        StylizeButton(Color, "彩色", ft_C_Normal_14, Screen_w*0.01, Screen_h*0.13, Screen_w*0.1, Screen_h*0.05);
    }
}

/*
 * Method: loading
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(loading()));
 * ------------------------------------------------------------------------------
 * loading() is a prviate slot that will be called when the Begin button is called
 * to add interesting in the transition when leaving the greeting page and going
 * to the main part of the programme
 */
void MainWindow:: loading(){
    Load->show();
    delay = new QTimer;
    delay->setInterval(1000);
    delay->start();
    Greeting_Widget->hide();

    QObject::connect(delay, SIGNAL(timeout()), Bottom_Widget, SLOT(show()));
    QObject::connect(delay, SIGNAL(timeout()), Top_Widget, SLOT(show()));
    QObject::connect(delay, SIGNAL(timeout()), Load, SLOT(hide()));
    QObject::connect(delay, SIGNAL(timeout()), Undo, SLOT(show()));
    QObject::connect(delay, SIGNAL(timeout()), Redo, SLOT(show()));
    QObject::connect(delay, SIGNAL(timeout()), delay, SLOT(deleteLater()));
}

/*
 * Method: backToMenu
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(backToMenu()));
 * ------------------------------------------------------------------------------
 * backToMenu() is a prviate slot that will be called when the Menu button is
 * clicked to back to the greeting page. This method will hide or show some
 * widgets or buttons, clear some collections to let them back to initial status.
 */
void MainWindow::backToMenu(){
    show_edit_status = false;
    EditWidget->hide();
    show_ratio_status = false;
    RatioWidget->hide();
    if (Path.isNull() == false){
        closeImage();
    }
    if(Confirm->isVisible() == false){
        lastStep();
    }
    Top_Widget->hide();
    Bottom_Widget->hide();
    Redo->hide();
    Undo->hide();
    Greeting_Widget->show();
    Credit->show();
}

/*
 * Method: openImage
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(openImage()));
 * ------------------------------------------------------------------------------
 * openImage() is a prviate slot that will be called when the choose file button is
 * clicked to allow the user to choose the image from their computer. This method
 * will trigger a file dialoge to get the path of the chosen image.
 */
void MainWindow::openImage(){
    QFileDialog *open_file = new QFileDialog(Top_Widget);
    open_file->setWindowTitle("Open Image");
    open_file->setDirectory(".");
    open_file->setNameFilter("Image Files(*.jpg *.jpeg *.png)");
    if (open_file->exec() == QDialog::Accepted) {
        Path = open_file->selectedFiles()[0];
        MainImage = new QLabel(Top_Widget);
        MainImage->setGeometry(Screen_w*0.01, Screen_h*0.01, Screen_w*0.96, Screen_h*0.78);
        img = new QImage(Path);
        QImage img_s = scaleImage(*img, Screen_w*0.96, Screen_h*0.78);
        unStack.push(*img);
        MainImage->setAlignment(Qt::AlignCenter);
        MainImage->setPixmap(QPixmap::fromImage(img_s));
        Cancel->setEnabled(true);
        Edit->setEnabled(true);
        Confirm->setEnabled(true);
        ChooseFile->hide();
        MainImage->show();
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}

/*
 * Method: closeImage
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(closeImage()));
 * ------------------------------------------------------------------------------
 * closeImage() is a prviate slot that will be called when the cancel button is
 * clicked to delete the current chosen image and rechoose image. This method
 * will set some buttons mute until the new image is chose.
 */
void MainWindow::closeImage(){
    MainImage->close();
    Cancel->setEnabled(false);
    Edit->setEnabled(false);
    Confirm->setEnabled(false);
    ChooseFile->show();
    while(unStack.empty() == false) {
        unStack.pop();
    }
    while(reStack.empty() == false) {
        reStack.pop();
    }
}

/*
 * Method: undoing
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(undoing()));
 * ------------------------------------------------------------------------------
 * undoing() is a prviate slot that will be called when the undo button is
 * clicked to undo the editing image.
 */
void MainWindow::undoing() {
    if (unStack.empty() == false) {
        reStack.push(unStack.top());
        unStack.pop();
    }
    if (unStack.empty() == false) {
        *img = unStack.top();
        updateImage();
    }
}

/*
 * Method: redoing
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(redoing()));
 * ------------------------------------------------------------------------------
 * redoing() is a prviate slot that will be called when the redo button is
 * clicked to redo the editing image.
 */
void MainWindow::redoing() {
    if (reStack.empty() == false) {
        unStack.push(reStack.top());
        reStack.pop();
    }
    if (unStack.empty() == false) {
        *img = unStack.top();
        updateImage();
    }
}

/*
 * Method: editImage
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(editImage()));
 * ------------------------------------------------------------------------------
 * editImage() is a prviate slot that will be called when the Edit, Crop, Spin, or Zoom
 * button is clicked to allow the user to do operations relative to the edit of the image.
 * This method will pop out the edit widget that offers three buttons for three edit
 * choices to the user when the Edit button is clicked and by clicking any of the four
 * buttons will close the widget. If the user selects to crop the image, the Ratio button
 * will show to allow the user to choose the crop ratio. As long as the edit is not finished
 * (the user does not press the Enter key), irrelevant buttons will keep in the inactive
 * status.
 */
void MainWindow::editImage(){
    QObject *sender = this->sender();
    if (Edit != sender){
        Cancel->setEnabled(false);
        Edit->setEnabled(false);
        Menu->setEnabled(false);
        Confirm->setEnabled(false);
        Undo->setEnabled(false);
        Redo->setEnabled(false);

        if(Crop == sender) {
            Ratio->show();
        }else if(Spin == sender) {
            rotate = true;
            MeasureR = 0;
        }else if(Zoom == sender) {
            zoom = true;
            MeasureD = 0;
        }
    }

    if (show_edit_status == false){
        show_edit_status = true;
        EditWidget->show();
    }else{
        show_edit_status = false;
        EditWidget->hide();
    }
}

/*
 * Method: selectRatio
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(selectRatio()));
 * ------------------------------------------------------------------------------
 * selectRatio() is a prviate slot that will be called when the Ratio, Free, W1H1, W16H9,
 * W9H16, W4H3, or W3H4 button is clicked to allow the user to choose ratio for the
 * cropping of the image. This method will pop out the ratio widget that offers six
 * choices to the user when the Ratio button is clicked and by clicking any of the seven
 * buttons will close the widget. Once the cropping is finished (the user press the Enter
 * key), mute bottons will be reacitivated.
 */
void MainWindow::selectRatio(){
    QObject *sender = this->sender();
    if (Ratio != sender){
        if(Free == sender) {
            crop = 0;
            Choose_Cut cropPic = Choose_Cut(img, Screen_h*0.78, Screen_w*0.96);
            cropPic.getCut(crop);
            cropPic.confirmPic();
            *img = cropPic.getQImage();
            unStack.push(*img);
            updateImage();

        }else if(W1H1 == sender) {
            crop = 1;
            Choose_Cut cropPic = Choose_Cut(img, Screen_h*0.78, Screen_w*0.96);
            cropPic.getCut(crop);
            cropPic.confirmPic();
            *img = cropPic.getQImage();
            unStack.push(*img);
            updateImage();

        }else if(W16H9 == sender) {
            crop = (double) 16 / 9;
            Choose_Cut cropPic = Choose_Cut(img, Screen_h*0.78, Screen_w*0.96);
            cropPic.getCut(crop);
            cropPic.confirmPic();
            *img = cropPic.getQImage();
            unStack.push(*img);
            updateImage();

        }else if(W9H16 == sender) {
            crop = (double) 9 / 16;
            Choose_Cut cropPic = Choose_Cut(img, Screen_h*0.78, Screen_w*0.96);
            cropPic.getCut(crop);
            cropPic.confirmPic();
            *img = cropPic.getQImage();
            unStack.push(*img);
            updateImage();

        }else if(W4H3 == sender) {
            crop = (double) 4 / 3;
            Choose_Cut cropPic = Choose_Cut(img, Screen_h*0.78, Screen_w*0.96);
            cropPic.getCut(crop);
            cropPic.confirmPic();
            *img = cropPic.getQImage();
            unStack.push(*img);
            updateImage();

        }else if(W3H4 == sender) {
            crop = (double) 3 / 4;
            Choose_Cut cropPic = Choose_Cut(img, Screen_h*0.78, Screen_w*0.96);
            cropPic.getCut(crop);
            cropPic.confirmPic();
            *img = cropPic.getQImage();
            unStack.push(*img);
            updateImage();

        }
        Cancel->setEnabled(true);
        Edit->setEnabled(true);
        Menu->setEnabled(true);
        Confirm->setEnabled(true);
        Undo->setEnabled(true);
        Redo->setEnabled(true);
        Ratio->hide();
    }
    if (show_ratio_status == false){
        show_ratio_status = true;
        RatioWidget->show();
    }else{
        show_ratio_status = false;
        RatioWidget->hide();
    }
}

/*
 * Method: updateImage
 * Usage: QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
 * ------------------------------------------------------------------------------
 * updateImage() is a prviate slot that will be called to update the image showing on the
 * screen after other methods has changed the image to show. This method will creat
 * another QImage variable to store the scaled verstion of the target image to avoid the
 * quality compression during the adaptive image showing.
 */
void MainWindow::updateImage(){
    QImage temp =scaleImage(*img, Screen_w*0.96, Screen_h*0.78);
    MainImage->setPixmap(QPixmap::fromImage(temp));
}

/*
 * Method: nextStep
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(nextStep()));
 * ------------------------------------------------------------------------------
 * nextStep() is a prviate slot that will be called when the Confirm button is clicked to
 * confirm to continue to next stage. This method will hide the buttons, labels, widget
 * that are special for the current stage and show the buttons, labels, and widgets that
 * are special for the next stage..
 */
void MainWindow::nextStep(){
    Cancel->hide();
    Edit->hide();
    Ratio->hide();
    Confirm->hide();
    show_edit_status = false;
    EditWidget->hide();
    show_ratio_status = false;
    RatioWidget->hide();
    Back->show();
    AllStyles->show();
    Apply->show();
    Undo->hide();
    Redo->hide();
}

/*
 * Method: lastStep
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(lastStep()));
 * ------------------------------------------------------------------------------
 * lastStep() is a prviate slot that will be called when the Back button is clicked to
 * back to the previous stage. This method will hide the buttons, labels, widget that
 * are special for the current stage, cleat the string and the queues that stores the
 * information of the current stage and show the buttons, labels, and widgets that
 * are special for the previous stage.
 */
void MainWindow::lastStep(){
    Cancel->show();
    Edit->show();
    Confirm->show();
    Back->hide();
    AllStyles->hide();
    Mode->hide();
    show_mode_status = false;
    ModeWidget->hide();
    result->setText("");
    show_style_status = false;
    StyleWidget->hide();
    Apply->setEnabled(false);
    Apply->hide();
    Save->hide();
    Undo->show();
    Redo->show();
    updateImage();
    while(!imagesToShow.empty()){
        imagesToShow.pop();
    }
    while (! allImages.empty()){
        allImages.pop();
    }
}

/*
 * Method: showStyles
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(showStyles()));
 * ------------------------------------------------------------------------------
 * showStyles() is a prviate slot that will be called when the AllStyles, Style_1, Style_2,
 * Style_3, or Style_4 button is clicked to allow the user to choose different styles for
 * the image to apply. This method will pop out the style widget that offers four styles
 * to the user when the AllStyles button is clicked and by clicking any of the five buttons
 * will close the widget. Once a style is chosen,  the information of which style the user
 * chooses will be recorded and showed in a label. If the chosen style is not the style 4,
 * the Mode button will show for further refining the style mode. The Apply button will
 * stay inactive until the whole style selecting has been finished.
*/
void MainWindow::showStyles(){
    QObject *sender = this->sender();
    if (AllStyles != sender){
        result->hide();
        Save->hide();
        Apply->show();
        Apply->setEnabled(false);
        if(Style_1 == sender) {
            ChosenStyle = Style_1->text();
            Mode->show();
        }
        else if(Style_2 == sender) {
            ChosenStyle = Style_2->text();
            Mode->show();
        }
        else if(Style_3 == sender) {
            ChosenStyle = Style_3->text();
            Mode->show();
        }
        else if(Style_4 == sender) {
            ChosenStyle = Style_4->text();
            Mode->hide();
            ModeWidget->hide();
            Apply->setEnabled(true);
        }
        if ( language == 0){
            result->setText("You selected:\n" + ChosenStyle);
        }else {
            result->setText("你选择了:\n" + ChosenStyle);
        }
        result->show();
    }
    if (show_style_status == false){
        show_style_status = true;
        StyleWidget->show();
    }else{
        show_style_status = false;
        StyleWidget->hide();
    }
}

/*
 * Method: selectMode
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(selectMode()));
 * ------------------------------------------------------------------------------
 * selectMode() is a prviate slot that will be called when the Mode, Quick, Standard, Advanced,
 * White_1, White_3, or Color button is clicked to allow the user to choose different mode
 * for a single style. This method will pop out the Mode widget that offers three modes (Quick,
 * Standard and Advanced for Style 1 and 2, White_1, White_2 and Color for Style 3) to the
 * user when the Mode button is clicked and by clicking any of the buttons will close the widget.
 * Once a mode is chosen,  the information of which mode the user chooses will be recorded
 * and showed in a label, and the apply button will be activated to apply the chosen style and mode.
*/
void MainWindow::selectMode(){
    if (ChosenStyle == Style_3->text()){
        Quick->hide();
        Standard->hide();
        Advanced->hide();
        White_1->show();
        White_2->show();
        Color->show();
    }else{
        Quick->show();
        Standard->show();
        Advanced->show();
        White_1->hide();
        White_2->hide();
        Color->hide();
    }

    QObject *sender = this->sender();
    QString ChosenMode;
    if (Mode != sender){
        if (Quick == sender) {
            mode = 0;
            ChosenMode = " -- "+ Quick->text();
        }
        else if (Standard == sender) {
            mode = 1;
            ChosenMode = " -- "+ Standard->text();
        }
        else if (Advanced == sender){
            mode = 2;
            ChosenMode = " -- " + Advanced->text();
        }
        else if (White_1 == sender) {
            mode = 0;
            ChosenMode = " -- " + White_1->text();
        }
        else if (White_2 == sender) {
            mode = 1;
            ChosenMode = " -- " + White_2->text();
        }
        else if (Color == sender) {
            mode = 2;
            ChosenMode = " -- " + Color->text();
        }
        if ( language == 0){
            result->setText("You selected:\n" + ChosenStyle + ChosenMode);
        }else {
            result->setText("你选择了:\n" + ChosenStyle + ChosenMode);
        }
        result->show();
        Save->hide();
        Apply->show();
        Apply->setEnabled(true);
    }

    if (show_mode_status == false){
        show_mode_status = true;
        ModeWidget->show();
    }else{
        show_mode_status = false;
        ModeWidget->hide();
    }
}

/*
 * Method: preApply
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(preApply()));
 * ------------------------------------------------------------------------------
 * preApply() is a prviate slot that will be called when the Apply button is clicked to apply the
 * style and mode the user chooses for the image. This method will first show a widget to tell
 * the user the image is in processing now and ask them to wait. the function uses in applying
 * will take up a long time so the waiting widget must show before the applying and use QTimer
 * to call the applyStyle() or the widget showing will be interupted and failed if not in that way.
*/
void MainWindow::preApply(){
    while(!imagesToShow.empty()){
        imagesToShow.pop();
    }
    while (! allImages.empty()){
        allImages.pop();
    }
    ResultShowing->show();
    show_style_status = false;
    StyleWidget->hide();
    show_mode_status = false;
    ModeWidget->hide();
    beforeShowing->show();
    ResultShowing->show();
    delay = new QTimer;
    delay->setInterval(500);
    delay->start();
    QObject::connect(delay, SIGNAL(timeout()), delay, SLOT(deleteLater()));
    QObject::connect(delay,SIGNAL(timeout()), this, SLOT(applyStyle()));
}

/*
 * Method: applyStyle
 * Usage: QObject::connect(timer, SIGNAL(timeout()), this, SLOT(preApply()));
 * ------------------------------------------------------------------------------
 * applyStyle() is a prviate slot that will be called in the preApply().This method will call different
 * functions written in the effect.h/cpp according to the chosen style and mode. imagesToShow
 * is the queue that stores the processed images for result showing and allImages will extra store
 * the internal processed images like the tone images of different channels besides the images in
 * the imagesToShow. The showResult_1_2() or showResult_3_4 will be called controlled by a timer
 * and the Save button will show after finishing the image processing, .
*/
void MainWindow::applyStyle(){
    imagesToShow.push(*img);
    if (ChosenStyle == Style_1->text()){
        if (mode == 0){
            allImages = P1_quick(imagesToShow);
        }else if (mode == 1){
            allImages = P1_standard(imagesToShow);
        }else if (mode == 2){
            allImages =  P1_advance(imagesToShow);
        }
    }
   else if (ChosenStyle == Style_2->text()){
        if (mode == 0){
            allImages =  P2_quick(imagesToShow);
        }else if (mode == 1){
            allImages =  P2_standard(imagesToShow);
        }else if (mode == 2){
            allImages =  P2_advance(imagesToShow);
        }
    }
   else if (ChosenStyle == Style_3->text()){
        if (mode == 0){
            P3_3(imagesToShow);
        }else if (mode == 1){
            P3_2(imagesToShow);
        }else if (mode == 2){
            P3_1(imagesToShow);
        }
   }
   else if (ChosenStyle == Style_4->text()){
        P4(imagesToShow);
   }
    beforeShowing->hide();
    ResultShowing->show();
    int count = imagesToShow.size();
    while(count > 0){
        QImage temp = imagesToShow.front();
        allImages.push(temp);
        imagesToShow.pop();
        imagesToShow.push(temp);
        count --;
    }

    delay = new QTimer;
    delay->setInterval(1500);
    delay->start();
    if (imagesToShow.size() > 2){
        QObject::connect(delay, SIGNAL(timeout()), this, SLOT(showResult_1_2()));
    }else{
        QObject::connect(delay, SIGNAL(timeout()), this, SLOT(showResult_3_4()));
    }
    Apply->hide();
    Save->show();
}

/*
 * Method: showResult_1_2
 * Usage: QObject::connect(timer, SIGNAL(timeout()), this, SLOT(showResult_1_2()));
 * ------------------------------------------------------------------------------
 * showResult_1_2() is a prviate slot that will be called in the applyStyle().This method will show
 * the processed images one by one for style 1 and style 2 which will offer four images to show,
 * namely are the source image, the tone image, the sketch or stroke, and the final result. After
 * showing the processed images, the image showed before the style application will be replaced
 * by the new result image.
*/
void MainWindow::showResult_1_2(){
    if(imagesToShow.size() == 4){
        QImage img_temp;
        first.setParent(ResultShowing);
        first.setGeometry(Screen_w*0.03, Screen_h*0.03, Screen_w*0.45, Screen_h*0.42);
        img_temp  = scaleImage(*img, Screen_w*0.45, Screen_h*0.42);
        first.setPixmap(QPixmap::fromImage(img_temp));
        first.setAlignment(Qt::AlignBottom | Qt::AlignRight);
        first.show();
        ResultShowing->show();
        imagesToShow.pop();
        delay->start();
    }else if(imagesToShow.size()== 3){
        QImage img_temp= imagesToShow.front();
        second.setParent(ResultShowing);
        second.setGeometry(Screen_w*0.52, Screen_h*0.03, Screen_w*0.45, Screen_h*0.42);
        img_temp  = scaleImage(img_temp, Screen_w*0.45, Screen_h*0.42);
        second.setPixmap(QPixmap::fromImage(img_temp));
        second.setAlignment(Qt::AlignBottom | Qt::AlignLeft);
        second.show();
        imagesToShow.pop();
        ResultShowing->show();
        delay->start();
    }else if (imagesToShow.size() == 2){
        QImage img_temp = imagesToShow.front();
        third.setParent(ResultShowing);
        third.setGeometry(Screen_w*0.03, Screen_h*0.5, Screen_w*0.45, Screen_h*0.42);
        img_temp  = scaleImage(img_temp, Screen_w*0.45, Screen_h*0.42);
        third.setPixmap(QPixmap::fromImage(img_temp));
        third.setAlignment(Qt::AlignTop | Qt::AlignRight);
        third.show();
        imagesToShow.pop();
        ResultShowing->show();
        delay->start();
    }else if (imagesToShow.size() == 1){
        QImage img_temp = imagesToShow.front();
        QImage img_forshow = img_temp;
        last.setParent(ResultShowing);
        last.setGeometry(Screen_w*0.52, Screen_h*0.5, Screen_w*0.45, Screen_h*0.42);
        img_temp  = scaleImage(img_temp, Screen_w*0.45, Screen_h*0.42);
        last.setPixmap(QPixmap::fromImage(img_temp));
        last.setAlignment(Qt::AlignTop | Qt::AlignLeft);
        last.show();
        ResultShowing->show();
        img_forshow = scaleImage(img_forshow, Screen_w*0.96, Screen_h*0.78);
        MainImage->setPixmap(QPixmap::fromImage(img_forshow));
        imagesToShow.pop();
        delay->setInterval(3000);
        delay->start();
    }else{
        last.hide();
        third.hide();
        second.hide();
        first.hide();
        MainImage->show();
        ResultShowing->hide();
        delay->deleteLater();
    }
}

/*
 * Method: showResult_3_4
 * Usage: QObject::connect(timer, SIGNAL(timeout()), this, SLOT(showResult_3_4()));
 * ------------------------------------------------------------------------------
 * showResult_3_4() is a prviate slot that will be called in the applyStyle().This method will show
 * the processed images one by one for style 3 and style 4 which will offer two images to show,
 * the source image and the final result. After showing the processed images, the image showed
 *  before the style application will be replaced by the new result image.
*/
void MainWindow::showResult_3_4(){
    if (imagesToShow.size() == 2){
            QImage img_temp = imagesToShow.front();
            third.setParent(ResultShowing);
            third.setGeometry(Screen_w*0.03, Screen_h*0.03, Screen_w*0.45, Screen_h*0.86);
            img_temp  = scaleImage(img_temp, Screen_w*0.45, Screen_h*0.86);
            third.setPixmap(QPixmap::fromImage(img_temp));
            third.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            third.show();
            imagesToShow.pop();
            ResultShowing->show();
            delay->start();
        }else if (imagesToShow.size() == 1){
            QImage img_temp = imagesToShow.front();
            QImage img_forshow = img_temp;
            last.setParent(ResultShowing);
            last.setGeometry(Screen_w*0.52, Screen_h*0.03, Screen_w*0.45, Screen_h*0.86);
            img_temp  = scaleImage(img_temp, Screen_w*0.45, Screen_h*0.86);
            last.setPixmap(QPixmap::fromImage(img_temp));
            last.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            last.show();
            ResultShowing->show();
            img_forshow = scaleImage(img_forshow, Screen_w*0.96, Screen_h*0.78);
            MainImage->setPixmap(QPixmap::fromImage(img_forshow));
            imagesToShow.pop();
            delay->start();
        }else{
            last.hide();
            third.hide();
            second.hide();
            first.hide();
            MainImage->show();
            ResultShowing->hide();
            delay->deleteLater();
        }
}

/*
 * Method: saveAs
 * Usage: QObject::connect(button, SIGNAL(clicked()), this, SLOT(saveAs()));
 * ------------------------------------------------------------------------------
 * saveAs() is a prviate slot that will be called when the Save button is clicked to save the result image
 * or the all images during the whole process .This method will divide the saving into four cases. The
 * first is for the style 3 and 4 because these two will not have internal processed images. In this case,
 * the method will directly ask the user to select path and input file name to save the final result. For
 * the other 2 style, the user will be asked to select whether save all images or just save the result image.
 * The second case is the user choose to save the result only. The third is the user chooses to save all but
 * the chosen mode is Quick which means there are four processed images. The fouth case is the user
 * chooses to save all and the mode is Standard or Advanced which has seven processed image. In the all
 * saved cases, the name of images is fixed and the user can choose the directory to save images.
*/
void MainWindow::saveAs(){
    if (ChosenStyle == Style_3->text() || ChosenStyle == Style_4->text()){
        QString save_path = QFileDialog::getSaveFileName(Top_Widget, "Save As", "", "Image Files (*.jpeg *.jpg *.png)");
        if (save_path.isNull() == false){
            QImage forsave = allImages.back();
            forsave.save(save_path);
        }
    }else {
        QMessageBox::StandardButton asw= QMessageBox::information(NULL, tr("Saving"), tr("Do you need to save the internal processing images (including the tone and the sketch image"), QMessageBox::Yes | QMessageBox::No);
        if (asw == QMessageBox::No){
            QString save_path = QFileDialog::getSaveFileName(Top_Widget, "Save As", "", "Image Files (*.jpeg *.jpg *.png)");
            if (save_path.isNull() == false){
                QImage forsave = allImages.back();
                forsave.save(save_path);
            }
        }else if (asw == QMessageBox::Yes){
            queue<QImage> q_temp;
            int count = allImages.size();
            while (count > 0){
                QImage img_temp;
                img_temp = allImages.front();
                q_temp.push(img_temp);
                allImages.pop();
                allImages.push(img_temp);
                count --;
            }

            queue<QString> q_name;
            if (mode != 0){
                q_name.push("/Tone_BlueChannel.png");
                q_name.push("/Tone_GreenChannel.png");
                q_name.push("/Tone_RedChannel.png");
            }
            q_name.push("/SourcePic.png");
            q_name.push("/Sketch.png");
            q_name.push("/Tone_selected.png");
            q_name.push("/FinalResult.png");

            QString save_path = QFileDialog::getExistingDirectory(Top_Widget, "Save in", "");
            if (save_path.isNull() == false){
                while(!q_temp.empty()){
                    QImage a = q_temp.front();
                    QString n = q_name.front();
                    a.save(save_path+n);
                    q_temp.pop();
                    q_name.pop();
                }
            }
        }
    }
}


