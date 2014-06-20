#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_generateButton_clicked();
    void on_showPw_toggled(bool checked);

    void on_copyButton_clicked();

private:
    Ui::MainWindow ui;

    QStringList _domainList;
    QStringList _descList;

    void setCompleter(QLineEdit *lineEdit, const QString &listName);

    QStringList loadWordList(const QString &listName);
    QStringList saveWordList(const QString &listName, QString newWord);

    QString configPath();
};

#endif // DIALOG_H
