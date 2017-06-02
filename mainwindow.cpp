#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    picshower=new PicBox(this);
    picshower->setImg( cv::imread("C:\\Users\\wcw\\Desktop\\170419-005316767.jpg"));
    setCentralWidget(picshower);

}

MainWindow::~MainWindow()
{

}
