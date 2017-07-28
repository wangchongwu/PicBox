#include "picbox.h"
#include <QDebug>
#include <QFileDialog>
#include <QLayout>
#include <QMessageBox>
#include <QLabel>
#include <QToolTip>

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
	this->setMouseTracking(true);
	// this->setDragMode(QGraphicsView::ScrollHandDrag);
	this->setMinimumSize(QSize(400, 400));

}



void PicBox::setImg(cv::Mat mat)
{
	imgScene->~QGraphicsScene();
	imgScene = new QGraphicsScene();
	m_scaleFactor = 1;
	srcImg = mat.clone();
	dstImg = mat.clone();
	//注意：如果不清除原有对象，会大量消耗内存。
	QPixmap Pix = QPixmap::fromImage(cvMat2QImage(srcImg));
	pixmapItem = imgScene->addPixmap(Pix);
	this->setScene(imgScene);
	setCursor(Qt::CrossCursor);

}

double PicBox::getScaleFactor()
{
	return m_scaleFactor;
}

void PicBox::changeImg(cv::Mat mat)
{
	imgScene->clear();
	m_scaleFactor = 1;
	dstImg = mat.clone();
	imgScene->addPixmap(QPixmap::fromImage(cvMat2QImage(dstImg)));
	this->setScene(imgScene);
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

int PicBox::getDN(int x, int y)
{
	int DN = -1;
	if (!srcImg.empty())
	{
		
		//DN = (int)(*(srcImg.data + srcImg.step[0] * srcImg.rows + srcImg.step[1] * srcImg.cols));
		DN = srcImg.at<Vec3b>(y, x)[0];
	}
	return DN;
}

void PicBox::openImg()
{
	QString srcpath = QFileDialog::getOpenFileName(this, "choose a image file", "", "Image File(*.jpg;*.png;*.bmp;*.tiff;*.tif)");
	if (srcpath.isEmpty())
		return;
	Mat mat = cv::imread(srcpath.toStdString());
	srcImg = mat.clone();
	this->setImg(srcImg);


}
void PicBox::clearImg()
{
	imgScene->clear();
	srcImg = NULL;
	dstImg = NULL;
}

void PicBox::saveImg()
{
	if (srcImg.empty())
	{
		QMessageBox::information(this, "注意", "没有图像!", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	QString dstpath = QFileDialog::getSaveFileName(this, "Enter  file name", "c:/", "Image File(*.jpg;*.png;*.bmp;*.tiff;*.tif)");
	if (dstpath.isEmpty())
		return;
	cv::imwrite(dstpath.toStdString(), dstImg);

}

void PicBox::resetImg()
{
	dstImg = srcImg.clone();
	setImg(dstImg);
}

void PicBox::testImg()
{
	int wid = 1000;
	int hei = 1000;

	Mat im(wid, hei, CV_8UC3);//
	uchar temp = rand() * 255;

	for (int i = 0; i < hei; i++)
	{

		for (int j = 0; j < wid; j++)
		{
			(im.data + (im.step)*i)[3 * j] = (i / 100 + j / 100) * 100;
			(im.data + (im.step)*i)[3 * j + 1] = (i / 100 + j / 100) * 100 + temp;
			(im.data + (im.step)*i)[3 * j + 2] = (i / 100 + j / 100) * 100 - temp;
		}

	}
	setImg(im);

	//MatIterator_<Vec3b> it, end;
	//for (it = im.begin<Vec3b>(), end = im.end<Vec3b>(); it != end; ++it)
	//{
	//	(*it)[0] = (it - im.begin<Vec3b>())/10*100;
	//	(*it)[1] = abs((*it)[0] - 255);
	//	(*it)[2] = abs((*it)[0] - 255);
	//}
	//setImg(im);
}

void PicBox::stretchImg1()
{
	stretchImg(1.0);
}
void PicBox::stretchImg2()
{
	stretchImg(2.0);
}
void PicBox::stretchImg5()
{
	stretchImg(5.0);
}
void PicBox::stretchImg(float k)
{
	if (srcImg.empty())
		return;
	uchar* imdata = srcImg.data;
	size_t hist[256] = { 0 };
	for (int loop = 0; loop < srcImg.rows*srcImg.cols; loop++)
	{
		hist[*(imdata + loop)]++;
	}
	int DNL = 0, DNH = 255;
	size_t cnt = 0;
	int loop = 0;
	while (loop < 256)
	{
		cnt += hist[loop];
		loop++;
		if (cnt > srcImg.rows*srcImg.cols*k / 100)
		{
			DNL = loop;
			break;
		}

	}
	while (loop < 256)
	{
		cnt += hist[loop];
		loop++;
		if (cnt > srcImg.rows*srcImg.cols*(1 - k / 100))
		{
			DNH = loop;
			break;
		}

	}

	Mat lut(1, 256, CV_8U);//查找表
	uchar *p = lut.data;
	//std::vector<int> vDN;
	for (int i = 0; i < 256; i++)
	{
		if (i < DNL)
			p[i] = 0;
		else if (i > DNH)
			p[i] = 255;
		else
			p[i] = (i - DNL) * 255 / (DNH - DNL);
		//vDN.push_back(p[i]);
	}
	//查找表操作
	Mat m;
	LUT(srcImg, lut, m);
	dstImg = m.clone();

	changeImg(dstImg);
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
		if (this->dragMode() != QGraphicsView::ScrollHandDrag)

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
	QMenu* graymenu = new QMenu("灰度调整", this);
	QMenu* filtmenu = new QMenu("滤波", this);


	//创建动作
	QAction* openAction = new QAction("打开", this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openImg()));

	QAction* saveasAction = new QAction("另存", this);
	connect(saveasAction, SIGNAL(triggered()), this, SLOT(saveImg()));

	QAction* clearAction = new QAction("清屏", this);
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearImg()));

	QAction* resetAction = new QAction("重置", this);
	connect(resetAction, SIGNAL(triggered()), this, SLOT(resetImg()));

	QAction* testAction = new QAction("产生测试图像", this);
	connect(testAction, SIGNAL(triggered()), this, SLOT(testImg()));


	QAction* lineadjust1 = new QAction("1%拉伸", this);
	connect(lineadjust1, SIGNAL(triggered()), this, SLOT(stretchImg1()));

	QAction* lineadjust2 = new QAction("2%拉伸", this);
	connect(lineadjust2, SIGNAL(triggered()), this, SLOT(stretchImg2()));

	QAction* lineadjust5 = new QAction("5%拉伸", this);
	connect(lineadjust5, SIGNAL(triggered()), this, SLOT(stretchImg5()));

	graymenu->addAction(lineadjust1);
	graymenu->addAction(lineadjust2);
	graymenu->addAction(lineadjust5);

	//添加动作
	menu->addAction(openAction);
	menu->addAction(saveasAction);
	menu->addAction(resetAction);
	menu->addAction(clearAction);
	menu->addAction(testAction);

	menu->addMenu(graymenu);
	menu->addMenu(filtmenu);
	menu->exec(QCursor::pos());
	event->accept();
}

