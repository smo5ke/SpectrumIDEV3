#include "AlifLexer.h"
#include <qstring.h>
#include <qvector.h>
#include <qset.h>
#include <qchar.h>


QVector<Token> Lexer::tokenize(const QString text) {
    while (pos < text.length()) {
        QChar currentChar = text[pos];

        if (currentChar.isSpace()) {
            while (pos < text.length() and text[pos].isSpace()) {
                pos++;
            }
        }
        else if (currentChar.isDigit()) {
            int start = pos;
            while (pos < text.length() and (text[pos].isDigit() or text[pos] == '.')) {
                pos++;
            }
            tokens.append(Token(TokenType::Number, start, pos - start, text.mid(start, pos - start)));
        }
        else if (currentChar.isLetter() or currentChar == '_') {
            int start = pos;
            while (pos < text.length() and (text[pos].isLetterOrNumber() or text[pos] == '_')) {
                pos++;
            }
            QString identifier = text.mid(start, pos - start);
            TokenType type = isKeyword(identifier) ? TokenType::Keyword : isKeyword1(identifier) ? TokenType::Keyword1 : isKeyword2(identifier) ? TokenType::Keyword2 : TokenType::Identifier;
            tokens.append(Token(type, start, pos - start, identifier));
        }
        else if (currentChar == '"' or currentChar == '\'') {
            int start = pos;
            QChar quoteChar = currentChar;
            pos++; // Move past the opening quote

            // Check if the string is an f-string (starts with u'م')
            bool isFString = (start > 0 and text[start - 1] == u'م');

            while (pos < text.length()) {
                if (text[pos] == '/') {
                    // Handle escape sequences
                    pos++; // Skip the escape character
                    if (pos < text.length()) {
                        pos++; // Skip the escaped character
                    }
                }
                else if (text[pos] == quoteChar) {
                    // Found the closing quote
                    pos++; // Move past the closing quote
                    break;
                }
                else if (isFString and text[pos] == '{') {
                    tokens.append(Token(TokenType::String, start, pos - start, text.mid(start, pos - start)));
                    pos++; // Move past the '{'
                    bracesNum++;
                    this->tokenize(text);
                    start = pos; // Start a new string segment after the '{'
                }
                else if (isFString and text[pos] == '}') {
                    bracesNum--;
                    this->tokenize(text);
                    pos++; // Move past the '}'
                    start = pos; // Start a new string segment after the '}'
                }
                else {
                    pos++; // Move to the next character
                }
            }

            // Append the remaining string content
            if (start < pos) {
                tokens.append(Token(TokenType::String, start, pos - start, text.mid(start, pos - start)));
            }
        }
        else if (currentChar == '#') {
            int start = pos;
            while (pos < text.length() and text[pos] != '\n') {
                pos++;
            }
            tokens.append(Token(TokenType::Comment, start, pos - start, text.mid(start, pos - start)));
        }
        else {
            pos++;
        }
    }
    return tokens;
}


bool Lexer::isKeyword(const QString& word) {
    static const QSet<QString> keywords = {
    "ك", "و", "في", "او", "أو", "من", "مع", "صح", "هل",
    "اذا", "إذا", "ليس", "مرر", "عدم", "ولد", "صنف", "خطا", "خطأ", "عام",
    "احذف", "دالة", "لاجل", "لأجل", "والا", "وإلا", "توقف", "نطاق", "ارجع",
    "اواذا", "أوإذا", "بينما", "انتظر", "استمر", "مزامنة", "استورد",
    };
    return keywords.contains(word);
}


bool Lexer::isKeyword1(const QString& word) {
    static const QSet<QString> keywords = {
        "اطبع", "ادخل"
    };
    return keywords.contains(word);
}

bool Lexer::isKeyword2(const QString& word) {
    static const QSet<QString> keywords = {
        "_تهيئة_", "هذا"
    };
    return keywords.contains(word);
}