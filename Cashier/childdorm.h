#ifndef CHILDDORM_H
#define CHILDDORM_H

#include <QWidget>
#include<QSqlQuery>
#include <vector>
#include<QString>
#include<QTcpSocket>
#include "structural.h"
using namespace std;

namespace Ui {
class childdorm;
}

class childdorm : public QWidget
{
    Q_OBJECT
signals:
    void Interface();//登录成功
    void Inter_no();//登录失败
public:
    explicit childdorm(QWidget *parent = 0);
    ~childdorm();
    void Init();    //初始化
    void client();  //客服端
    void flag(QByteArray array);          //标志位判断
    void log_in(QString name,QString pw); //发送登录信息
    void cashier_in();     //登录成功
    void cashier_no();     //登录失败
    void cashier_query();  //打印账单查询结果
    void cashier_refresh();//刷新餐桌状态
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::childdorm *ui;
    QString m_name;  //用户名
    float m_sum;  //顾客消费总额
    QTcpSocket *tcpsocket;
    FoodInfo F_head;//订单结构体
    vector<FoodInfo> foodVec;//通用订单容器
    vector<FoodInfo> FoodVec;//订单容器

};

#endif // CHILDDORM_H
