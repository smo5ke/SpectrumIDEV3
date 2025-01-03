#pragma once

#include <qtextedit.h>




class AlifEditor : public QTextEdit {
	Q_OBJECT

public:
	AlifEditor(QWidget* parent = nullptr);


private:
	class SyntaxHighlighter* highlighter{};

	void setupHighlighter();

};