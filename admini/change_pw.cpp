#include "change_pw.h"
#include "ui_change_pw.h"
#include <QDesktopWidget>
change_pw::change_pw(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::change_pw)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(600,400);//背景图尺寸
    //this->move(QPoint(100,100));
    this->setWindowIcon(QIcon(":/image/44.png"));//APP图标

   //窗口居中显示
   QDesktopWidget* desktop = QApplication::desktop();
   move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/31.jpg")));//背景图
    setPalette(pal);

    db = QSqlDatabase::database(); //获取已有数据库连接
    if(!db.isOpen())
    {
       QMessageBox::warning(this,"提示","数据库连接失败");
    }
}

change_pw::~change_pw()
{
    delete ui;
}

void change_pw::on_pushButton_2_clicked()//取消
{
     close();
}

void change_pw::on_pushButton_clicked()//更改密码
{
    QSqlQuery query(db);
    char str[100];

    QString name = ui->lineEdit->text();//用户名
    QString pw = ui->lineEdit_2->text();//旧密码
    QString new_pw = ui->lineEdit_3->text();//新密码

    std::string s1 = name.toStdString();
    //std::string s2 = pw.toStdString();
    std::string s3 = new_pw.toStdString();

    QString temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
    query.exec(temp);
    query.next();
    if(query.value("密码").toString() !=  pw)
    {
        QMessageBox::warning(this,"提示","用户名或密码错误!");
    }
    else
    {
        sprintf(str,"update  用户 set 密码 = '%s' where 用户名 = '%s'",s3.c_str(),s1.c_str());
        query.exec(str);
         //qDebug()<<str;
         temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
         query.exec(temp);
          query.next();
          if(query.value("密码").toString() == new_pw)
          {
              QMessageBox::warning(this,"提示","修改成功!");
              close();
          }
          else
          {
              QMessageBox::warning(this,"提示","修改失败!");
          }

    }
}
