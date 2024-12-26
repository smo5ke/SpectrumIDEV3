#include "Spectrum.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Spectrum w;
    w.show();
    return a.exec();
}
