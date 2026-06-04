#ifndef ADD_DRINKS_H
#define ADD_DRINKS_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class Add_drinks;
}

class Add_drinks : public QWidget
{
    Q_OBJECT

public:
    explicit Add_drinks(QWidget *parent = 0);
    ~Add_drinks();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Add_drinks *ui;
    QSqlDatabase db;
};

#endif // ADD_DRINKS_H