void PicBox::mouseDoubleClickEvent(QMouseEvent * event)
{
	//QLabel* tips=new QLabel(this);
	//QPoint pos1 = QPoint(event->x(), event->y());
	//QPoint pos2 = QPoint(event->x()+50, event->y()+10);
	//tips->setText(QString::number(event->x()) + "  " + QString::number(event->y()));
	//tips->setWindowFlags(Qt::FramelessWindowHint);
	//tips->setGeometry(QRect(pos1, pos2));
	//tips->show();

	//qDebug() << "Mouse:" << pixmapItem->mapToScene(event->x(), event->y());
	//qDebug() << imgScene->itemsBoundingRect();
	//qDebug() << this->pos();

	//qDebug() << mapToGlobal( pixmapItem->boundingRect());
	
	
}

void PicBox::mouseMoveEvent(QMouseEvent * event)
{
	//QPointF qf = pixmapItem->QGraphicsItem::mapToScene(event->x(), event->y());
	QPointF qff(event->x(), event->y());
	QPointF qf = mapToScene(QPoint(qff.x(),qff.y()));
	int xPos = floor(qf.x());
	int yPos = floor(qf.y());

	
	if (xPos >= 0 && yPos >= 0 && xPos <= srcImg.cols-1&&yPos <= srcImg.rows-1)
	{
		QString pStr =QString::number(xPos+1) + "  " +QString::number(yPos+1) + QString(":") + QString::number(getDN(xPos, yPos));
		QToolTip::showText(event->globalPos(), pStr);
	}

	QGraphicsView::mouseMoveEvent(event);//继续传递事件
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
