#include <QTime>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "presenter.h"
#include "view.h"
#include "bubble.h"

Presenter::Presenter(const QRectF &sceneRect, int initial_bubbles_number)
{
    setSceneRect(sceneRect);
    setItemIndexMethod(QGraphicsScene::NoIndex);
#ifdef QT_DEBUG
    for (qreal x1 = sceneRect.left(); x1 <= sceneRect.right(); x1+=100 ) addLine(x1,sceneRect.top(),x1,sceneRect.bottom());
    for (qreal y1 = sceneRect.left(); y1 <= sceneRect.right(); y1+=100 ) addLine(sceneRect.left(),y1,sceneRect.right(),y1);
#endif

    createBubbles(initial_bubbles_number);

    animationTimer.setSingleShot(true);
    connect(&animationTimer, &QTimer::timeout, this, &Presenter::advance);
    animationTimer.start(1000/FPS);

}

Presenter::~Presenter()
{
}

void Presenter::addBubble(QPointF position, qreal radius, QPointF velocity)
{
    Bubble* bubble = new Bubble(radius, velocity);
    bubble->setPos(position);
    addItem(bubble);

    qDebug() << "Adding bubble: " << position << radius << velocity;
}

void Presenter::popBubble(Bubble* bubble)
{
    removeItem(bubble);
    delete bubble;
}

void Presenter::createBubbles(int number)
{
    while (number--) {
        QPointF position(width()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                        height()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))));
        qreal radius = qMin(width(), height())*(0.0125 + 0.0875*qrand()/(RAND_MAX+1.0));
        QPointF velocity(FPS*width()*0.0125*(-0.5 + qrand()/(RAND_MAX+1.0)),
                        FPS*height()*0.0125*(-0.5 + qrand()/(RAND_MAX+1.0)));
        addBubble(position,radius,velocity);
    }
}

void Presenter::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if(mouseEvent->button() == Qt::RightButton) {
        QPointF clickPosition = mouseEvent->scenePos();
        addBubble(clickPosition,qMin(width(), height())*(0.0125 + 0.0875*qrand()/(RAND_MAX+1.0)),{0.0,0.0});
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Presenter::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if(mouseEvent->button() == Qt::LeftButton) {
        QPointF releasePosition = mouseEvent->scenePos();
        View * view = qobject_cast<View*>(mouseEvent->widget()->parent());
        QGraphicsItem * clickedItem = itemAt(releasePosition,view->transform());
        if (clickedItem && releasePosition == mouseEvent->buttonDownScenePos(Qt::LeftButton))
            popBubble(dynamic_cast<Bubble*>(clickedItem));
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void Presenter::advance() {
    QTime t;
    t.start();
    QGraphicsScene::advance();
    animationTimer.start(1000/FPS -t.elapsed());
}
