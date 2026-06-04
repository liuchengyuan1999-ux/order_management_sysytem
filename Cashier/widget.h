#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QSqlQuery>
#include <vector>
#include<QString>
#include<QTcpSocket>
#include "childdorm.h"
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

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void comeback();//关闭此窗口
    void comeback_no();//登录失败
public:
    Ui::Widget *ui;
    FoodInfo F_head;//订单结构体
    vector<FoodInfo> foodVec;//通用订单容器
    vector<FoodInfo> FoodVec;//订单容器
    childdorm *child ;
};

#endif // WIDGET_H
