#pragma once

#include <QString>
#include <QVector>
#include <QPair>





static const char* reservedKeywords[] = {
	"ك", "و",  // 1 char
	"في", "او", "أو", "من", "مع", "صح", "هل",  // 2 chars
	"اذا", "إذا", "ليس", "مرر", "عدم", "ولد", "صنف", "خطا", "خطأ", "عام",  // 3 chars
	"احذف", "دالة", "لاجل", "لأجل",  "والا", "وإلا", "توقف", "نطاق", "ارجع",   // 4 chars
	"اواذا", "أوإذا", "بينما",  "انتظر", "استمر",  // 5 chars
	"مزامنة", "استورد",  // 6 chars
};


enum TokenType {
    Keyword,
    Other,
};



class AlifParser {
public:
    QVector<QPair<QString, TokenType>> tokenize(const QString& text);

private:
    QString currentToken;
    TokenType currentType;
    void process_token(const QChar& ch);
};