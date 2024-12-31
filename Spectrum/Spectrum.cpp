#include "Spectrum.h"
#include "SyntaxHighlighter.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

Spectrum::Spectrum(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(" Spectrum V3 - Alif5 Editor");
    this->setGeometry(800, 400, 600, 400);

    QWidget* center = new QWidget(this);
    QVBoxLayout* vlay = new QVBoxLayout(center);
    vlay->setContentsMargins(0, 0, 0, 0);



    menu = this->menuBar();
    QMenu* fileMenu = menu->addMenu("ملف");

    QAction* newAction = new QAction("جديد", this);
    QAction* openAction = new QAction("فتح", this);
    QAction* saveAction = new QAction("حفظ", this);

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    //fileMenu->addSeparator();
    fileMenu->addAction(saveAction);




    SyntaxHighlighter* editor = new SyntaxHighlighter(center);
    editor->setTabStopDistance(32);
    editor->setStyleSheet("QTextEdit { background-color: #151729; color: #ffffff; }");
    editor->setFont(QFont("Tajawal", 12, 500));

    // set "force" cursor and text direction from right to left
    QTextDocument* document = editor->document();
    QTextOption option = document->defaultTextOption();
    option.setTextDirection(Qt::RightToLeft);
    document->setDefaultTextOption(option);




    
    
    vlay->addWidget(editor);

    this->setCentralWidget(center);


    connect(newAction, &QAction::triggered, this, &Spectrum::newFile);
}

Spectrum::~Spectrum()
{}



void Spectrum::newFile() {
    QMessageBox::information(this, tr("New File"), tr("New file action triggered"));
}

void Spectrum::openFile() {

}

void Spectrum::saveFile() {

}