#include "change_per.h"
#include "ui_change_per.h"
#include <QDesktopWidget>
change_per::change_per(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::change_per)
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

change_per::~change_per()
{
    delete ui;
}

void change_per::on_pushButton_2_clicked()//取消
{
     close();
}

void change_per::on_pushButton_clicked()//修改权限
{
    QSqlQuery query(db);
    char str[100];

    QString name = ui->lineEdit->text();//用户名
    QString pw = ui->lineEdit_2->text();//密码
    QString limit = ui->comboBox->currentText();//权限

    std::string s1 = name.toStdString();
    //std::string s2 = pwm.toStdString();
    std::string s3 = limit.toStdString();

    QString temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
    query.exec(temp);
    query.next();
    if(query.value("密码").toString() !=  pw)
    {
        QMessageBox::warning(this,"提示","用户名或密码错误!");
    }
    else
    {
        sprintf(str,"update  用户 set 权限 = '%s' where 用户名 = '%s'",s3.c_str(),s1.c_str());
        query.exec(str);
         //qDebug()<<str;
         temp=QString::fromUtf8("select 权限 from 用户 where 用户名 ='%1'").arg(name);
         query.exec(temp);
          query.next();
          if(query.value("权限").toString() == limit)
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
