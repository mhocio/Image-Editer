#include "graphicsscene.h"
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include "qmath.h"

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setSceneRect(0, 0, 254, 254);
    this->setBackgroundBrush(Qt::gray);
    removeAllPoints();
}

void GraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    qDebug() << Q_FUNC_INFO << mouseEvent->scenePos();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::_clearItems()
{
    QList<QGraphicsItem*> itemsList = this->items();
    QList<QGraphicsItem*>::iterator iter = itemsList.begin();
    QList<QGraphicsItem*>::iterator end = itemsList.end();
    while(iter != end)
    { QGraphicsItem* item = (*iter); this->removeItem(item); iter++; }
    this->clear();
}

void GraphicsScene::removeAllPoints()
{
    _clearItems();
    points.clear();
    points.push_back(std::make_pair(0, 255));
    points.push_back(std::make_pair(255, 0));
    drawFunction();
}

class QGraphicsEllipseItem_Custom : public QGraphicsEllipseItem
{
protected:
};

void GraphicsScene::drawFunction()
{
    int radius = 3;
    _clearItems();

    std::pair<int,int> prevPair;
    bool first = true;

    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    QLineF lineDotted = QLineF(0, 255, 255, 0);
    this->addLine(lineDotted, QPen(Qt::DashDotLine));

    for (std::pair<int,int> elem: points) {
        if (!first) {
            QLineF line = QLineF(prevPair.first, prevPair.second, elem.first, elem.second);
            this->addLine(line, blackPen);
        }

        //QGraphicsEllipseItem_CustomPoint * ellipse;
        QGraphicsEllipseItem * ellipse = this->addEllipse(elem.first - radius, elem.second - radius, radius*2, radius*2);
        QGraphicsEllipseItem_Custom Custom;
        Custom.setRect(ellipse->rect());

        ellipse->setFlag(QGraphicsItem::ItemIsMovable);
        ellipse->setBrush(Qt::white);

        //Custom.setFlag(QGraphicsItem::ItemIsMovable);
        Custom.setBrush(Qt::red);
        this->addItem(&Custom);

        prevPair.first = elem.first;
        prevPair.second = elem.second;

        if (first)
            first = false;
    }
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * me)
{
    qDebug() << Q_FUNC_INFO << me->scenePos();

    points.push_back(std::make_pair(me->scenePos().x(), me->scenePos().y()));
    sort(points.begin(),points.end());

    this->drawFunction();

    QGraphicsScene::mouseReleaseEvent(me);
}
