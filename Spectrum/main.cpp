#include "Spectrum.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setLayoutDirection(Qt::RightToLeft);
    app.setWindowIcon(QIcon(":/Resources/TaifLogo.ico"));
    
    // لتشغيل ملف ألف بإستخدام محرر طيف عند إختيار المحرر ك برنامج للتشغيل
    QString filePath{};
    if (app.arguments().count() > 2) return -1;
    if (app.arguments().count() == 2) filePath = app.arguments().at(1);

    filePath = "Status.alif";
    Spectrum w(filePath);
    w.show();
    return app.exec();
}
