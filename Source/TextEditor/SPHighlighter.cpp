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
            format.setForeground(QColor(254, 135, 48));
            break;
        case TokenType::Keyword1:
            format.setForeground(QColor(218, 183, 68));
            break;
        case TokenType::Keyword2:
            format.setForeground(QColor(121, 129, 230));
            break;
        case TokenType::Number:
            format.setForeground(QColor(168, 135, 206));
            break;
        case TokenType::Identifier:
            if (isFunctionName(text, token.startPos + token.len)) {
                format.setForeground(QColor(206, 147, 74));
            }
            break;
        case TokenType::Comment:
            format.setForeground(QColor(85, 91, 100));
            format.setFontItalic(true);
            break;
        case TokenType::String:
            format.setForeground(QColor(78, 116, 51));
            break;
        case TokenType::Operator:
            format.setForeground(QColor(224, 108, 117));
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
