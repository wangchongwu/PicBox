#include "picbox.h"
#include <QDebug>
#include <QFileDialog>
PicBox::PicBox(QWidget *parent)
    :QGraphicsView(parent)
{
	pixmapItem = new QGraphicsPixmapItem();
    imgScene = new QGraphicsScene();
    this->setDragMode(QGraphicsView::RubberBandDrag);//可拖放显示一个蓝色框？？
    this->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);//放缩调整等行为以鼠标当前位置为中心

    m_scaleFactor = 1;//初始化放缩系数
 // this->setDragMode(QGraphicsView::ScrollHandDrag);



}



void PicBox::setImg(cv::Mat mat)
{
	imgScene->clear();
	m_scaleFactor = 1;
	srcImg = mat;
	dstImg = srcImg.clone();
	//注意：如果不清除原有对象，会大量消耗内存。
	imgScene->addPixmap(QPixmap::fromImage(cvMat2QImage(srcImg)));
	this->setScene(imgScene);
	setCursor(Qt::CrossCursor);
}

void PicBox::zoomIn()
{
    m_scaleFactor *= 1.2;
    QMatrix matrix;
    matrix.scale(m_scaleFactor, m_scaleFactor);//两个方向上放缩比例
    this->setMatrix(matrix);
}
void PicBox::zoomOut()
{
    m_scaleFactor *= 0.8;
    QMatrix matrix;
    matrix.scale(m_scaleFactor, m_scaleFactor);//两个方向上放缩比例
    this->setMatrix(matrix);
}
void PicBox::openImg()
{
	QString srcpath = QFileDialog::getOpenFileName(this, "choose a image file", "", "Image File(*.jpg;*.png;*.bmp;*.tiff;*.tif)");
	if (srcpath.isEmpty())
		return;
	this->setImg(cv::imread(srcpath.toStdString()));
	
}
void PicBox::clearImg()
{
	imgScene->clear();
	
}

void PicBox::saveImg()
{
	QString dstpath = QFileDialog::getSaveFileName(this, "Enter  file name", "c:/", "Image File(*.jpg;*.png;*.bmp;*.tiff;*.tif)");
	if (dstpath.isEmpty())
		return;
	cv::imwrite(dstpath.toStdString(), dstImg);
	
}

void PicBox::stretchImg(int)
{

}

void PicBox::wheelEvent(QWheelEvent *event)
{
    // 滚轮向上滑动，放大图像
    if (event->delta() > 0)
    {
        zoomIn();
    }
    // 滚轮向下滑动，缩小图像
    if (event->delta() < 0)
    {
        zoomOut();
    }
}

void PicBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton)
    {
        if(this->dragMode()!=QGraphicsView::ScrollHandDrag)

        {
           this->setDragMode(QGraphicsView::ScrollHandDrag);
           this->setInteractive(1);
        }
        else
        {
            this->setDragMode(QGraphicsView::RubberBandDrag);
            // this->setInteractive(0);
        }
    }

    QGraphicsView::mousePressEvent(event);//继续传递事件

}

void PicBox::contextMenuEvent(QContextMenuEvent * event)
{
	QMenu* menu = new QMenu(this);             //创建上下文菜单
	QMenu* graymenu = new QMenu("灰度调整",this);
	QMenu* filtmenu = new QMenu("滤波", this);
	
	//创建动作
	QAction* openAction = new QAction("打开", this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openImg()));
	QAction* saveasAction = new QAction("另存", this);
	connect(saveasAction, SIGNAL(triggered()), this, SLOT(saveImg()));
	QAction* clearAction = new QAction("清屏", this);
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearImg()));

	QAction* lineadjust1 = new QAction("1%拉伸", this);
	connect(lineadjust1, SIGNAL(triggered()), this, SLOT(stretchImg(1)));
	QAction* lineadjust2 = new QAction("2%拉伸", this);
	connect(lineadjust2, SIGNAL(triggered()), this, SLOT(stretchImg(2)));
	QAction* lineadjust5 = new QAction("5%拉伸", this);
	connect(lineadjust5, SIGNAL(triggered()), this, SLOT(stretchImg(5)));

	graymenu->addAction(lineadjust1);
	graymenu->addAction(lineadjust2);
	graymenu->addAction(lineadjust5);

	//添加动作
	menu->addAction(openAction);
	menu->addAction(saveasAction);
	menu->addAction(clearAction);

	menu->addMenu(graymenu);
	menu->addMenu(filtmenu);
	menu->exec(QCursor::pos());
	event->accept();
}

QImage cvMat2QImage(const cv::Mat & mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1  
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat  
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3  
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat  
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat  
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
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
