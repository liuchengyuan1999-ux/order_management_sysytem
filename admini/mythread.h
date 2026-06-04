#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>
#include <QWidget>
#include <QTcpSocket>
#include <QByteArray>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <vector>
#include <cstring>
#include <string>
#include <QDateTime>
#include "structural.h"

using namespace std;

class mythread : public QThread
{
    Q_OBJECT
public:
    mythread(QTcpSocket *socket);
    void run();    // 线程入口，启动后由线程自动执行，不要手动调用
    void Init();   // 初始化菜单数据
    void Flag(QByteArray array); // 客户端消息标志位判断
    void order_menu();     // 向点餐端发送菜单
    void order_food();     // 点餐端订单处理
    void chef_sent();      // 向厨师端发送订单
    void chef_in();        // 厨师端上菜处理
    void cashier_in();     // 收银端登录
    void cashier_query();  // 收银端查询账单
    void cashier_refresh();// 收银端刷新餐桌状态
    void cashier_finish(); // 收银端结账

public:
   QTcpSocket *tcpsocket;
   static QTcpSocket *order_socket[21];  // 点餐端套接字
   static QTcpSocket *chef_socket;       // 厨师端套接字
   static QTcpSocket *cashier_socket;    // 收银端套接字
   vector<MenuInfo> MenuVec;       // 菜单容器
   MenuInfo M_head;                // 菜单结构体
   vector<FoodInfo> Foodvec;       // 订单容器
   FoodInfo F_head;                // 订单结构体
   QSqlDatabase db;
};

static vector<FoodInfo> foodvec[21]; // 订单容器

#endif // MYTHREAD_H
