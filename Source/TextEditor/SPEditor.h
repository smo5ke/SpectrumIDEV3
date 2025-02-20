#pragma once

#include "SPHighlighter.h"
#include "AlifComplete.h"
#include <qtextedit.h>


class LineNumberArea;

class SPEditor : public QTextEdit {
	Q_OBJECT

public:
	SPEditor(QWidget* parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth() const;
    QTextBlock firstVisibleBlock() const;
    QRect blockBoundingRect(const QTextBlock& block) const;

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;

private:
    SyntaxHighlighter* highlighter{};
    LineNumberArea* lineNumberArea{};
    AutoComplete* autoComplete{};

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea();


signals:
    void openRequest(QString filePath);
};


class LineNumberArea : public QWidget {
public:
    LineNumberArea(SPEditor* editor) : QWidget(editor), spEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(spEditor->lineNumberAreaWidth(), height());
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        spEditor->lineNumberAreaPaintEvent(event);
    }

private:
    SPEditor* spEditor;
};