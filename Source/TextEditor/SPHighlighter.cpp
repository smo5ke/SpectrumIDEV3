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
            format.setForeground(QColor(255, 121, 198));
            format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Keyword1:
            format.setForeground(QColor(222, 49, 99));
            format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Keyword2:
            format.setForeground(QColor(204, 204, 255));
            format.setFontWeight(QFont::Bold);
            break;
        case TokenType::Number:
            format.setForeground(QColor(255, 184, 108));
            break;
        case TokenType::Identifier:
            if (isFunctionName(text, token.startPos + token.len)) {
                format.setForeground(QColor(139, 233, 253));
                format.setFontWeight(QFont::Bold);
            }
            break;
        case TokenType::Comment:
            format.setForeground(QColor(98, 114, 164));
            break;
        case TokenType::String:
            format.setForeground(QColor(80, 250, 123));
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
