#include "add_users.h"
#include "ui_add_users.h"
#include <QDesktopWidget>
#include<QComboBox>
#include<QDebug>
add_users::add_users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_users)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(600,400);//背景图尺寸
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

add_users::~add_users()
{
    delete ui;
}

void add_users::on_pushButton_2_clicked()//取消
{
    close();
//    CDA_Plot = new FrequencyPlot();netne
//    CDA_Plot->setAttribute(Qt::WA_DeleteOnClose); //设置子窗口属性，在子窗口关闭之后，释放子窗口的资源(释放指针)
}

void add_users::on_pushButton_clicked() //添加用户
{
    QSqlQuery query(db);
    char str[100];
    QString name = ui->lineEdit->text();//用户名
    QString pwm = ui->lineEdit_2->text();//密码
    QString limit = ui->comboBox->currentText();//权限

    std::string s1 = name.toStdString();
    std::string s2 = pwm.toStdString();
    std::string s3 = limit.toStdString();

    QString temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
    query.exec(temp);
    query.next();
    if(query.value("密码").toString() !=  "\0")
    {
        QMessageBox qm(this);
        qm.setText(QStringLiteral("该用户已被注册!\n请重新输入!"));
        qm.move(this->geometry().center());
        qm.exec();
    }
    else
    {
        sprintf(str,"insert into 用户 values('%s','%s','%s')",s1.c_str(),s2.c_str(),s3.c_str());
        query.exec(str);

         temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
         query.exec(temp);
          query.next();
          if(query.value("密码").toString() == pwm)
          {
              QMessageBox::warning(this,"提示","添加成功!");
              close();
          }
          else
          {
              QMessageBox::warning(this,"提示","添加失败!");
          }

    }


}
