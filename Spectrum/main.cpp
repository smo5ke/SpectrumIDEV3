#include "Spectrum.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setLayoutDirection(Qt::RightToLeft);

    int fontId1 = QFontDatabase::addApplicationFont(":/fonts/Resources/fonts/Tajawal/Tajawal-Regular.ttf");
    int fontId2 = QFontDatabase::addApplicationFont(":/fonts/Resources/fonts/KawkabMono-Regular.ttf");
    if(fontId1 == -1 or fontId2 == -1) {
        qWarning() << "لم يستطع تحميل الخط";
    } else {
        QString tajawal = QFontDatabase::applicationFontFamilies(fontId1).at(0);
        QString kawkabMono = QFontDatabase::applicationFontFamilies(fontId2).at(0);
        QFont font{};
        QStringList fontFamilies{};
        fontFamilies << tajawal << kawkabMono;
        font.setFamilies(fontFamilies);
        font.setPixelSize(14);
        app.setFont(font);
    }

    // تخصيص شريط التمرير العمودي في كامل المحرر
    app.setStyleSheet(R"(
        QScrollBar:vertical {
            background: transparent;
            width: 12px;
            margin: 0px 4px 0px 4px;
        }

        QScrollBar::handle:vertical {
            background: #254663;
            min-height: 15px;
            border-radius: 2px;
        }

        QScrollBar::handle:vertical:hover {
            background: #325573;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            background: none;
            height: 0px;
        }

        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
        }
    )");



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
