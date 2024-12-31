#pragma once
#include "SyntaxHighlighter.h"
#include <QtWidgets/QMainWindow>

class Spectrum : public QMainWindow
{
    Q_OBJECT

public:
    Spectrum(QWidget* parent = nullptr);
    ~Spectrum();

private:
    QMenuBar* menu{};
    SyntaxHighlighter* editor{};
    QString currentFile{};

private slots:
    void newFile();
    void openFile();
    bool saveFile();
    void saveFileAs();
    bool maybeSave();
};
