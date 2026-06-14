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

class childdorm;
class QTableWidgetItem;
class Widget : public QWidget
{
    Q_OBJECT


public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void Init();//初始化
    void client();//客服端
    void flag(QByteArray array);//标志位判断
    void Menu_print();//打印菜单
    void Chef_in(vector<MenuInfo> menuVec);//上菜
    // friend class childdorm;
    //这里就是get函数，就是成员变量对外的接口，返回值是一个容器的指针
    vector<FoodInfo>* getVecNo(){return &FoodVec_no;}
    vector<FoodInfo>* getVec(){return &FoodVec;}
    QTcpSocket * getsocket(){return tcpsocket;}
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void onMenuItemClicked(QTableWidgetItem *item);
    void comeback();
    void Delete();
public:
    float m_sum;//总额
    Ui::Widget *ui;
    QSqlDatabase db;
    FoodInfo F_head;//订单结构体
    vector<FoodInfo> foodVec;//通用订单容器
    vector<FoodInfo> FoodVec;//订单容器
    vector<FoodInfo> FoodVec_no;//未上菜单容器
    vector<FoodInfo> FoodVec_in;//已上菜单容器
    QTcpSocket *tcpsocket;
    vector<MenuInfo> MenuVec;//菜单容器
    MenuInfo M_head;//菜单结构体
    childdorm *child ;//这里定义的是另一个窗口的类
};



#endif // WIDGET_H
