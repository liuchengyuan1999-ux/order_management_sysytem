#include "widget.h"
#include "ui_widget.h"
#include "childdorm.h"//自己添加
#include <QDesktopWidget>
#include<QLabel>//自己添加
#include<QPalette>//自己添加
#include<QMessageBox>//对话框
#include<QFile>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(400,377);//背景图尺寸
    this->move(QPoint(100,100));
    this->setWindowIcon(QIcon(":/image/5.png"));//APP图标

   //窗口居住显示
   QDesktopWidget* desktop = QApplication::desktop();
   move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/1.jpg")));//背景图
    setPalette(pal);

    child = new childdorm();
    child->hide();
    //child->getData(this);   //通过this把父窗口的指针传到子窗口当中
    connect(child,SIGNAL(Interface()),this,SLOT(comeback()));  //信号槽
    connect(child,SIGNAL(Inter_no()),this,SLOT(comeback_no())); //信号槽
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_2_clicked()//退出
{
    exit(-1);
}

void Widget::on_pushButton_clicked()//登录
{
    QString name = ui->lineEdit->text();
    QString pw = ui->lineEdit_2->text();
    child->log_in(name,pw);  //发送登录信息
}

void Widget::comeback()//信号槽,关闭父窗口
{
    this->hide();
}

void Widget::comeback_no()//信号槽,登录失败
{
    QMessageBox::information(this,"登录失败!","用户名或密码错误!",QMessageBox::Yes,QMessageBox::Yes);
}
