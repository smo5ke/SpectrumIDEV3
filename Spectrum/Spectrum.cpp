#include "Spectrum.h"


#include <QVBoxLayout>
#include <QScreen>


Spectrum::Spectrum(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(" Spectrum V3 - Alif5 Editor");
    QScreen* screenSize = QGuiApplication::primaryScreen();
    this->setGeometry(screenSize->size().width() / 8, screenSize->size().height() / 8, 800, 600);

    QWidget* center = new QWidget(this);
    QVBoxLayout* vlay = new QVBoxLayout(center);
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);


    editor = new AlifEditor(center);
    menu = new AlifMenuBar(this->menuBar(), editor);
    terminal = new Terminal(center);

    
    
    vlay->addWidget(editor);
    vlay->addWidget(terminal);

    this->setCentralWidget(center);
}

Spectrum::~Spectrum()
{}
