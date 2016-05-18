#ifndef PRESENTER_H
#define PRESENTER_H

#include <QGraphicsScene>
#include <QTimer>

constexpr int FPS = 100;
constexpr double STEP = 1.0/FPS;

class Bubble;

class Presenter : public QGraphicsScene
{
    Q_OBJECT

    QTimer animationTimer;

private:
    void addBubble(QPointF position, qreal radius, QPointF velocity);
    void popBubble(Bubble * bubble);
    void createBubbles(int);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

public slots:
    void advance();

public:
    Presenter(const QRectF &sceneRect, int initial_bubbles_number);
    ~Presenter();



};

#endif // PRESENTER_H
