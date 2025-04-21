#include "SPHighlighter.h"


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
            format.setForeground(QColor(205, 119, 57));
            // format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Keyword1:
            format.setForeground(QColor(205, 119, 57));
            // format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Keyword2:
            format.setForeground(QColor(195, 124, 88));
            format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Number:
            format.setForeground(QColor(168, 135, 206));
            break;
        case TokenType::Identifier:
            if (isFunctionName(text, token.startPos + token.len)) {
                format.setForeground(QColor(206, 147, 74));
                // format.setFontWeight(QFont::Bold);
            }
            break;
        case TokenType::Comment:
            format.setForeground(QColor(85, 91, 100));
            format.setFontItalic(true);
            break;
        case TokenType::String:
            format.setForeground(QColor(137, 174, 66));
            break;
        case TokenType::Operator:
            format.setForeground(QColor(195, 124, 88));
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
