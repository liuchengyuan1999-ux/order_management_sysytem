#ifndef ADD_FOOD_H
#define ADD_FOOD_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class Add_food;
}

class Add_food : public QWidget
{
    Q_OBJECT

public:
    explicit Add_food(QWidget *parent = 0);
    ~Add_food();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Add_food *ui;
    QSqlDatabase db;
};

#endif // ADD_FOOD_H
