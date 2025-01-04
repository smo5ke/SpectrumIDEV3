#pragma once

#include <qtextedit.h>


class LineNumberArea;

class AlifEditor : public QTextEdit {
	Q_OBJECT

public:
	AlifEditor(QWidget* parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth() const;
    QTextBlock firstVisibleBlock() const;
    QRect blockBoundingRect(const QTextBlock& block) const;

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
	class SyntaxHighlighter* highlighter{};
    LineNumberArea* lineNumberArea{};

	void setupHighlighter();

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea();
};


class LineNumberArea : public QWidget {
public:
    LineNumberArea(AlifEditor* editor) : QWidget(editor), alifEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(alifEditor->lineNumberAreaWidth(), height());
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        alifEditor->lineNumberAreaPaintEvent(event);
    }

private:
    AlifEditor* alifEditor;
};