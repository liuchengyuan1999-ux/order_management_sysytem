#ifndef ADD_USERS_H
#define ADD_USERS_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class add_users;
}

class add_users : public QWidget
{
    Q_OBJECT

public:
    explicit add_users(QWidget *parent = 0);
    ~add_users();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::add_users *ui;
    QSqlDatabase db;
};

#endif // ADD_USERS_H
