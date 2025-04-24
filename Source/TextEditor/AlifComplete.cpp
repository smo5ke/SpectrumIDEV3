#include "AlifComplete.h"

#include <QVBoxLayout>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QStringList>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QLabel>


AutoComplete::AutoComplete(QPlainTextEdit* editor, QObject* parent)
    : QObject(parent), editor(editor) {
    keywords = QStringList()
               << "اطبع"
               << "اذا"
               << "اواذا"
               << "والا"
               << "استمر"
               << "ارجع"
               << "استورد"
               << "احذف"
               << "ادخل"
               << "او"
               << "انتظر"

               << "بينما"

               << "توقف"

               << "حاول"

               << "خطأ"
               << "خلل"

               << "دالة"

               << "صنف"
               << "صح"
               << "صحيح"

               << "عدم"
               << "عند"
               << "عام"
               << "عشري"

               << "في"

               << "ك"

               << "لاجل"
               << "ليس"

               << "مرر"
               << "من"
               << "مزامنة"
               << "مدى"

               << "نطاق"
               << "نهاية"

               << "هل"

               << "والا"
               << "ولد"
               << "و"

               << "_تهيئة_";

    shortcuts = {
        {"اطبع", "اطبع($1)"},
        {"اذا", "اذا $1:\n\tمرر\nوالا:\n\tمرر"},
        {"اواذا", "اواذا $1:"},
        {"أوإذا", "أوإذا $1:"},
        {"لاجل", "لاجل $1 في :"},
        {"لأجل", "لأجل $1 في :"},
        {"استمر", "استمر"},
        {"استورد", "استورد $1"},
        {"ارجع", "ارجع $1"},
        {"إذا", "إذا $1:"},
        {"احذف", "احذف $1"},
        {"ادخل", "ادخل($1)"},
        {"بينما", "بينما $1:"},
        {"توقف", "توقف"},
        {"خطا", "خطا"},
        {"خطأ", "خطأ"},
        {"صح", "صح"},
        {"دالة", "دالة $1():"},
        {"صنف", "صنف $1:"},
        {"عدم", "عدم"},
        {"ليس", "ليس"},
        {"مدى", "مدى($1)"},
        {"والا", "والا:\n\t$1"},
        {"وإلا", "وإلا:\n\t$1"},
        {"عام", "عام $1"},
        {"و", "و"},
        {"او", "او"},
        {"اضف", ".اضف($1)"},
        {"امسح", ".امسح($1)"},
        {"ادرج", ".ادرج($1)"},
        {"صحيح", "صحيح($1)"},
        {"عشري", "عشري($1)"},
        {"غفوة", "الوقت.غفوة($1)"},
        {"_تهيئة_", "_تهيئة_"}
    };        
    descriptions = {
        {"اطبع", "لعرض نص أو رقم أو قيمة في الطرفية."},
        {"اذا", "تنفيذ أمر في حال تحقق الشرط."},
        {"إذا", "تنفيذ أمر في حال تحقق الشرط."},
        {"اواذا", "شرط إضافي بعد الشرط الرئيسي."},
        {"أوإذا", "شرط إضافي بعد الشرط الرئيسي."},
        {"لاجل", "حلقة تكرار ضمن مدى من الاعداد او مجموعة عناصر حاوية كالمصفوفة."},
        {"لأجل", "حلقة تكرار ضمن مدى من الاعداد او مجموعة عناصر حاوية كالمصفوفة."},
        {"استمر", "الانتقال إلى التكرار التالي."},
        {"استورد", "جلب مكتبة أو ملف خارجي للبرنامج."},
        {"ارجع", "إرجاع قيمة من دالة."},
        {"احذف", "إزالة متغير من الذاكرة."},
        {"ادخل", "قراءة مدخل من المستخدم."},
        {"بينما", "حلقة تعمل طالما أن الشرط صحيح."},
        {"توقف", "إيقاف تنفيذ الحلقة."},
        {"خطا", "قيمة منطقية تدل على أن الشرط غير محقق."},
        {"خطأ", "قيمة منطقية تدل على أن الشرط غير محقق."},
        {"صح", "قيمة منطقية تدل على أن الشرط محقق."},
        {"دالة", "تعريف دالة جديدة لتنفيذ مجموعة اوامر معينة."},
        {"صنف", "إنشاء كائن."},
        {"عدم", "قيمة فارغة."},
        {"ليس", "نفي شرط أو قيمة."},
        {"مدى", "تحديد مجموعة أرقام بين قيمتين."},
        {"عام", "إخبار النطاق الداخلي أن هذا المتغير عام."},
        {"و", "تحقق الشرطين معًا."},
        {"او", "يكفي تحقق أحد الشرطين."},
    };    

    popup = new QWidget(editor, Qt::ToolTip | Qt::FramelessWindowHint);
    popup->setStyleSheet(
        "QWidget { background-color: #242533; color: #cccccc; }"
        "QListWidget { background-color: #242533; color: #cccccc; }"
        "QListWidget::item { padding: 7px; }"
        "QListWidget::item:selected { background-color: #3a3d54; padding: 0 12px 0 0; }");

    QVBoxLayout* popupLayout = new QVBoxLayout(popup);
    popupLayout->setContentsMargins(0, 0, 0, 0);

    listWidget = new QListWidget(popup);

    QLabel* descriptionLabel = new QLabel(popup);
    descriptionLabel->setStyleSheet("color: #cccccc; padding: 3px;");
    descriptionLabel->setWordWrap(true);

    // set layouts
    popupLayout->addWidget(listWidget);
    popupLayout->addWidget(descriptionLabel);
    popup->setLayout(popupLayout);

    // connections
    connect(listWidget, &QListWidget::currentItemChanged, this,
            [=](QListWidgetItem* current, QListWidgetItem* previos) {
        if (!current) return;
        QString desc = descriptions.value(current->text(), QString());
        if (desc.isEmpty()) {
            return;
        }
        descriptionLabel->setText(desc);
    });
    connect(editor, &QPlainTextEdit::textChanged, this, &AutoComplete::showCompletion);
    connect(listWidget, &QListWidget::itemClicked, this, &AutoComplete::insertCompletion);

    // filters
    editor->installEventFilter(this);
}


