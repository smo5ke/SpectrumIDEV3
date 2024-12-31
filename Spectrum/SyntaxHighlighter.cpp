#include "Syntaxhighlighter.h"

#include <QTextCursor>
#include <QTextCharFormat>
#include <QKeyEvent>

SyntaxHighlighter::SyntaxHighlighter(QWidget* parent)
    : QTextEdit(parent) {
    connect(this, &QTextEdit::textChanged, this, &SyntaxHighlighter::highlight_text);
}

void SyntaxHighlighter::keyPressEvent(QKeyEvent* event) {
    QTextEdit::keyPressEvent(event);
    highlight_text();
}

void SyntaxHighlighter::highlight_text() {
    static bool settingFormat = false;
    if (settingFormat) return;
    settingFormat = true;

    QTextCursor cursor(document());
    //cursor.movePosition(QTextCursor::Start);
    QTextCharFormat keywordFormat{};
    keywordFormat.setForeground(QColor("#00d6e8"));
    QTextCharFormat identifierFormat{};
    identifierFormat.setForeground(Qt::white);

    QString text = toPlainText();
    QVector<QPair<QString, TokenType>> tokens = parser.tokenize(text);

    for (const auto& token : tokens) {
        QString word = token.first;
        TokenType type = token.second;

        cursor.select(QTextCursor::WordUnderCursor);
        QString selectedWord = cursor.selectedText();
        if (selectedWord == word) {
            if (type == TokenType::Keyword) {
                cursor.setCharFormat(keywordFormat);
            }
            else if (type == TokenType::Other) {
                cursor.setCharFormat(identifierFormat);
            }
        }
        cursor.movePosition(QTextCursor::NextWord);
    }
    cursor.movePosition(QTextCursor::NextWord);

    settingFormat = false;
}

