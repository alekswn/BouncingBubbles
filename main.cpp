#include <QApplication>
#include <QScreen>

#include "presenter.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Presenter scene(app.screens().front()->availableGeometry(), 20);
    View view(&scene);
    view.show();
    return app.exec();
}
