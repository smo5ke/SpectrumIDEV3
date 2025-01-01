#pragma once

#include <qsyntaxhighlighter.h>
#include <QTextCharFormat>
#include <qregularexpression.h>

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit SyntaxHighlighter(QTextDocument* parent = nullptr);
    
    
    enum HighlightState {
        NormalState = -1,
        StringState,
        CommentState
    };

    void addHighlightingRule(const QString& pattern, const QTextCharFormat& format, HighlightState state);


protected:
    void highlightBlock(const QString& text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
        HighlightState state;
    };
    QVector<HighlightingRule> highlightingRules;
};

