#include "Spectrum.h"
#include "SyntaxHighlighter.h"

#include <QVBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QScreen>


Spectrum::Spectrum(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(" Spectrum V3 - Alif5 Editor");
    QScreen* screenSize = QGuiApplication::primaryScreen();
    this->setGeometry(screenSize->size().width() / 4, screenSize->size().height() / 4, 800, 400);

    QWidget* center = new QWidget(this);
    QVBoxLayout* vlay = new QVBoxLayout(center);
    vlay->setContentsMargins(0, 0, 0, 0);



    menu = this->menuBar();
    QMenu* fileMenu = menu->addMenu("ملف");

    QAction* newAction = new QAction("جديد", this);
    QAction* openAction = new QAction("فتح", this);
    QAction* saveAction = new QAction("حفظ", this);
    QAction* saveAsAction = new QAction("حفظ باسم", this);

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    //fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);




    //editor = new SyntaxHighlighter(center);
    editor = new QTextEdit(center);
    editor->setTabStopDistance(32);
    editor->setAcceptRichText(true);
    editor->setStyleSheet("QTextEdit { background-color: #151729; color: #ffffff;}");
    editor->setFont(QFont("Tajawal", 12, 500));

    // set "force" cursor and text direction from right to left
    QTextDocument* document = editor->document();
    QTextOption option = document->defaultTextOption();
    option.setTextDirection(Qt::RightToLeft);
    document->setDefaultTextOption(option);

    highlighter = new SyntaxHighlighter(editor->document());
    setupHighlighter();


    
    
    vlay->addWidget(editor);

    this->setCentralWidget(center);


    connect(newAction, &QAction::triggered, this, &Spectrum::newFile);
    connect(openAction, &QAction::triggered, this, &Spectrum::openFile);
    connect(saveAction, &QAction::triggered, this, &Spectrum::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &Spectrum::saveFileAs);
}

Spectrum::~Spectrum()
{}



void Spectrum::newFile() {
    if (maybeSave()) {
        // Clear the text edit widget to start a new file
        editor->clear();
        currentFile.clear();
    }
}

void Spectrum::openFile() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("فتح ملف"), "", tr("ملف ألف (*.alif);;All Files (*)"));
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                editor->setPlainText(in.readAll());
                file.close();
                currentFile = fileName;
                editor->document()->setModified(false);
            }
            else {
                QMessageBox::warning(this, tr("خطأ"), tr("لا يمكن فتح الملف"));
            }
        }
    }
}

bool Spectrum::saveFile() {
    if (currentFile.isEmpty()) {
        saveFileAs();
        return true;
    }
    else {
        QFile file(currentFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << editor->toPlainText();
            file.close();
            editor->document()->setModified(false);
        }
        else {
            QMessageBox::warning(this, tr("خطأ"), tr("لا يمكن حفظ الملف"));
            return false;
        }
    }
    return false;
}


void Spectrum::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("حفظ الملف"), "", tr("ملف ألف (*.alif);;All Files (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << editor->toPlainText();
            file.close();
            currentFile = fileName;
            editor->document()->setModified(false);
        }
        else {
            QMessageBox::warning(this, tr("خطأ"), tr("لا يمكن حفظ الملف"));
        }
    }
}


bool Spectrum::maybeSave() {
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("ألف"),
            tr("تم التعديل على الملف.\n"
                "هل تريد حفظ التغييرات؟"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return Spectrum::saveFile();
        }
        else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}












void Spectrum::setupHighlighter() {
    // Example highlighting rules
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor("#00d6e8"));
    highlighter->addHighlightingRule("(?<!\w)(اذا|بينما|لاجل|في)(?!\w)", keywordFormat);

    QTextCharFormat stringFormat;
    stringFormat.setForeground(Qt::green);
    highlighter->addHighlightingRule("\".*\"", stringFormat);

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::gray);
    highlighter->addHighlightingRule("#.*", commentFormat);


    QTextCharFormat formattedContentFormat;
    formattedContentFormat.setForeground(Qt::white); // White content inside braces
    highlighter->addHighlightingRule("\{(.*?)\}", formattedContentFormat);

    QTextCharFormat formattedBraceFormat;
    formattedBraceFormat.setForeground(QColor(255, 165, 0)); // Orange braces
    highlighter->addHighlightingRule("(\{)", formattedBraceFormat);
    highlighter->addHighlightingRule("(\})", formattedBraceFormat);
}