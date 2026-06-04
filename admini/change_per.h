#ifndef CHANGE_PER_H
#define CHANGE_PER_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class change_per;
}

class change_per : public QWidget
{
    Q_OBJECT

public:
    explicit change_per(QWidget *parent = 0);
    ~change_per();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::change_per *ui;
    QSqlDatabase db;
};

#endif // CHANGE_PER_H
