#include "Syntaxhighlighter.h"

#include <qtextcursor.h>
#include <QTextCharFormat>


SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent) {
}

void SyntaxHighlighter::addHighlightingRule(const QString& pattern,
    const QTextCharFormat& format, HighlightState state = NormalState) {
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = format;
    rule.state = state;
    highlightingRules.append(rule);
}

void SyntaxHighlighter::highlightBlock(const QString& text) {
    int previousRuleState{-1};
    int currentRuleState{-1};

    for (const HighlightingRule& rule : std::as_const(highlightingRules)) {

        currentRuleState = rule.state;

        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();

            if (previousRuleState == StringState
                and currentRuleState == CommentState
                and match.captured().endsWith('"')) {
                continue;
            }

            // Apply formatting
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);


            if (previousRuleState == StringState) {
                previousRuleState = NormalState;
            }
            else if (currentRuleState == StringState) {
                previousRuleState = StringState;
            }
        }
    }
}


