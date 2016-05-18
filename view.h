#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QGLWidget>

#include "presenter.h"

class View : public QGraphicsView
{
    Q_OBJECT

    void resizeEvent(QResizeEvent *event);
public:
    View(Presenter* scene);
};

#endif // VIEW_H
