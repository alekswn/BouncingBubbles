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

Bubble::Bubble(Presenter* scene, const QPointF &position, qreal radius, const QPointF &velocity)
    : model(scene->model, Coords2D(position.x(), position.y()),
                  radius, Coords2D(velocity.x(), velocity.y())),
      newPos(position),
      radius(radius)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsScenePositionChanges);
    setCacheMode(DeviceCoordinateCache);

    innerColor = randomColor();
    outerColor = randomColor();
    updateBrush();

    setPos(position);
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
    if (phase) {
        setPos(newPos);
    } else { //Check if bubble is escaping the scene
        QRectF bounds = scene()->sceneRect();
        Coords2D velocity = model.getVel();
        newPos = scenePos();
        newPos.rx() += velocity.x*STEP;
        newPos.ry() += velocity.y*STEP;
        if (   (newPos.x() - radius < bounds.left() && velocity.x < 0)
            || (newPos.x() + radius > bounds.right() && velocity.x > 0) ) {
            velocity.x = -velocity.x;
            newPos.rx() += 2*velocity.x*STEP;
        }
        if (   (newPos.y() - radius < bounds.top() && velocity.y < 0)
            || (newPos.y() + radius > bounds.bottom() && velocity.y > 0) ) {
            velocity.y = -velocity.y;
            newPos.ry() += 2*velocity.y*STEP;
        }
        model.setPos(Coords2D(newPos.x(), newPos.y()));
        model.setVel(velocity);
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

QVariant Bubble::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == ItemScenePositionHasChanged) {
        QPointF pos = value.toPointF();
        model.setPos(Coords2D(pos.x(), pos.y()));
    }
    return QGraphicsItem::itemChange(change, value);
}

