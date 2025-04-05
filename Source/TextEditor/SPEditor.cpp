#include "SPEditor.h"

#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QMimeData>

SPEditor::SPEditor(QWidget* parent) {
    setAcceptDrops(true);
    this->setStyleSheet("QPlainTextEdit { background-color: #141520; color: #cccccc;}");
    this->setTabStopDistance(32);

    // set "force" cursor and text direction from right to left
    QTextDocument* editorDocument = this->document();
    QTextOption option = editorDocument->defaultTextOption();
    option.setTextDirection(Qt::RightToLeft);
    option.setAlignment(Qt::AlignRight);
    editorDocument->setDefaultTextOption(option);


    highlighter = new SyntaxHighlighter(editorDocument);
    autoComplete = new AutoComplete(this, parent);
    lineNumberArea = new LineNumberArea(this);

    connect(this, &SPEditor::blockCountChanged, this, &SPEditor::updateLineNumberAreaWidth);
    connect(this, &SPEditor::updateRequest, this, &SPEditor::updateLineNumberArea);
    connect(this, &SPEditor::cursorPositionChanged, this, &SPEditor::highlightCurrentLine);

    updateLineNumberAreaWidth();
    highlightCurrentLine();

    // Handle special key events
    installEventFilter(this); // for SHIFT + ENTER it's make line without number
}

bool SPEditor::eventFilter(QObject* obj, QEvent* event) {
    if (obj == this and event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        // Handle Shift+Return or Shift+Enter
        if ((keyEvent->key() == Qt::Key_Return or keyEvent->key() == Qt::Key_Enter)
            and (keyEvent->modifiers() & Qt::ShiftModifier)) {

            return true; // Event handled
        }
    }

    return QPlainTextEdit::eventFilter(obj, event);
}

int SPEditor::lineNumberAreaWidth() const {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    // Increased width to accommodate line numbers
    int space = 21 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void SPEditor::updateLineNumberAreaWidth() {
    int width = lineNumberAreaWidth();
    // Set viewport margins to create space for line number area on the Left
    setViewportMargins(0, 0, width + 10, 0);
}

inline void SPEditor::updateLineNumberArea(const QRect &rect, int dy) {
    // Trigger a repaint of the line number area
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();
}

void SPEditor::resizeEvent(QResizeEvent* event) {
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    int areaWidth = lineNumberAreaWidth();
    // Position line number area on the Left
    lineNumberArea->setGeometry(QRect(
        cr.right() - areaWidth,
        cr.top(),
        areaWidth,
        cr.height()
    ));
}


void SPEditor::lineNumberAreaPaintEvent(QPaintEvent* event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::transparent);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() and top <= event->rect().bottom()) {
        if (block.isVisible() and bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(200, 200, 200));
            painter.drawText(12, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight | Qt::AlignVCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }

}



void SPEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(23, 24, 36, 240);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


/* ---------------------------------- Drag and Drop ---------------------------------- */

void SPEditor::dragEnterEvent(QDragEnterEvent* event) {
    // Check if the dragged data contains URLs (files)
    if (event->mimeData()->hasUrls()) {
        // Check if any of the URLs have a .alif ... extension
        for (const QUrl& url : event->mimeData()->urls()) {
            if (url.fileName().endsWith(".alif", Qt::CaseInsensitive) or
                url.fileName().endsWith(".aliflib", Qt::CaseInsensitive) or
                url.fileName().endsWith(".txt", Qt::CaseInsensitive)) {
                event->acceptProposedAction(); // Accept the drag event
                return;
            }
        }
    }
    event->ignore(); // Ignore if not a .alif ... file
}

void SPEditor::dropEvent(QDropEvent* event) {
    // Check if the dropped data contains URLs (files)
    if (event->mimeData()->hasUrls()) {
        for (const QUrl& url : event->mimeData()->urls()) {
            if (url.fileName().endsWith(".alif", Qt::CaseInsensitive) or
                url.fileName().endsWith(".aliflib", Qt::CaseInsensitive) or
                url.fileName().endsWith(".txt", Qt::CaseInsensitive)) {

                QString filePath = url.toLocalFile();
                emit openRequest(filePath);

                event->acceptProposedAction();
                return;
            }
        }
    }
    event->ignore(); // Ignore if not a .alif ... file
}

void SPEditor::dragMoveEvent(QDragMoveEvent* event) { // ضروري لمنع ظهور سلوك غريب بعد الإفلات
    event->acceptProposedAction();
}

void SPEditor::dragLeaveEvent(QDragLeaveEvent* event) {
    event->accept();
}

