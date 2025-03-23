#include "AlifComplete.h"

#include <QVBoxLayout>

AutoComplete::AutoComplete(QPlainTextEdit* editor, QObject* parent)
    : QObject(parent), editor(editor) {
    keywords = {
        "اطبع", "اواذا", "لاجل", "لأجل", "استمر", "استورد", "ارجع", "اذا", "احذف", "ادخل",
        "أوإذا", "إذا",
        "بينما",
        "توقف",
        "خطا", "خطأ",
        "دالة",
        "صنف", "صح",
        "عدم",
        "ليس",
        "مرر", "مدى",
        "والا", "وإلا", 
        "هذا",
        "_تهيئة_",
    };

    popup = new QWidget(editor, Qt::ToolTip | Qt::FramelessWindowHint);
    listWidget = new QListWidget(popup);
    popup->setStyleSheet("QWidget { background-color: #242533; color: #cccccc;}");

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(listWidget);
    popup->setLayout(layout);

    editor->installEventFilter(this);

    connect(editor, &QPlainTextEdit::textChanged, this, &AutoComplete::showCompletion);
    connect(listWidget, &QListWidget::itemClicked, this, &AutoComplete::insertCompletion);
}



bool AutoComplete::eventFilter(QObject* obj, QEvent* event) {
    if (obj == editor and event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (popup->isVisible()) {
            switch (keyEvent->key()) {
            case Qt::Key_Tab:
            case Qt::Key_Return:
                insertCompletion();
                return true;
            case Qt::Key_Escape:
                hidePopup();
                return true;
            case Qt::Key_Up:
            case Qt::Key_Down:
                QCoreApplication::sendEvent(listWidget, event);
                return true;
            default:
                return false;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

QString AutoComplete::getCurrentWord() const
{
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
    return cursor.selectedText().trimmed();
}

void AutoComplete::showCompletion() {
    QString currentWord = getCurrentWord();
    if (currentWord.isEmpty() or currentWord.length() < 1) {
        hidePopup();
        return;
    }

    QStringList suggestions{};
    for (const QString& keyword : keywords) {
        if (keyword.startsWith(currentWord, Qt::CaseInsensitive)) {
            suggestions << keyword;
        }
    }

    if (!suggestions.isEmpty()) {
        listWidget->clear();
        listWidget->addItems(suggestions);
        listWidget->setCurrentRow(0);
        showPopup();
    }
    else {
        hidePopup();
    }
}

void AutoComplete::showPopup() {
    QTextCursor cursor = editor->textCursor();
    QRect rect = editor->cursorRect(cursor);
    QPoint pos = editor->viewport()->mapToGlobal(rect.bottomLeft());

    // Set the minimum width of the popup
    popup->setFixedSize(256, 128);
    int popupWidth = popup->width();
    int popupHeight = popup->height();

    // Adjust the position to the left bottom of the cursor
    pos.setX(pos.x() - popup->width());
    pos.setY(pos.y() + 2); // Add a small offset to avoid overlapping

    // Ensure the popup stays within the screen boundaries
    QScreen* screen = QGuiApplication::screenAt(pos);
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();

        // Adjust X position if the popup goes off the right edge of the screen
        if (pos.x() + popupWidth > screenGeometry.right()) {
            pos.setX(screenGeometry.right() - popupWidth);
        }

        // Adjust Y position if the popup goes off the bottom edge of the screen
        if (pos.y() + popupHeight > screenGeometry.bottom()) {
            pos.setY(screenGeometry.bottom() - popupHeight);
        }
    }

    // Set the popup size and position
    popup->move(pos);
    popup->show();
}

inline void AutoComplete::hidePopup() {
    popup->hide();
}

void AutoComplete::insertCompletion() {
    if (!popup->isVisible()) return;

    QListWidgetItem* item = listWidget->currentItem();
    if (!item) return;

    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(item->text());
    hidePopup();
}
