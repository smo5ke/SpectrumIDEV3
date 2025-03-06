#include "Spectrum.h"

#include <QDockWidget>
#include <QVBoxLayout>
#include <QScreen>
#include <QShortcut>


Spectrum::Spectrum(const QString& filePath, QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(" طـيـف ");
    QScreen* screenSize = QGuiApplication::primaryScreen();
    this->setGeometry(screenSize->size().width() / 3, screenSize->size().height() / 7, 400, 450);
    setStyleSheet(R"(
            QMainWindow::separator {
                background-color: #2a2c44;
                width: 4px;
                height: 4px;
            }
            
            QMainWindow::separator:hover {
                background-color: #393c5d;
            })");

    QWidget* center = new QWidget(this);
    QVBoxLayout* vlay = new QVBoxLayout(center);
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);


    fileIO = new SPFileIO(this);
    editor = new SPEditor(center);
    //terminal = new Terminal(this);
    //folderTree = new FolderTree(editor, this);
    menuBar = new SPMenuBar(this);
    setMenuBar(menuBar);
    
    vlay->addWidget(editor);
    //vlay->addWidget(terminal);


    //addDockWidget(Qt::BottomDockWidgetArea, terminal); // يجب أن تكون بعد vlay->addWidget(terminal)
    //addDockWidget(Qt::RightDockWidgetArea, folderTree);
    this->setCentralWidget(center);

    // لتشغيل ملف ألف بإستخدام محرر طيف عند إختيار المحرر ك برنامج للتشغيل
    if (!filePath.isEmpty()) {
        this->onOpenRequested(filePath);
    }


    connect(menuBar, &SPMenuBar::newRequested, this, &Spectrum::onNewRequested);
    connect(menuBar, &SPMenuBar::openRequested, this, [this]() {this->onOpenRequested(""); });
    connect(menuBar, &SPMenuBar::saveRequested, this, &Spectrum::onSaveRequested);
    connect(menuBar, &SPMenuBar::saveAsRequested, this, &Spectrum::onSaveAsRequested);
    connect(editor, &SPEditor::openRequest, this, &Spectrum::onOpenRequested);
}

Spectrum::~Spectrum()
{}






void Spectrum::onNewRequested() {
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(nullptr, "ألف",
            "تم التعديل على الملف.\n"
            "هل تريد حفظ التغييرات؟",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            fileIO->saveFile(editor->document()->toPlainText());
        }
        else if (ret == QMessageBox::Discard) {
            // nothing
        }
        else {
            return;
        }
    }

    editor->clear();
    fileIO->newFile();
    editor->document()->setModified(false);
}

void Spectrum::onOpenRequested(QString filePath) {
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(nullptr, "ألف",
            "تم التعديل على الملف.\n"
            "هل تريد حفظ التغييرات؟",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            fileIO->saveFile(editor->document()->toPlainText());
        }
        else if (ret == QMessageBox::Discard) {
            // nothing
        }
        else {
            return;
        }
    }

    QString content = fileIO->openFile(filePath);
    if (content != nullptr) {
        editor->setPlainText(content);
        editor->document()->setModified(false);
    }
}

void Spectrum::onSaveRequested() {
    fileIO->saveFile(editor->toPlainText());
    editor->document()->setModified(false);
}

void Spectrum::onSaveAsRequested() {
    fileIO->saveFileAs(editor->toPlainText());
    editor->document()->setModified(false);
}
