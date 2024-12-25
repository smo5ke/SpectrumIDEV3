#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SpectrumIDEV3.h"

class SpectrumIDEV3 : public QMainWindow
{
    Q_OBJECT

public:
    SpectrumIDEV3(QWidget *parent = nullptr);
    ~SpectrumIDEV3();

private:
    Ui::SpectrumIDEV3Class ui;
};
