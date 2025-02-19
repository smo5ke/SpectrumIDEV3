#pragma once

#include <qmenubar.h>
#include <qfilesystemmodel.h>
#include <qtreeview.h>



class SPMenuBar : public QMenuBar {

	Q_OBJECT
public:
    SPMenuBar(QWidget* parent = nullptr);


signals:
    void newRequested();
    void openRequested();
    void saveRequested();
    void saveAsRequested();

private slots:
    void onNewAction() {
        emit newRequested();
    }
    void onOpenAction() {
        emit openRequested();
    }
    void onSaveAction() {
        emit saveRequested();
    }
    void onSaveAsAction() {
        emit saveAsRequested();
    }
};