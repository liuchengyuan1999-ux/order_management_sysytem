#include "widget.h"
#include "ui_widget.h"
#include "childdorm.h"//自己添加
#include <QDesktopWidget>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QFile>
#include <QSqlError>
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(400,377);//背景图尺寸

    this->setWindowIcon(QIcon(":/image/44.png"));//APP图标

   //窗口居中显示
   QDesktopWidget* desktop = QApplication::desktop();
   move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/1.jpg")));//背景图
    setPalette(pal);

    //连接数据库，第一行括号内为使用的驱动名
    db = QSqlDatabase::addDatabase("QMYSQL"); //添加数据库
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("123456789");
    db.setPort(3306);
    db.setDatabaseName("Order");
    //连接，并进行连接判断
    if(!db.open())
    {
       QMessageBox::warning(this,"提示","无法连接数据库");
    }
    else
    {
        QSqlQuery query(db);
        query.exec("SET NAMES utf8mb4");
    }
    //这里开始监听，这里对监听代码进行了封装
    tcp();

}


Widget::~Widget()
{
    delete ui;
}

void Widget::tcp()
{
    //新建监听的套接字
    tcpserver = new QTcpServer(this);
    //开启监听
    tcpserver->listen(QHostAddress::Any,8888);//binf + listen
    //捕获新连接
    connect(tcpserver,&QTcpServer::newConnection,  //lambda表达式:accept函数
    [=](){
        //新建新的套接字:客服端套接字
        tcpsocket = tcpserver->nextPendingConnection();//最近一个接进来的客服端
        QString ip =tcpsocket->peerAddress().toString();//得到地址
        int port = tcpsocket->peerPort();
        QString temp = QString::fromUtf8("地址:%1 端口:%2").arg(ip).arg(port);
         qDebug()<<temp;

        //这里进行线程的创建，然后在线程里继续读取操作
        //这里是没来一个客户端，创建一个线程
         mythread *thread = new mythread(tcpsocket);
         thread->start();
    });
}


//主要思路就是组合输入信息，执行SQL指令，在数据库中进行查找
//根据数据库用户列表中的信息，查看账号密码是否正确，不正确弹出窗口提示
//密码正确再判断是不是管理员，是管理员就进入我们自己创建的childdom类窗口
void Widget::on_pushButton_clicked()//登录
{
    QSqlQuery query(db);

    QString name = ui->lineEdit->text();//用户名
    QString pwm = ui->lineEdit_2->text();//密码

    QString temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
    query.exec(temp);
    query.next();
    if(query.value("密码").toString() ==  pwm)
    {
        temp=QString::fromUtf8("select 权限 from 用户 where 用户名 ='%1'").arg(name);
        query.exec(temp);
        query.next();
        if(query.value("权限").toString()=="管理员")
        {
            this->hide();
            childdorm *child = new childdorm();
            child->getname(name);
            child->show();
        }
        else
        {
            QMessageBox qm(this);
            qm.setText(QStringLiteral("该用户不是管理员!"));
            qm.move(this->geometry().center());
            qm.exec();
        }
    }
    else
    {
        QMessageBox qm(this);
        qm.setText(QStringLiteral("账号或密码错误!"));
        qm.move(this->geometry().center());
        qm.exec();
    }
}

void Widget::on_pushButton_2_clicked()
{
    exit(-1);
}
