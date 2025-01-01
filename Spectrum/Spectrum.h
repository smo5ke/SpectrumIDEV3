#pragma once
#include "AlifEditor.h"
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
    AlifEditor* editor{};
    QString currentFile{};

private slots:
    void newFile();
    void openFile();
    bool saveFile();
    void saveFileAs();
    bool maybeSave();
};
