#pragma once

#include "AlifFolders.h"
#include "AlifEditor.h"
#include "AlifTerminal.h"
#include "SyntaxHighlighter.h"
#include "AlifMenu.h"

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
    FolderTree* folderTree{};
};
