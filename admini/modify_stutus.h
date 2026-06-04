#ifndef MODIFY_STUTUS_H
#define MODIFY_STUTUS_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QSqlError>
#include <QDesktopWidget>

namespace Ui {
class Modify_stutus;
}

class Modify_stutus : public QWidget
{
    Q_OBJECT

public:
    explicit Modify_stutus(QWidget *parent = 0);
    ~Modify_stutus();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Modify_stutus *ui;
    QSqlDatabase db;
};

#endif // MODIFY_STUTUS_H
