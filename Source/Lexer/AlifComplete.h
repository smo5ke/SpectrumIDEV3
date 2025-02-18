#pragma once

#include <QObject>
#include <QListWidget>
#include <QMenu>
#include <QPlainTextEdit>
#include <QStringList>

class AutoComplete : public QObject
{
    Q_OBJECT
public:
    explicit AutoComplete(QTextEdit* editor, QObject* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void showCompletion();
    void insertCompletion();

private:
    QTextEdit* editor{};
    QWidget* popup{};
    QListWidget* listWidget{};
    QStringList keywords{};

    QString getCurrentWord() const;
    void showPopup();
    void hidePopup();
};