#pragma once

#include "AlifParser.h"
#include <QTextEdit>




class SyntaxHighlighter : public QTextEdit {
    Q_OBJECT

public:
    SyntaxHighlighter(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    AlifParser parser{};
    void highlight_text();
};