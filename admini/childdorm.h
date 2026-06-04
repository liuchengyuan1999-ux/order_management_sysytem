#ifndef CHILDDORM_H
#define CHILDDORM_H

#include <QWidget>
#include<QTcpSocket>//建立套接字的类
#include<QTcpServer>//监听套接字的类
#include <vector>
#include<QSqlDatabase>
#include <QDesktopWidget>
#include<QSqlError>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框

#include "structural.h"
#include "widget.h"
#include "add_users.h"
#include "change_per.h"
#include "change_pw.h"
#include "add_food.h"
#include "change_price.h"
#include "change_status.h"
#include "add_drinks.h"
#include "modify_drinks.h"
#include "modify_stutus.h"
#include "bill.h"
#include <QDesktopWidget>
using namespace std;

namespace Ui {
class childdorm;
}

class childdorm : public QWidget
{
    Q_OBJECT

public:
    explicit childdorm(QWidget *parent = 0);
    ~childdorm();
    void Menu_print();//打印菜单
    void User_print();//打印用户
    void Bill_print();//打印账单
    void Init();      //初始化
    void getname(QString name);//打印管理者姓名
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::childdorm *ui;
    QTcpServer *Server;
    QTcpSocket *Socket;
    QSqlDatabase db;
    vector<MenuInfo> MenuVec;//菜单容器
    MenuInfo M_head;//菜单结构体
    vector<FoodInfo> Foodvec;//订单容器
    FoodInfo F_head;//订单结构体
};

#endif // CHILDDORM_H
