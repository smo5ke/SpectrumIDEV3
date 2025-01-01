#pragma once

#include "SyntaxHighlighter.h"

#include <qtextedit.h>




class AlifEditor : public QTextEdit {
	Q_OBJECT

public:
	AlifEditor(QWidget* parent = nullptr);


private:
	SyntaxHighlighter* highlighter{};

	void setupHighlighter();

};