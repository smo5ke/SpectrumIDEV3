#include "AlifMenu.h"

#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qshortcut.h>

AlifMenuBar::AlifMenuBar(QMenuBar* parent, AlifEditor* editorPtr) {

    editorRef = editorPtr;

    QMenu* fileMenu = parent->addMenu("ملف");

    QAction* newAction = new QAction("جديد", parent);
    QAction* openAction = new QAction("فتح", parent);
    QAction* saveAction = new QAction("حفظ", parent);
    QAction* saveAsAction = new QAction("حفظ باسم", parent);

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    //fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);





    // Create a shortcut for Ctrl+S
    QShortcut* saveShortcut = new QShortcut(QKeySequence::Save, parent);
    connect(saveShortcut, &QShortcut::activated, this, &AlifMenuBar::saveFile);



    connect(newAction, &QAction::triggered, this, &AlifMenuBar::newFile);
    connect(openAction, &QAction::triggered, this, &AlifMenuBar::openFile);
    connect(saveAction, &QAction::triggered, this, &AlifMenuBar::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &AlifMenuBar::saveFileAs);
}




void AlifMenuBar::newFile() {
    if (maybeSave()) {
        // Clear the text edit widget to start a new file
        editorRef->clear();
        currentFile.clear();
    }
}

void AlifMenuBar::openFile() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("فتح ملف"), "", tr("ملف ألف (*.alif);;All Files (*)"));
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                editorRef->setPlainText(in.readAll());
                file.close();
                currentFile = fileName;
                editorRef->document()->setModified(false);
            }
            else {
                QMessageBox::warning(this, tr("خطأ"), tr("لا يمكن فتح الملف"));
            }
        }
    }
}

bool AlifMenuBar::saveFile() {
    if (currentFile.isEmpty()) {
        saveFileAs();
        return true;
    }
    else {
        QFile file(currentFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << editorRef->toPlainText();
            file.close();
            editorRef->document()->setModified(false);
        }
        else {
            QMessageBox::warning(this, tr("خطأ"), tr("لا يمكن حفظ الملف"));
            return false;
        }
    }
    return false;
}



void AlifMenuBar::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("حفظ الملف"), "", tr("ملف ألف (*.alif);;All Files (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << editorRef->toPlainText();
            file.close();
            currentFile = fileName;
            editorRef->document()->setModified(false);
        }
        else {
            QMessageBox::warning(this, tr("خطأ"), tr("لا يمكن حفظ الملف"));
        }
    }
}


bool AlifMenuBar::maybeSave() {
    if (editorRef->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("ألف"),
            tr("تم التعديل على الملف.\n"
                "هل تريد حفظ التغييرات؟"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile();
        }
        else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}


