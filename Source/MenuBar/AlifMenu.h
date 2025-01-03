#pragma once

#include <qmenubar.h>
#include <qfilesystemmodel.h>
#include <qtreeview.h>



class AlifMenuBar : public QMenuBar {

	Q_OBJECT
public:
	AlifMenuBar(QMenuBar* parent = nullptr, class AlifEditor* editorPtr = nullptr, class FolderTree* folderTreePtr = nullptr);


private:
    class AlifEditor* editorRef{};
    FolderTree* folderTreeRef{};
    QString currentFile{};

private slots:
    void newFile();
    void openFile();
    bool saveFile();
    void saveFileAs();
    bool maybeSave();
};