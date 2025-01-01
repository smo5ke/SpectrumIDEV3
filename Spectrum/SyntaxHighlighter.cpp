#include "Syntaxhighlighter.h"

#include <QTextCursor>
#include <QTextCharFormat>
#include <QKeyEvent>


SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent) {
}

void SyntaxHighlighter::addHighlightingRule(const QString& pattern, const QTextCharFormat& format) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = format;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::highlightBlock(const QString& text) {
    for (const HighlightingRule& rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
