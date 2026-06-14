#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QSqlQuery>
#include <vector>
#include<QString>
#include<QTcpSocket>
#include "structural.h"
using namespace std;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void Init();
    void client();
    void Order_print(QByteArray array);//初始化
private slots:
    void on_pushButton_2_clicked();//退出

    void on_pushButton_clicked();//上菜

    void on_pushButton_3_clicked();//刷新

public:
    Ui::Widget *ui;
    //QSqlDatabase db;
    FoodInfo F_head;
    vector<FoodInfo> FoodVec;//订单结构体，用于接收
    vector<FoodInfo> foodVec;//订单结构体，用于发送
    QTcpSocket *tcpsocket;
};

#endif // WIDGET_H
