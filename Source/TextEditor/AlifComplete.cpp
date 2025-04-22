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
        << "اطبع" << "اواذا" << "لاجل" << "أوإذا"
        << "استمر" << "استورد" << "ارجع" << "اذا"
        << "احذف" << "ادخل" << "بينما" << "توقف"
        << "خطأ" << "دالة" << "صنف" << "صح"
        << "عدم" << "ليس" << "مدى" << "والا"
        << "عام" << "و" << "او" << "إذا"
        << "اضف" << "امسح" << "ادرج"
        << "صحيح" << "عشري" << "غفوة" << "_تهيئة_";

    shortcuts = {
        {"اطبع", "اطبع($1)"},
        {"اواذا", "اواذا $1:"},
        {"أوإذا", "أوإذا $1:"},
        {"لاجل", "لاجل $1 في مدى():"},
        {"لأجل", "لأجل $1 في مدى():"},
        {"استمر", "استمر"},
        {"استورد", "استورد $1"},
        {"ارجع", "ارجع $1"},
        {"اذا", "اذا $1:"},
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
        {"اواذا", "شرط إضافي بعد الشرط الأول."},
        {"أوإذا", "شرط إضافي بعد الشرط الأول."},
        {"لاجل", "حلقة تكرار تُستخدم عددًا معينًا من المرات."},
        {"لأجل", "حلقة تكرار تُستخدم عددًا معينًا من المرات."},
        {"استمر", "تجاوز التكرار الحالي والانتقال إلى التالي."},
        {"استورد", "جلب مكتبة أو ملف خارجي للبرنامج."},
        {"ارجع", "إرجاع قيمة من دالة."},
        {"اذا", "تنفيذ أمر في حال تحقق الشرط."},
        {"إذا", "تنفيذ أمر في حال تحقق الشرط."},
        {"احذف", "إزالة متغير من الذاكرة."},
        {"ادخل", "قراءة مدخل من المستخدم."},
        {"بينما", "حلقة تعمل طالما أن الشرط صحيح."},
        {"توقف", "إيقاف تنفيذ الحلقة."},
        {"خطا", "قيمة تدل على أن الشرط غير محقق."},
        {"خطأ", "قيمة تدل على أن الشرط غير محقق."},
        {"صح", "قيمة تدل على أن الشرط محقق."},
        {"دالة", "تعريف دالة جديدة لتنفيذ أمر معين."},
        {"صنف", "إنشاء كائن وفق نموذج معين."},
        {"عدم", "التحقق من عدم وجود شيء."},
        {"ليس", "نفي شرط أو قيمة."},
        {"مدى", "تحديد مجموعة أرقام بين قيمتين."},
        {"والا", "تنفيذ أمر إذا لم يتحقق الشرط السابق."},
        {"وإلا", "تنفيذ أمر إذا لم يتحقق الشرط السابق."},
        {"عام", "تعريف متغير عام متاح في كل مكان."},
        {"و", "تحقق الشرطين معًا."},
        {"او", "يكفي تحقق أحد الشرطين."},
        {"اضف", "إضافة عنصر إلى قائمة."},
        {"امسح", "حذف عنصر من قائمة."},
        {"ادرج", "إدخال عنصر في موضع محدد داخل قائمة."},
        {"غفوة", "تأخير تنفيذ البرنامج لفترة معينة."},
        {"_تهيئة_", "تهيئة متغيرات أو كائنات قبل الاستخدام."}
    };    

    popup = new QWidget(editor, Qt::ToolTip | Qt::FramelessWindowHint);
    listWidget = new QListWidget(popup);
    popup->setStyleSheet(
        "QWidget { background-color: #242533; color: #cccccc; }"
        "QListWidget { background-color: #242533; color: #cccccc; }"
        "QListWidget::item { padding: 7px 12px; }"
        "QListWidget::item:selected { background-color: #3a3d54; }");

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(listWidget);
    popup->setLayout(layout);

    editor->installEventFilter(this);

    connect(editor, &QPlainTextEdit::textChanged, this, &AutoComplete::showCompletion);
    connect(listWidget, &QListWidget::itemClicked, this, &AutoComplete::insertCompletion);

    QWidget* descrWidget = new QWidget(popup);
    QHBoxLayout* descrLayout = new QHBoxLayout(descrWidget);
    descrLayout->setContentsMargins(0, 0, 0, 0);
    descrLayout->setSpacing(10);

    QLabel* descriptionLabel = new QLabel(descrWidget);
    descriptionLabel->setStyleSheet("color: #cccccc; padding: 5px;");
    descriptionLabel->setWordWrap(true);

    QPushButton* moreButton = new QPushButton("عرض المزيد", descrWidget);
    moreButton->setFixedWidth(100);
    moreButton->setStyleSheet("background-color: none; color: #10a8f4; padding: 0px; border: none;");

    QVBoxLayout* popupLayout = qobject_cast<QVBoxLayout*>(popup->layout());
    if (popupLayout) {
        if (popupLayout->count() > 1) {
            QLayoutItem* oldItem = popupLayout->takeAt(1);
            delete oldItem->widget();
            delete oldItem;
        }
        popupLayout->addWidget(descrWidget);
    }

    descrLayout->addWidget(descriptionLabel);
    descrLayout->addWidget(moreButton);

    connect(listWidget, &QListWidget::currentItemChanged, this, [=](QListWidgetItem* current, QListWidgetItem*) {
        if (!current) return;
        QString desc = descriptions.value(current->text(), QString());
        if (desc.isEmpty()) {
            return;
        }
        descriptionLabel->setText(desc);

        connect(moreButton, &QPushButton::clicked, this, [current]() {
            QString keyword = current->text();
            QString url = QString("https://aliflang.org/Docs/#%1").arg(keyword);
            QDesktopServices::openUrl(QUrl(url));
        });
    });
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

    // فرز العلامات حسب ظهورها
    std::sort(matches.begin(), matches.end(), [](const QPair<int, int> &a, const QPair<int, int> &b) {
        return a.first < b.first;
    });

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
        currentPlaceholderIndex = 0;
        cursor.setPosition(cursor.position() - newText.length() + placeholderPositions.first());
        editor->setTextCursor(cursor);
    } else {
        editor->setTextCursor(cursor);
    }

    hidePopup();
}
