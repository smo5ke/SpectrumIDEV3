#pragma once

#include <QtWidgets/QMainWindow>

class Spectrum : public QMainWindow
{
    Q_OBJECT

public:
    Spectrum(QWidget* parent = nullptr);
    ~Spectrum();

private:
    QMenuBar* menu{};

private slots:
    void newFile();
    void openFile();
    void saveFile();


};
