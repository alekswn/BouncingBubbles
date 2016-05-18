#include <QResizeEvent>
#include <QDebug>

#include "view.h"

View::View(Presenter* scene)
    :QGraphicsView(scene)
{
//    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setBackgroundBrush(Qt::darkCyan);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setWindowTitle(tr("Bouncing Bubbles"));

}

void View::resizeEvent(QResizeEvent *event) {
    qDebug() << "RESIZE: " << event;
    scene()->setSceneRect(0,0,event->size().width(), event->size().height());
    QGraphicsView::resizeEvent(event);
}

