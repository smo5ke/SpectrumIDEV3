#include "AlifParser.h"



void AlifParser::process_token(const QChar& ch) {
    if (!currentToken.isEmpty()) {
        for (int i = 0; i < 35; i++) {
            if (currentToken == reservedKeywords[i]) {
                currentType = TokenType::Keyword;
                return;
            }
        }
        currentType = TokenType::Other;
        return;
    }
}

QVector<QPair<QString, TokenType>> AlifParser::tokenize(const QString& text) {
    QVector<QPair<QString, TokenType>> tokens;
    for (const QChar& ch : text) {
        if (ch.isLetterOrNumber() and !ch.isSpace()) {
            currentToken.append(ch);
            continue;
        }
        process_token(ch);
        if (!currentToken.isEmpty()) {
            tokens.append(qMakePair(currentToken, currentType));
        }
        currentToken.clear();
        currentType = TokenType::Other;
    }
    return tokens;
}