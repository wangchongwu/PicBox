//一个Qt图像显示类，封装图像显示、放缩、平移等操作
//采用opencv Mat类作为数据,封装了到QImage的接口


#ifndef PICBOX_H
#define PICBOX_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
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
	void setImg(cv::Mat);

protected://重写鼠标事件
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
	

private slots:
	void openImg();
	void clearImg();
	void saveImg();
	void stretchImg(int);
private:
	QGraphicsScene* imgScene;
	QGraphicsPixmapItem* pixmapItem;
	Mat srcImg;//原始cv图像
	Mat dstImg;//临时cv图像,保存中间状态
	double m_scaleFactor;//放缩系数

private:
	void zoomIn();
	void zoomOut();
};


static QImage cvMat2QImage(const cv::Mat & mat);//转换函数




#endif // PICBOX_H
