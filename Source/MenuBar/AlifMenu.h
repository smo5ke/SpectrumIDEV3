#pragma once

#include "AlifEditor.h"

#include <qmenubar.h>



class AlifMenuBar : public QMenuBar {

	Q_OBJECT
public:
	AlifMenuBar(QMenuBar* parent = nullptr, AlifEditor* editorPtr = nullptr);


private:
    AlifEditor* editorRef{};
    QString currentFile{};

private slots:
    void newFile();
    void openFile();
    bool saveFile();
    void saveFileAs();
    bool maybeSave();

};