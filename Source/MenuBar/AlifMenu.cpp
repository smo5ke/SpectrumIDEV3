#include "AlifFolders.h"
#include "AlifEditor.h"
#include "AlifMenu.h"


#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qshortcut.h>
#include <QModelIndex>


AlifMenuBar::AlifMenuBar(QMenuBar* parent, AlifEditor* editorPtr, FolderTree* folderTreePtr) {

    parent->setStyleSheet(R"(
        QMenuBar {
            color: #dddddd;
            background-color: #1e202e;
            spacing: 5px; /* spacing between menu bar items */
        }

        QMenuBar::item {
            padding: 5px 9px;
            background: transparent;
            border-radius: 3px;
        }

        QMenuBar::item:selected { /* when selected using mouse or keyboard */
            background: #303349;
        }

        QMenuBar::item:pressed {
            background: #373a54;
        }
    )");

    editorRef = editorPtr;
    folderTreeRef = folderTreePtr;

    QMenu* fileMenu = parent->addMenu("ملف");
    QMenu* editMenu = parent->addMenu("تحرير");
    QMenu* runMenu = parent->addMenu("تشغيل");
    QMenu* helpMenu = parent->addMenu("مساعدة");

    fileMenu->setMinimumWidth(200);
    editMenu->setMinimumWidth(200);
    runMenu->setMinimumWidth(200);
    helpMenu->setMinimumWidth(200);

    QAction* folderAction = new QAction("فتح مجلد", parent);
    QAction* newAction = new QAction("جديد", parent);
    QAction* openAction = new QAction("فتح", parent);
    QAction* saveAction = new QAction("حفظ", parent);
    QAction* saveAsAction = new QAction("حفظ باسم", parent);
    QAction* optionsAction = new QAction("خيارات", parent);
    QAction* exitAction = new QAction("خروج", parent);

    QAction* aboutAction = new QAction("عن المحرر", parent);


    fileMenu->addAction(folderAction);
    fileMenu->addSeparator();
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(optionsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    helpMenu->addAction(aboutAction);


    QString style = R"(
        QMenu {
            color: #dddddd;
            background-color: #1e202e;
            border: 1px solid #dddddd;
            border-radius: 10px 10px 10px 10px;
        }
        QMenu::item {
            border: 7px solid transparent;
        }
        QMenu::separator {
            height: 1px;
            background: #303349;
            margin-left: 15px;
            margin-right: 10px;
        }
        QMenu::indicator {
            width: 1px;
            height: 1px;
        }
)";
    fileMenu->setStyleSheet(style);
    editMenu->setStyleSheet(style);
    runMenu->setStyleSheet(style);
    helpMenu->setStyleSheet(style);





    // Create a shortcut for Ctrl+S
    QShortcut* saveShortcut = new QShortcut(QKeySequence::Save, parent);
    connect(saveShortcut, &QShortcut::activated, this, &AlifMenuBar::saveFile);

    connect(folderAction, &QAction::triggered, folderTreeRef, &FolderTree::openFolder);
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


