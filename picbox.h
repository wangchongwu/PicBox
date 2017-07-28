//一个Qt图像显示类，封装图像显示、放缩、平移等操作
//采用opencv Mat类作为数据,封装了到QImage的接口


#ifndef PICBOX_H
#define PICBOX_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <QMenu>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QAction>
#include <opencv.hpp>

using namespace cv;

class PicBox : public QGraphicsView//继承
{
    Q_OBJECT
public:
    PicBox(QWidget *parent);
	void setImg(cv::Mat);//主要对外接口，设置一个全新的图像
	double getScaleFactor();//查询当前放缩比
protected://重写鼠标事件
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
private slots:
	void openImg();
	void clearImg();
	void saveImg();
	void resetImg();
	void testImg();//生成一个测试图像，不用每次都找图像测试
	void stretchImg1();
	void stretchImg2();
	void stretchImg5();
private://私有成员
	QGraphicsScene* imgScene;
	QGraphicsPixmapItem* pixmapItem;
	Mat srcImg;//原始cv图像
	Mat dstImg;//临时cv图像,保存中间状态
	double m_scaleFactor;//放缩系数

private://私有函数
	void changeImg(cv::Mat);//改变图像，内部函数使用，不对外提供
	void zoomIn();
	void zoomOut();
	int getDN(int x, int y);//获取DN值
	void stretchImg(float);//实现拉伸的函数，控制拉伸比例
};


static QImage cvMat2QImage(const cv::Mat & mat);//转换函数




#endif // PICBOX_H
