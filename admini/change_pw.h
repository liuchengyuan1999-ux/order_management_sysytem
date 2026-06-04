#ifndef CHANGE_PW_H
#define CHANGE_PW_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class change_pw;
}

class change_pw : public QWidget
{
    Q_OBJECT

public:
    explicit change_pw(QWidget *parent = 0);
    ~change_pw();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::change_pw *ui;
    QSqlDatabase db;
};

#endif // CHANGE_PW_H
