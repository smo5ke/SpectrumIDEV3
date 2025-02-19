#include "SPFileIO.h"

void SPFileIO::newFile() {
    emit checkEditorModified();
    currentFile.clear();
}

QString SPFileIO::openFile(QString filePath) {
    emit checkEditorModified();

    QString content{};
    filePath.isEmpty() ? filePath = QFileDialog::getOpenFileName(nullptr, "فتح ملف", "", "ملف ألف (*.alif);;All Files (*)") : filePath;
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            content = in.readAll();
            file.close();
            currentFile = filePath;
        }
        else {
            QMessageBox::warning(nullptr, "خطأ", "لا يمكن فتح الملف");
        }
    }
    return content;
}

bool SPFileIO::saveFile(const QString& content) {
    if (currentFile.isEmpty()) {
        saveFileAs(content);
        return true;
    }
    else {
        QFile file(currentFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << content;
            file.close();

        }
        else {
            QMessageBox::warning(nullptr, "خطأ", "لا يمكن حفظ الملف");
            return false;
        }
    }
    return false;
}



void SPFileIO::saveFileAs(const QString& content) {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "حفظ الملف", "", "ملف ألف (*.alif);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << content;
            file.close();
            currentFile = fileName;
        }
        else {
            QMessageBox::warning(nullptr, "خطأ", "لا يمكن حفظ الملف");
        }
    }
}


void SPFileIO::onEditorModified(const QString& content) {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(nullptr, "ألف",
        "تم التعديل على الملف.\n"
        "هل تريد حفظ التغييرات؟",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save) {
        saveFile(content);
    }
}