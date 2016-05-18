/****************************************************************************
** Copyright (C) 2016 Alexey Novikov, alexey@novikov.io                    **
** Derived from overpaiting example                                        **
** http://doc.qt.io/qt-4.8/qt-opengl-overpainting-example.html             **
****************************************************************************/

#include <QtGui>
#include <QGraphicsScene>
#include <QDebug>


#include "bubble.h"
#include "presenter.h"

Bubble::Bubble(qreal radius, const QPointF &velocity)
    : radius(radius), vel(velocity), acc(0.0,0.0)
{
    setFlag(ItemIsMovable);
//    setFlag(ItemSendsScenePositionChanges);
    setCacheMode(DeviceCoordinateCache);

    innerColor = randomColor();
    outerColor = randomColor();
    updateBrush();
}


void Bubble::updateBrush()
{
    QRadialGradient gradient(QPointF(radius, radius), radius,
                             QPointF(radius*0.5, radius*0.5));

    gradient.setColorAt(0, QColor(255, 255, 255, 255));
    gradient.setColorAt(0.25, innerColor);
    gradient.setColorAt(1, outerColor);
    brush = QBrush(gradient);
}

void Bubble::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(-radius, -radius);
    painter->setBrush(brush);
    painter->setPen(QPen(Qt::white, 0));
    painter->drawEllipse(0, 0, 2.0*radius, 2.0*radius);
    painter->restore();
}

QColor Bubble::randomColor()
{
    int red = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int green = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int blue = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int alpha = int(91 + 100.0*qrand()/(RAND_MAX+1.0));

    return QColor(red, green, blue, alpha);
}

void Bubble::advance(int phase)
{
    if (!phase) {
        QRectF sceneRect = scene()->sceneRect();
        QPointF newPos = mapToScene(vel*STEP);
        if (   (newPos.x() - radius < sceneRect.left() && vel.x() < 0)
            || (newPos.x() + radius > sceneRect.right() && vel.x() > 0) )
            vel.setX(-vel.x());
        if (   (newPos.y() - radius < sceneRect.top() && vel.y() < 0)
            || (newPos.y() + radius > sceneRect.bottom() && vel.y() > 0) )
            vel.setY(-vel.y());
    } else {
        moveBy(vel.x()*STEP,vel.y()*STEP);
    }
}

QRectF Bubble::boundingRect() const
{
    return QRectF(-radius, -radius, 2.0 * radius, 2.0 * radius);
}


QPainterPath Bubble::shape() const
{
    QPainterPath path;
    path.addEllipse(-radius, -radius,  2.0*radius, 2.0*radius);
    return path;
}
/*
QVariant Bubble::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == ItemScenePositionHasChanged) {
  //      qDebug() << this << value;
    }
    return QGraphicsItem::itemChange(change, value);
}
*/
