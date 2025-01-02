#pragma once

#include "AlifMenu.h"
#include "AlifEditor.h"
#include "AlifTerminal.h"

#include <QMainWindow>

class Spectrum : public QMainWindow
{
    Q_OBJECT

public:
    Spectrum(QWidget* parent = nullptr);
    ~Spectrum();

private:
    AlifMenuBar* menu{};
    AlifEditor* editor{};
    Terminal* terminal{};
};
