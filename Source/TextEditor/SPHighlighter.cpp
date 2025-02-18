#include "SPhighlighter.h"

#include <qtextcursor.h>
#include <QTextCharFormat>


SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent) {
}

void SyntaxHighlighter::highlightBlock(const QString& text) {
    Lexer lexer{};
    QVector<Token> tokens = lexer.tokenize(text);

    for (const auto& token : tokens) {
        QTextCharFormat format;
        switch (token.type) {
        case TokenType::Keyword:
            format.setForeground(QColor("#FF79C6"));
            format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Keyword1:
            format.setForeground(QColor("#DE3163"));
            format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Keyword2:
            format.setForeground(QColor("#CCCCFF"));
            format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Number:
            format.setForeground(QColor("#FFB86C"));
            break;
        case TokenType::Identifier:
            if (isFunctionName(text, token.startPos + token.len)) {
                format.setForeground(QColor("#8BE9FD"));
                format.setFontWeight(QFont::Bold);
            }
            break;
        case TokenType::Comment:
            format.setForeground(QColor("#6272A4"));
            break;
        case TokenType::String:
            format.setForeground(QColor("#50FA7B"));
            break;
        default:
            break;
        }
        setFormat(token.startPos, token.len, format);
    }
}



bool SyntaxHighlighter::isFunctionName(const QString& blockText, int idEndPos) {
    if (idEndPos < blockText.length() and blockText[idEndPos] == '(') {
        return true;
    }
    return false;
}