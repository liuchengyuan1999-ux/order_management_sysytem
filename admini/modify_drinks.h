#ifndef MODIFY_DRINKS_H
#define MODIFY_DRINKS_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class Modify_drinks;
}

class Modify_drinks : public QWidget
{
    Q_OBJECT

public:
    explicit Modify_drinks(QWidget *parent = 0);
    ~Modify_drinks();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Modify_drinks *ui;
    QSqlDatabase db;
};

#endif // MODIFY_DRINKS_H
