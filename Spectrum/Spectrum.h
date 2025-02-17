#pragma once

#include "SPFolders.h"
#include "SPEditor.h"
#include "SPTerminal.h"
#include "SPHighlighter.h"
#include "SPMenu.h"

#include <QMainWindow>


class Spectrum : public QMainWindow
{
    Q_OBJECT

public:
    Spectrum(QWidget* parent = nullptr);
    ~Spectrum();

private:
    AlifMenuBar* menu{};
    SPEditor* editor{};
    Terminal* terminal{};
    FolderTree* folderTree{};
};
