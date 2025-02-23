#include "Spectrum.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setLayoutDirection(Qt::RightToLeft);
    app.setWindowIcon(QIcon(":/Resources/TaifLogo.ico"));
    

    Spectrum w;
    w.show();
    return app.exec();
}
