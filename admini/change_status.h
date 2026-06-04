#ifndef CHANGE_STATUS_H
#define CHANGE_STATUS_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>
namespace Ui {
class change_status;
}

class change_status : public QWidget
{
    Q_OBJECT

public:
    explicit change_status(QWidget *parent = 0);
    ~change_status();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::change_status *ui;
    QSqlDatabase db;
};

#endif // CHANGE_STATUS_H