bool AutoComplete::eventFilter(QObject* obj, QEvent* event) {
    if (obj == editor and event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (popup->isVisible()) {
            if (keyEvent->key() == Qt::Key_Tab
                or keyEvent->key() == Qt::Key_Return
                or keyEvent->key() == Qt::Key_Enter) {
                insertCompletion();
                return true;
            } else if (keyEvent->key() == Qt::Key_Escape) {
                hidePopup();
                return true;
            } else if (keyEvent->key() == Qt::Key_Up
                       or keyEvent->key() == Qt::Key_Down) {
                QCoreApplication::sendEvent(listWidget, event);
                return true;
            } else {
                return false;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

QString AutoComplete::getCurrentWord() const {
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
    popup->setFixedSize(450, 250);
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

    QString word = item->text();
    if (!shortcuts.contains(word)) return;

    QString text = shortcuts.value(word);
    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();

    placeholderPositions.clear();

    // البحث عن جميع العلامات مثل $1 وغيرها
    QRegularExpression re("\\$(\\d+)");
    QRegularExpressionMatchIterator i = re.globalMatch(text);
    QList<QPair<int, int>> matches;

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        int pos = match.capturedStart();
        int length = match.capturedLength();
        matches.append(qMakePair(pos, length));
    }

    // إزالة العلامات وحساب المواقع الجديدة
    QString newText = text;
    int offset = 0;
    for (const auto &match : matches) {
        int originalPos = match.first - offset;
        int length = match.second;
        newText.remove(originalPos, length);
        placeholderPositions.append(originalPos);
        offset += length;
    }

    cursor.insertText(newText);

    // حفظ المواقع وتحديد المؤشر
    if (!placeholderPositions.isEmpty()) {
        cursor.setPosition(cursor.position() - newText.length() + placeholderPositions.first());
        editor->setTextCursor(cursor);
    } else {
        editor->setTextCursor(cursor);
    }

    hidePopup();
}



bool const AutoComplete::isPopupVisible() {
    return popup->isVisible();
}
