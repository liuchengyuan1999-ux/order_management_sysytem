#ifndef CHANGE_PRICE_H
#define CHANGE_PRICE_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class change_price;
}

class change_price : public QWidget
{
    Q_OBJECT

public:
    explicit change_price(QWidget *parent = 0);
    ~change_price();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::change_price *ui;
    QSqlDatabase db;
};

#endif // CHANGE_PRICE_H
