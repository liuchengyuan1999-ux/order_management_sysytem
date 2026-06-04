#ifndef BILL_H
#define BILL_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class Bill;
}

class Bill : public QWidget
{
    Q_OBJECT

public:
    explicit Bill(QWidget *parent = 0);
    ~Bill();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Bill *ui;
    QSqlDatabase db;
};

#endif // BILL_H
