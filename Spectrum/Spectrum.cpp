#include "Spectrum.h"

#include <QDockWidget>
#include <QVBoxLayout>
#include <QScreen>
#include <QShortcut>


Spectrum::Spectrum(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(" Spectrum V3 - Alif5 Editor");
    QScreen* screenSize = QGuiApplication::primaryScreen();
    this->setGeometry(screenSize->size().width() / 5, screenSize->size().height() / 9, 400, 600);
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






    connect(menuBar, &SPMenuBar::newRequested, this, &Spectrum::onNewRequested);
    connect(menuBar, &SPMenuBar::openRequested, this, [this]() {this->onOpenRequested(""); });
    connect(menuBar, &SPMenuBar::saveRequested, this, &Spectrum::onSaveRequested);
    connect(menuBar, &SPMenuBar::saveAsRequested, this, &Spectrum::onSaveAsRequested);
    connect(fileIO, &SPFileIO::checkEditorModified, this, [this]() {
        editor->document()->isModified() ? fileIO->onEditorModified(editor->toPlainText()) : (void)0;
        });

    connect(editor, &SPEditor::openRequest, this, &Spectrum::onOpenRequested);
}

Spectrum::~Spectrum()
{}






void Spectrum::onNewRequested() {
    fileIO->newFile();
    editor->clear();
    editor->document()->setModified(false);
}

void Spectrum::onOpenRequested(QString filePath) {
    QString content = fileIO->openFile(filePath);
    editor->setPlainText(content);
    editor->document()->setModified(false);
}

void Spectrum::onSaveRequested() {
    fileIO->saveFile(editor->toPlainText());
    editor->document()->setModified(false);
}

void Spectrum::onSaveAsRequested() {
    fileIO->saveFileAs(editor->toPlainText());
    editor->document()->setModified(false);
}
