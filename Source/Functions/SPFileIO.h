#pragma once

#include "SPEditor.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QString>
#include <QObject>
#include <QFileDialog>
#include <QMessageBox>


class SPFileIO : public QObject {
    Q_OBJECT

public:
    SPFileIO(QObject* parent = nullptr) : QObject(parent) {}

public:
    void newFile();
    QString openFile(QString fileName);
    bool saveFile(const QString& content);
    void saveFileAs(const QString& content);



private:
    QString currentFile{};
};