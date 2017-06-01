#include "picbox.h"
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

void PicBox::setImg(QImage im)
{
    imgScene->clear();
    currentImg=im;
    //注意：如果不清除原有对象，会大量消耗内存。
    imgScene->addPixmap(QPixmap::fromImage(im));
    this->setScene(imgScene);
    setCursor(Qt::CrossCursor);
}

void PicBox::setImg(cv::Mat mat)
{
	imgScene->clear();
	currentImg = cvMat2QImage(mat);
	//注意：如果不清除原有对象，会大量消耗内存。
	imgScene->addPixmap(QPixmap::fromImage(currentImg));
	this->setScene(imgScene);
	setCursor(Qt::CrossCursor);
}

void PicBox::zoomIn()
{
    m_scaleFactor *= 1.1;
    QMatrix matrix;
    matrix.scale(m_scaleFactor, m_scaleFactor);//两个方向上放缩比例
    this->setMatrix(matrix);
}
void PicBox::zoomOut()
{
    m_scaleFactor *= 0.9;
    QMatrix matrix;
    matrix.scale(m_scaleFactor, m_scaleFactor);//两个方向上放缩比例
    this->setMatrix(matrix);
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

	QAction* openmenu = new QAction("Open", this);
	//connect(openmenu,SIGNAL(triggered()),this,open
	QAction* saveasmenu = new QAction("Save as...", this);

	menu->addAction(openmenu);
	menu->addAction(saveasmenu);
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
