#include "modify_drinks.h"
#include "ui_modify_drinks.h"
#include <QDesktopWidget>
Modify_drinks::Modify_drinks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modify_drinks)
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

Modify_drinks::~Modify_drinks()
{
    delete ui;
}

void Modify_drinks::on_pushButton_clicked()//修改饮品价格
{
    QSqlQuery query(db);

    QString number = ui->lineEdit->text().trimmed();//饮品名或序号
    QString price = ui->lineEdit_2->text();//价格

    QString temp=QString::fromUtf8("select 饮品名 from 饮品 where 序号 ='%1' or 饮品名 ='%1'").arg(number);
    if(!query.exec(temp) || !query.next())
    {
        QMessageBox::warning(this,"提示","饮品不存在!请重新输入!");
    }
    else
    {
        temp=QString::fromUtf8("update 饮品 set 价格 = '%1' where 序号 = '%2' or 饮品名 = '%2'").arg(price).arg(number);
        query.exec(temp);
         //qDebug()<<str;
         temp=QString::fromUtf8("select 价格 from 饮品 where 序号 ='%1' or 饮品名 ='%1'").arg(number);
         query.exec(temp);
          if(query.next() && query.value("价格").toString() == price)
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

void Modify_drinks::on_pushButton_2_clicked()//关闭
{
    close();
}
