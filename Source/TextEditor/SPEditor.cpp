#include "SPEditor.h"

#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QMimeData>

SPEditor::SPEditor(QWidget* parent) {

    setAcceptDrops(true);
    this->setAcceptRichText(false);
    this->setStyleSheet("QTextEdit { background-color: #141520; color: #cccccc;}");
    this->setTabStopDistance(32);

    // set "force" cursor and text direction from right to left
    QTextDocument* editorDocument = this->document();
    QTextOption option = editorDocument->defaultTextOption();
    option.setTextDirection(Qt::RightToLeft);
    editorDocument->setDefaultTextOption(option);

    highlighter = new SyntaxHighlighter(editorDocument);
    autoComplete = new AutoComplete(this, parent);
    lineNumberArea = new LineNumberArea(this);

    updateLineNumberAreaWidth();
    connect(this, &QTextEdit::textChanged, this, [this]() {
        updateLineNumberAreaWidth();
    });

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

    return QTextEdit::eventFilter(obj, event);
}

int SPEditor::lineNumberAreaWidth() const {
    int digits = 1;
    int max = qMax(1, document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    QFont font{};
    font.setPointSize(10); // most be same lineNumberAreaPaintEvent() font PointSize
    QFontMetrics fm(font);

    // Increased width to accommodate line numbers
    int space = 21 + fm.horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void SPEditor::updateLineNumberAreaWidth() {
    int width = lineNumberAreaWidth();

    // Set viewport margins to create space for line number area on the Left
    setViewportMargins(0, 0, width + 10, 0);
}

void SPEditor::resizeEvent(QResizeEvent* event) {
    QTextEdit::resizeEvent(event);

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

    lineNumberArea->setStyleSheet(
        "QWidget {"
        "   border-left: 1px solid #10a8f4;"
        "   border-top-left-radius: 9px;"        // Rounded top-left corner
        "   border-bottom-left-radius: 9px;"     // Rounded bottom-left corner
        "}"
        );

    // Set font size
    QFont font = QFont("KawkabMono");
    font.setPointSize(10);
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

        QString number = QString::number(blockNumber + 1);

        // Number color
        painter.setPen(QColor(200, 200, 200));

        // Calculate text width
        painter.drawText(12, blockTop, lineNumberArea->width(),
            fontMetrics().height(),
            Qt::AlignRight | Qt::AlignVCenter, number);

        block = block.next();
        ++blockNumber;
    }
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

