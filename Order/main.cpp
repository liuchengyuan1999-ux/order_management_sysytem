#include "widget.h"
#include <QApplication>
#include<QSqlQuery>
#include <vector>
#include<QString>
#include<QTcpSocket>
#include "childdorm.h"
#include "structural.h"
using namespace std;

//FoodInfo F_head;//订单结构体
//vector<FoodInfo> foodVec;//通用订单容器
//vector<FoodInfo> FoodVec;//订单容器
//vector<FoodInfo> FoodVec_no;//未上菜单容器
//vector<FoodInfo> FoodVec_in;//已上菜单容器
//QTcpSocket *tcpsocket;
//vector<MenuInfo> MenuVec;//菜单容器
//MenuInfo M_head;//菜单结构体

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
     w.show();

//    tcpsocket = new QTcpSocket(this);//创建客户端套接字


//    connect(tcpsocket,&QTcpSocket::connected,//请求连接信号
//           [=]()
//    {
//        //ui->textEdit->setText("成功连接服务器");
//    }
//    );

//    QString ip = "192.168.12.13";
//    qint16 port = 8888;
//    tcpsocket->connectToHost(QHostAddress(ip),port);


//    //qDebug() << QSqlDatabase::drivers();//Qt支持的数据库类型
//    db = QSqlDatabase::addDatabase("QMYSQL"); //添加数据库
//    db.setHostName("192.168.12.13");
//    db.setUserName("root");
//    db.setPassword("12345678");
//    db.setPort(3306);
//    db.setDatabaseName("Order");

//    if(!db.open())
//    {
//       QMessageBox::warning(this,"提示","无法连接数据库");
//    }




//    connect(tcpsocket,&QTcpSocket::readyRead,
//            [=]()
//    {
//        QByteArray array = tcpsocket->readAll();
//        w.Menu_print(array);//打印菜单
//    }
//    );



    return a.exec();
}
