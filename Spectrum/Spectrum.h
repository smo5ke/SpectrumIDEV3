#pragma once

//#include "SPFolders.h"
#include "SPEditor.h"
//#include "SPTerminal.h"
#include "SPHighlighter.h"
#include "SPMenu.h"
#include "SPFileIO.h"

#include <QMainWindow>


class Spectrum : public QMainWindow
{
    Q_OBJECT

public:
    Spectrum(const QString& filePath = "", QWidget* parent = nullptr);
    ~Spectrum();



private slots:
    void onNewRequested();
    void onOpenRequested(QString filePath);
    void onSaveRequested();
    void onSaveAsRequested();


private:
    SPEditor* editor{};
    SPMenuBar* menuBar{};
    SPFileIO* fileIO{};
};
