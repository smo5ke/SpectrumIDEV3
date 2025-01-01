#include "Spectrum.h"

#include <QVBoxLayout>
#include <QScreen>


Spectrum::Spectrum(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(" Spectrum V3 - Alif5 Editor");
    QScreen* screenSize = QGuiApplication::primaryScreen();
    this->setGeometry(screenSize->size().width() / 4, screenSize->size().height() / 4, 600, 400);

    QWidget* center = new QWidget(this);
    QVBoxLayout* vlay = new QVBoxLayout(center);
    vlay->setContentsMargins(0, 0, 0, 0);


    editor = new AlifEditor(center);
    menu = new AlifMenuBar(this->menuBar(), editor);
    
    
    vlay->addWidget(editor);

    this->setCentralWidget(center);
}

Spectrum::~Spectrum()
{}
