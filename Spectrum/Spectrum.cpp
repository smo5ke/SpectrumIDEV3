#include "Spectrum.h"

#include <QDockWidget>
#include <QVBoxLayout>
#include <QScreen>
#include <QShortcut>
#include <QMessageBox>
#include <QFileDialog>


Spectrum::Spectrum(const QString& filePath, QWidget *parent)
    : QMainWindow(parent)
{
    QScreen* screenSize = QGuiApplication::primaryScreen();
    this->setGeometry(screenSize->size().width() / 3, screenSize->size().height() / 7, 500, 600);
    setStyleSheet(R"(
            QMainWindow::separator {
                background-color: #2a2c44;
                width: 4px;
                height: 4px;
            }
            
            QMainWindow::separator:hover {
                background-color: #393c5d;
            })");

    QWidget* center = new QWidget(this);
    QVBoxLayout* vlay = new QVBoxLayout(center);
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    editor = new SPEditor(center);
    //terminal = new Terminal(this);
    //folderTree = new FolderTree(editor, this);
    menuBar = new SPMenuBar(this);
    setMenuBar(menuBar);

    updateWindowTitle();

    vlay->addWidget(editor);
    //vlay->addWidget(terminal);


    //addDockWidget(Qt::BottomDockWidgetArea, terminal); // يجب أن تكون بعد vlay->addWidget(terminal)
    //addDockWidget(Qt::RightDockWidgetArea, folderTree);
    this->setCentralWidget(center);

    // لتشغيل ملف ألف بإستخدام محرر طيف عند إختيار المحرر ك برنامج للتشغيل
    if (!filePath.isEmpty()) {
        this->openFile(filePath);
    }

    connect(menuBar, &SPMenuBar::newRequested, this, &Spectrum::newFile);
    connect(menuBar, &SPMenuBar::openRequested, this, [this](){this->openFile("");});
    connect(menuBar, &SPMenuBar::saveRequested, this, &Spectrum::saveFile);
    connect(menuBar, &SPMenuBar::saveAsRequested, this, &Spectrum::saveFileAs);
    connect(editor, &SPEditor::openRequest, this, [this](QString filePath){this->openFile(filePath);});

    // Connect modification signal so when doc modified it's add "*"
    connect(editor->document(), &QTextDocument::modificationChanged,
            this, &Spectrum::onModificationChanged);
}

Spectrum::~Spectrum()
{}






void Spectrum::newFile() {
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(nullptr, "ألف",
            "تم التعديل على الملف.\n"
            "هل تريد حفظ التغييرات؟",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            this->saveFile();
        }
        else if (ret == QMessageBox::Cancel) {
            return;
        }
    }

    currentFile.clear();
    // editor->clear();
    editor->document()->setPlainText("");
    editor->document()->setModified(false);
    updateWindowTitle();
}

void Spectrum::openFile(QString filePath) {
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(nullptr, "ألف",
            "تم التعديل على الملف.\n"
            "هل تريد حفظ التغييرات؟",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            this->saveFile();
        }
        else if (ret == QMessageBox::Cancel) {
            return;
        }
    }


    filePath.isEmpty() ? filePath = QFileDialog::getOpenFileName(nullptr, "فتح ملف", "", "ملف ألف (*.alif *.aliflib);;All Files (*)") : filePath;
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            editor->document()->setPlainText(content);
            file.close();
            currentFile = filePath;
        }
        else {
            QMessageBox::warning(nullptr, "خطأ", "لا يمكن فتح الملف");
        }
    }

    editor->document()->setModified(false);
    updateWindowTitle();
}

void Spectrum::saveFile() {
    QString content = editor->document()->toPlainText();
    if (currentFile.isEmpty()) {
        saveFileAs();
    }
    else {
        QFile file(currentFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << content;
            file.close();
            editor->document()->setModified(false);
            updateWindowTitle();
        }
        else {
            QMessageBox::warning(nullptr, "خطأ", "لا يمكن حفظ الملف");
        }
    }
}


void Spectrum::saveFileAs() {
    QString content = editor->document()->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(nullptr, "حفظ الملف", "ملف جديد", "ملف ألف (*.alif);;مكتبة ألف(*.aliflib);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << content;
            file.close();
            currentFile = fileName;
            editor->document()->setModified(false);
            updateWindowTitle();
        }
        else {
            QMessageBox::warning(nullptr, "خطأ", "لا يمكن حفظ الملف");
        }
    }
}


void Spectrum::updateWindowTitle() {
    QString title{};
    if (currentFile.isEmpty()) {
        title = "غير معنون[*]";
    } else {
        title = QFileInfo(currentFile).fileName() + "[*]";
    }
    title += " - طيف";
    setWindowTitle(title);
}

void Spectrum::onModificationChanged(bool modified) {
    this->setWindowModified(modified);
}










