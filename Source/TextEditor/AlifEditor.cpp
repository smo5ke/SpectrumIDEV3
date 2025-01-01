#include "AlifEditor.h"



AlifEditor::AlifEditor(QWidget* parent) {

    this->setTabStopDistance(32);
    this->setAcceptRichText(true);
    this->setStyleSheet("QTextEdit { background-color: #151729; color: #ffffff;}");
    this->setFont(QFont("Tajawal", 12, 500));

    // set "force" cursor and text direction from right to left
    QTextDocument* document = this->document();
    QTextOption option = document->defaultTextOption();
    option.setTextDirection(Qt::RightToLeft);
    document->setDefaultTextOption(option);


    highlighter = new SyntaxHighlighter(document);
    setupHighlighter();
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
