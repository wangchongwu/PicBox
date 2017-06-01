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

    QGraphicsView::mousePressEvent(event);

}


