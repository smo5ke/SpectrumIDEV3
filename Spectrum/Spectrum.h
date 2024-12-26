#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Spectrum.h"

class Spectrum : public QMainWindow
{
    Q_OBJECT

public:
    Spectrum(QWidget* parent = nullptr);
    ~Spectrum();

private:
    Ui::SpectrumClass ui;
};
