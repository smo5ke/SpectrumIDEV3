#include "SyntaxHighlighter.h"
#include "AlifEditor.h"

#include <QPainter>
#include <qabstracttextdocumentlayout.h>
#include <qrect.h>
#include <QTextBlock>
#include <QScrollBar>


AlifEditor::AlifEditor(QWidget* parent) {

    this->setTabStopDistance(32);
    this->setAcceptRichText(true);
    this->setStyleSheet("QTextEdit { background-color: #141520; color: #dddddd;}");
    this->setFont(QFont("Tajawal", 12, 500));

    // set "force" cursor and text direction from right to left
    QTextDocument* editorDocument = this->document();
    QTextOption option = editorDocument->defaultTextOption();
    option.setTextDirection(Qt::RightToLeft);
    editorDocument->setDefaultTextOption(option);


    highlighter = new SyntaxHighlighter(editorDocument);
    setupHighlighter();



    lineNumberArea = new LineNumberArea(this);

    connect(this, &QTextEdit::textChanged, this, [this]() {
        updateLineNumberAreaWidth(0);
        updateLineNumberArea();
        });
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
        this, &AlifEditor::updateLineNumberArea);

    // Handle special key events
    installEventFilter(this);

    updateLineNumberAreaWidth(0);
}



void AlifEditor::setupHighlighter() {
    // highlighting rules

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor("#00d6e8"));
    highlighter->addHighlightingRule("(?:^|(?<!\\p{Arabic}[\\w_-]))(ك|و|في|او|أو|من|مع|صح|هل|اذا|إذا|ليس|مرر|عدم|ولد|صنف|خطا|خطأ|عام|احذف|دالة|لاجل|لأجل|والا|وإلا|توقف|نطاق|ارجع|اواذا|أوإذا|بينما|انتظر|استمر|مزامنة|استورد)(?![\\p{Arabic}\\w_-])", keywordFormat, SyntaxHighlighter::NormalState);

    QTextCharFormat staticClassFormat;
    staticClassFormat.setForeground(QColor("#f1a332"));
    highlighter->addHighlightingRule("(?:^|(?<!\\p{Arabic}[\\w_-]))(مدى|اطبع|ادخل)(?![\\p{Arabic}\\w_-])", staticClassFormat, SyntaxHighlighter::NormalState);

    QTextCharFormat numbersFormat;
    numbersFormat.setForeground(QColor("#cc3beb"));
    highlighter->addHighlightingRule("(?<![\\p{Arabic}\\w])\\d+(?![\\p{Arabic}\\w])", numbersFormat, SyntaxHighlighter::NormalState);

    QTextCharFormat stringFormat;
    stringFormat.setForeground(Qt::green);
    highlighter->addHighlightingRule("\".*\"", stringFormat, SyntaxHighlighter::StringState);

    QTextCharFormat formattedContentFormat;
    formattedContentFormat.setForeground(Qt::white); // White content inside braces
    highlighter->addHighlightingRule("{(.*?)}", formattedContentFormat, SyntaxHighlighter::NormalState);

    QTextCharFormat formattedBraceFormat;
    formattedBraceFormat.setForeground(QColor("#3985e0")); // curly braces
    highlighter->addHighlightingRule("({)", formattedBraceFormat, SyntaxHighlighter::NormalState);
    highlighter->addHighlightingRule("(})", formattedBraceFormat, SyntaxHighlighter::NormalState);

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::gray);
    highlighter->addHighlightingRule("#.*", commentFormat, SyntaxHighlighter::CommentState);
}


bool AlifEditor::eventFilter(QObject* obj, QEvent* event) {
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



int AlifEditor::lineNumberAreaWidth() const {
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
    int space = 12 + fm.horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void AlifEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    int width = lineNumberAreaWidth();

    // Set viewport margins to create space for line number area on the RIGHT
    setViewportMargins(0, 0, width, 0);

    // Adjust cursor position
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
}

void AlifEditor::updateLineNumberArea() {
    // Trigger a repaint of the line number area
    lineNumberArea->update();
}

void AlifEditor::resizeEvent(QResizeEvent* event) {
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





void AlifEditor::lineNumberAreaPaintEvent(QPaintEvent* event) {
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
            int numberWidth = painter.fontMetrics().horizontalAdvance(number);

            // Draw line number aligned to the right of the number bar
            painter.drawText(
                lineNumberArea->width() - numberWidth - 7,  // X position
                blockTop,
                numberWidth + 10,  // Width
                fontMetrics().height(),
                Qt::AlignRight | Qt::AlignVCenter,
                number
            );
        }

        block = block.next();
        ++blockNumber;
    }
}

QTextBlock AlifEditor::firstVisibleBlock() const {
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

QRect AlifEditor::blockBoundingRect(const QTextBlock& block) const {
    QTextLayout* layout = block.layout();
    if (layout) {
        return QRect(0, layout->position().y(),
            viewport()->width(),
            layout->boundingRect().height());
    }
    return QRect();
}

