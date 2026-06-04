#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QTcpServer>   //监听套接字的类
#include<QTcpSocket>   //建立套接字的类
#include "structural.h"
#include"mythread.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void tcp();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    QSqlDatabase db;
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
};

#endif // WIDGET_H
