#include "SPHighlighter.h"
#include "SPEditor.h"

#include <QPainter>
#include <qabstracttextdocumentlayout.h>
#include <qrect.h>
#include <QTextBlock>
#include <QScrollBar>


SPEditor::SPEditor(QWidget* parent) {

    this->setTabStopDistance(32);
    this->setAcceptRichText(true);
    this->setStyleSheet("QTextEdit { background-color: #141520; color: #cccccc;}");
    this->setFont(QFont("Tajawal", 12, 500));

    // set "force" cursor and text direction from right to left
    QTextDocument* editorDocument = this->document();
    QTextOption option = editorDocument->defaultTextOption();
    option.setTextDirection(Qt::RightToLeft);
    editorDocument->setDefaultTextOption(option);

    highlighter = new SyntaxHighlighter(editorDocument);
    autoComplete = new AutoComplete(this, parent);

    lineNumberArea = new LineNumberArea(this);
    connect(this, &QTextEdit::textChanged, this, [this]() {
        updateLineNumberAreaWidth(0);
        updateLineNumberArea();
        });
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
        this, &SPEditor::updateLineNumberArea);

    // Handle special key events
    installEventFilter(this);

    updateLineNumberAreaWidth(0);
}


bool SPEditor::eventFilter(QObject* obj, QEvent* event) {
    if (obj == this && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        // Handle Shift+Return or Shift+Enter
        if ((keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            && (keyEvent->modifiers() & Qt::ShiftModifier)) {

            // Insert new line
            QTextCursor cursor = textCursor();
            cursor.insertText("\n");

            // Trigger updates
            updateLineNumberAreaWidth(0);
            updateLineNumberArea();

            return true; // Event handled
        }
    }

    return QTextEdit::eventFilter(obj, event);
}



int SPEditor::lineNumberAreaWidth() const {
    int digits = 1;
    int max = qMax(1, document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    QFont font;
    font.setPointSize(12);
    QFontMetrics fm(font);

    // Increased width to accommodate line numbers
    int space = 18 + fm.horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void SPEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    int width = lineNumberAreaWidth();

    // Set viewport margins to create space for line number area on the RIGHT
    setViewportMargins(0, 0, width, 0);

    // Adjust cursor position
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
}

void SPEditor::updateLineNumberArea() {
    // Trigger a repaint of the line number area
    lineNumberArea->update();
}

void SPEditor::resizeEvent(QResizeEvent* event) {
    QTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    // Position line number area on the RIGHT
    lineNumberArea->setGeometry(QRect(
        cr.right() - lineNumberAreaWidth(),
        cr.top(),
        lineNumberAreaWidth(),
        cr.height()
    ));
}





void SPEditor::lineNumberAreaPaintEvent(QPaintEvent* event) {
    QPainter painter(lineNumberArea);

    // Background color
    painter.fillRect(event->rect(), QColor("#141520"));

    // Set font size
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    // Get vertical scroll bar value
    int scrollValue = verticalScrollBar()->value();

    // Get document layout
    QAbstractTextDocumentLayout* docLayout = document()->documentLayout();

    QTextBlock block = document()->begin();
    int blockNumber = 0;

    while (block.isValid()) {
        // Get block's bounding rectangle
        QRectF blockRect = docLayout->blockBoundingRect(block);

        // Calculate the vertical position of the block
        int blockTop = qRound(blockRect.top() - scrollValue);

        // Check if block is within the visible area
        if (blockTop + blockRect.height() >= 0 && blockTop <= height()) {
            QString number = QString::number(blockNumber + 1);

            // Number color #dddddd
            painter.setPen(QColor(221, 221, 221));

            // Calculate text width
            painter.drawText(12, blockTop, lineNumberArea->width() - 12,
                fontMetrics().height(),
                Qt::AlignRight | Qt::AlignVCenter, number);
        }

        block = block.next();
        ++blockNumber;
    }
}

QTextBlock SPEditor::firstVisibleBlock() const {
    // Get the vertical scroll bar value
    int scrollValue = verticalScrollBar()->value();

    // Iterate through blocks to find the first visible block
    QTextBlock block = document()->begin();
    while (block.isValid()) {
        QRect rect = cursorRect(QTextCursor(block));

        // Check if this block is below the scroll position
        if (rect.top() >= scrollValue) {
            return block;
        }

        block = block.next();
    }

    // Fallback to first block if no block is found
    return document()->begin();
}

QRect SPEditor::blockBoundingRect(const QTextBlock& block) const {
    QTextLayout* layout = block.layout();
    if (layout) {
        return QRect(0, layout->position().y(),
            viewport()->width(),
            layout->boundingRect().height());
    }
    return QRect();
}


