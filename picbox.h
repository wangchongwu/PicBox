//一个Qt图像显示类，封装图像显示、放缩、平移等操作


#ifndef PICBOX_H
#define PICBOX_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QImage>
class PicBox : public QGraphicsView//继承
{
    Q_OBJECT
public:
    PicBox(QWidget *parent);
    void setImg(QImage);
private:
    QGraphicsScene* imgScene;
    QGraphicsPixmapItem* pixmapItem;
    QImage currentImg;
    double m_scaleFactor;//放缩系数
protected://重写鼠标事件
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent *event);


public slots:
    void zoomIn();
    void zoomOut();

private slots:

};








#endif // PICBOX_H
