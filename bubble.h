/****************************************************************************
** Copyright (C) 2016 Alexey Novikov, alexey@novikov.io                    **
** Derived from overpaiting example                                        **
** http://doc.qt.io/qt-4.8/qt-opengl-overpainting-example.html             **
****************************************************************************/

#ifndef BUBBLE_H
#define BUBBLE_H

#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include "model.h"

class QPainter;
class Presenter;

class Bubble  : public QGraphicsItem
{
public:
    Bubble(Presenter* scene, const QPointF &position, qreal radius, const QPointF &velocity);

    int type() const { return UserType + 1; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void advance(int);

private:
    void updateBrush();
    QColor randomColor();

    BubbleModel model;
    mutable QPointF newPos;

    qreal radius;
    QBrush brush;
    QColor innerColor;
    QColor outerColor;


};

#endif
