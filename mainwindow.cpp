#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    picshower=new PicBox(this);
    picshower->setImg( QImage("C:\\Users\\WangChongwu.HP-DARKGHOST\\Desktop\\lena512color.tif"));
    setCentralWidget(picshower);
}

MainWindow::~MainWindow()
{

}
