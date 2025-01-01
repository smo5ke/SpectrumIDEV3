#pragma once
#include "SyntaxHighlighter.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>

class Spectrum : public QMainWindow
{
    Q_OBJECT

public:
    Spectrum(QWidget* parent = nullptr);
    ~Spectrum();

private:
    QMenuBar* menu{};
    //SyntaxHighlighter* editor{};
    QTextEdit* editor{};
    SyntaxHighlighter* highlighter{};
    QString currentFile{};

    void setupHighlighter();

private slots:
    void newFile();
    void openFile();
    bool saveFile();
    void saveFileAs();
    bool maybeSave();
};
