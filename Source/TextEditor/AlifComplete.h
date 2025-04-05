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
    explicit AutoComplete(QPlainTextEdit* editor, QObject* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void showCompletion();
    void insertCompletion();

private:
    QPlainTextEdit* editor{};
    QWidget* popup{};
    QListWidget* listWidget{};
    QStringList keywords{};

    QString getCurrentWord() const;
    void showPopup();
    inline void hidePopup();
};
