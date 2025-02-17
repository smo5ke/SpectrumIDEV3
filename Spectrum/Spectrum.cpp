#include "Spectrum.h"

#include <qdockwidget.h>
#include <QVBoxLayout>
#include <QScreen>


Spectrum::Spectrum(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(" Spectrum V3 - Alif5 Editor");
    QScreen* screenSize = QGuiApplication::primaryScreen();
    this->setGeometry(screenSize->size().width() / 8, screenSize->size().height() / 8, 800, 600);
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


    editor = new SPEditor(center);
    //terminal = new Terminal(this);
    //folderTree = new FolderTree(editor, this);
    menu = new AlifMenuBar(this->menuBar(), editor, folderTree);

    
    vlay->addWidget(editor);
    //vlay->addWidget(terminal);


    //addDockWidget(Qt::BottomDockWidgetArea, terminal); // يجب أن تكون بعد vlay->addWidget(terminal)
    //addDockWidget(Qt::RightDockWidgetArea, folderTree);
    this->setCentralWidget(center);
}

Spectrum::~Spectrum()
{}


