#include "Spectrum.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setLayoutDirection(Qt::RightToLeft);

    int fontId = QFontDatabase::addApplicationFont(":/fonts/Resources/fonts/Tajawal/Tajawal-Regular.ttf");
    if(fontId == -1) {
        qWarning("لم يستطع تحميل الخط");
    } else {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        font.setPixelSize(14);
        app.setFont(font);
    }

    // لتشغيل ملف ألف بإستخدام محرر طيف عند إختيار المحرر ك برنامج للتشغيل
    QString filePath{};
    if (app.arguments().count() > 2) {
        int ret = QMessageBox::warning(nullptr, "ألف",
                                       "لا يمكن تمرير أكثر من معامل واحد",
                                       QMessageBox::Close);
        return ret;
    }
    if (app.arguments().count() == 2) {
        filePath = app.arguments().at(1);
    }

    Spectrum w(filePath);
    w.show();
    return app.exec();
}
