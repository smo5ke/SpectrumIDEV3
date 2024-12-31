#include "Spectrum.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setLayoutDirection(Qt::RightToLeft);

    Spectrum w;
    w.show();
    return app.exec();
}
