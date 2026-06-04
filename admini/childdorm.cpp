#include "childdorm.h"
#include "ui_childdorm.h"

childdorm::childdorm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::childdorm)
{
    //这行指令是把设计的ui界面加进来
    ui->setupUi(this);
    this->setFixedSize(1500,800);//背景图尺寸
    // this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setWindowTitle(QStringLiteral("管理员界面"));
   this->setWindowIcon(QIcon(":/image/44.png"));//APP图标

    //窗口居住显示，显示在屏幕最中间
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    //这里解释设置这个窗口的属性，注释掉的地方是窗口背景图
    QPalette pal = this->palette();
    //pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/27.jpg")));//背景图
    setPalette(pal);

    //将tablewidgets的每个界面都设置为只读模式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置每一个界面中显示数据的tablewidget都设置自适应宽度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应


    db = QSqlDatabase::database(); //获取已有数据库连接
    if(!db.isOpen())
    {
       QMessageBox::warning(this,"提示","数据库连接失败");
    }


    Init();//页面初始化
}

childdorm::~childdorm()
{
    delete ui;
}

void childdorm::getname(QString name)
{
    ui->lineEdit->setText(name);
}

void childdorm::on_pushButton_clicked()//重新登录
{
    this->hide();
    Widget *child = new Widget();
    child->show();
}


void childdorm::on_pushButton_2_clicked()//退出
{
  exit(-1);
}


void childdorm::on_pushButton_3_clicked()//添加用户
{

    add_users *child = new add_users();
    child->show();
}

void childdorm::on_pushButton_5_clicked()//修改密码
{
    change_pw *child = new change_pw();
    child->show();
}

void childdorm::on_pushButton_6_clicked()//修改权限
{
    change_per *child = new change_per();
    child->show();
}


void childdorm::on_pushButton_8_clicked()//添加菜品
{
    Add_food *child = new Add_food();
    child->show();
}


void childdorm::on_pushButton_10_clicked()//修改菜品价格
{
    change_price *child = new change_price;
    child->show();
}

void childdorm::on_pushButton_11_clicked()//修改菜品状态
{
    change_status *child = new change_status;
    child->show();
}

void childdorm::on_pushButton_13_clicked()//添加饮品
{
    Add_drinks *child = new Add_drinks;
    child->show();
}

void childdorm::on_pushButton_15_clicked()//修改饮品价格
{
    Modify_drinks *child = new Modify_drinks;
    child->show();
}

void childdorm::on_pushButton_16_clicked()//修改饮品状态
{
    Modify_stutus *child = new Modify_stutus;
    child->show();
}

void childdorm::on_pushButton_18_clicked()//账单详情
{
    Bill *child = new Bill;
    child->show();
}


//进行页面初始化
void childdorm::Init()
{
    MenuVec.clear();

    //初始化一个SQL查询对象
    QSqlQuery query;
    //获得菜单信息，返回值是是否读到信息的标志位。如果query在定义的时候构造函数没有给参数，那么exec后面就必须给参数
    //其实这个参数就是告诉系统我们读取的是哪一个表
    //select *from 菜单，*表示我们要查询这个数据列表里的所有字段，也可以换成字段名，或者某一个字段对应的索引，这个是sql语言
    //exec会判断语句是否执行成功
    bool ret = query.exec(QString::fromUtf8("select *from 菜单"));//数据库执行语句
    //没有读取到就打印错误信息
    if(!ret)
    {
        //SQL查询对象函数调用，返回值是一个类，获取错误信息，并且跳出窗口进行提示，第二行是一个拼接的函数
        QSqlError error = query.lastError();
        QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
        QMessageBox::warning(this,"提示",str);
    }

    //放进循环读取每一行的数据，next函数的作用是移动到下一行，并且返回是否有数据的标志位
    while (query.next())   //读取下一行信息
    {
        //value读取这一行的这一列的数据，这里可以使用字段名和索引的方式，也就是0、1、2这样的数字方式，并且转化为Qstring的形式
        QString str = query.value("序号").toString();
        QString str1 = query.value("菜名").toString();
        QString str2 = query.value("价格").toString();
        QString str3 = query.value("状态").toString();
        //定义字符数组，用来存放前面获取到的数据
        char Number[10];
        char food[100];
        char Price[10];
        char State[20];
        //把数据库读取出来的Qstring转化为QByteArray，放进定义的字符数组中，数组长度加1是为了将字符串结束的符号也加进去。
        QByteArray ba = str.toUtf8();
        memcpy(Number,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str1.toUtf8();
        memcpy(food,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str2.toUtf8();
        memcpy(Price,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str3.toUtf8();
        memcpy(State,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        //这里是将读取出来的一条记录中的多个字段进行组合，组合进自己定义的一个结构体里面，然后放进这样一个容器中
        strcpy(M_head.number,Number);
        strcpy(M_head.food, food);
        strcpy(M_head.price ,Price);
        strcpy(M_head.state , State);
        MenuVec.push_back(M_head);//菜单信息放入容器中
    }


    //上面是在菜单的数据表中进行读取，现在在饮品这个菜单中进行读取，下面的操作除了读取的sql语句不一样，其他都一样
    //但是有一个问题是，菜品和饮品是放在一个容器里的
    ret = query.exec(QString::fromUtf8("select *from 饮品"));//数据库执行语句
    if(!ret)
    {
        QSqlError error = query.lastError();
        QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
        QMessageBox::warning(this,"提示",str);
    }


    while (query.next())   //读取下一行信息
    {

        QString str = query.value("序号").toString();
        QString str1 = query.value("饮品名").toString();
        QString str2 = query.value("价格").toString();
        QString str3 = query.value("状态").toString();

        char Number[10];
        char food[100];
        char Price[10];
        char State[20];
        QByteArray ba = str.toUtf8();
        memcpy(Number,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str1.toUtf8();
        memcpy(food,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str2.toUtf8();
        memcpy(Price,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        ba = str3.toUtf8();
        memcpy(State,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
        strcpy(M_head.number,Number);
        strcpy(M_head.food, food);
        strcpy(M_head.price ,Price);
        strcpy(M_head.state , State);
        MenuVec.push_back(M_head);//菜单信息放入容器中
    }

    Menu_print();//打印菜单
    User_print();//打印用户
    Bill_print();//打印账单
}

//下面是将数据库读取到Qt界面上
void childdorm::Menu_print() //打印菜单
{
    //这里定义一个字符串数组，用来后面初始化不同页面的表头
    QStringList headtext;
    headtext<<"序号"<<"餐名"<<"价格"<<"状态";
    ui->tableWidget_3->setColumnCount(headtext.count());//列表列数设置和我们定义的表头数组一样
    ui->tableWidget_3->setHorizontalHeaderLabels(headtext);//插入表头，将字符串放进去
    //这是一个关键语句，需要先把除了表头意外的数据清零，因为后面需要从数据库读取数据，进行刷新数据。
    //如果这里不清楚，后面读取的时候会重复出现，因为后面刷新也是使用这个函数重新显示的
    ui->tableWidget_3->setRowCount(0);

    QStringList headtext1;
    headtext1<<"序号"<<"饮品名"<<"价格"<<"状态";
    ui->tableWidget_4->setColumnCount(headtext1.count());//列表设置为和headtext相等
    ui->tableWidget_4->setHorizontalHeaderLabels(headtext1);//插入表头
    ui->tableWidget_4->setRowCount(0);

    for (size_t j=0;j< MenuVec.size();j++)
    {
        QString temp = QString(MenuVec[j].number);
        int number = temp.toInt();
        if( number < 20000)//这里的根据将容器里的编号，然后区分饮品和菜品，是因为数据库里面给的序号是这样设置的，菜品序号都是20000以下的
        {
            //这里是先获取这里有多少行了，然后插入一行
            //比如有五行，insertRow（5），那么正好是插入索引为5的，正好就是第六行
            int rowcount = ui->tableWidget_3->rowCount();
            ui->tableWidget_3->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(MenuVec[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(MenuVec[j].food);
            QTableWidgetItem *column2 = new QTableWidgetItem(MenuVec[j].price);
            QTableWidgetItem *column3 = new QTableWidgetItem(MenuVec[j].state);

            ui->tableWidget_3->setItem(rowcount,0,column);
            ui->tableWidget_3->setItem(rowcount,1,column1);
            ui->tableWidget_3->setItem(rowcount,2,column2);
            ui->tableWidget_3->setItem(rowcount,3,column3);

        }
        else
        {
            int rowcount = ui->tableWidget_4->rowCount();
            ui->tableWidget_4->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(MenuVec[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(MenuVec[j].food);
            QTableWidgetItem *column2 = new QTableWidgetItem(MenuVec[j].price);
            QTableWidgetItem *column3 = new QTableWidgetItem(MenuVec[j].state);

            ui->tableWidget_4->setItem(rowcount,0,column);
            ui->tableWidget_4->setItem(rowcount,1,column1);
            ui->tableWidget_4->setItem(rowcount,2,column2);
            ui->tableWidget_4->setItem(rowcount,3,column3);
        }

     }
}


//这里的显示操作和上面是一样的，但是上面是一次性读取出来，然后进行显示处理
//但是这里可能是因为数据比较少，所以直接读取，然后显示了。
void childdorm::User_print() //打印用户
{
    //获取连接到的数据库
    QSqlQuery query(db);
      //从中读取用户信息
     bool ret = query.exec(QString::fromUtf8("select *from 用户"));//数据库执行语句
      if(!ret)
      {
          QSqlError error = query.lastError();
          QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
          QMessageBox::warning(this,"提示",str);
      }

      //tablewifget操作，操作同上
      QStringList headtext;
      headtext<<"用户名"<<"密码"<<"权限";

      ui->tableWidget->setColumnCount(headtext.count());//列表设置为和headtext相等
      ui->tableWidget->setHorizontalHeaderLabels(headtext);//插入表头

      ui->tableWidget->setRowCount(0);
      while (query.next())   //读取下一行信息
      {
          int rowcount = ui->tableWidget->rowCount();

          ui->tableWidget->insertRow(rowcount);


          QTableWidgetItem *column = new QTableWidgetItem(query.value("用户名").toString());
          QTableWidgetItem *column1 = new QTableWidgetItem(query.value("密码").toString());
          QTableWidgetItem *column2 = new QTableWidgetItem(query.value("权限").toString());

          ui->tableWidget->setItem(rowcount,0,column);
          ui->tableWidget->setItem(rowcount,1,column1);
          ui->tableWidget->setItem(rowcount,2,column2);

      }
}


//这里的账单信息也是直接读取显示
void childdorm::Bill_print() //打印账单
{
    QSqlQuery query(db);
      //打印账单信息
     bool ret = query.exec(QString::fromUtf8("select *from 账单"));//数据库执行语句
      if(!ret)
      {
          QSqlError error = query.lastError();
          QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
          QMessageBox::warning(this,"提示",str);
      }

      //tablewifget操作
      QStringList headtext;
      headtext<<"交易号"<<"时间"<<"桌号"<<"消费额"<<"收银人";

      ui->tableWidget_2->setColumnCount(headtext.count());//列表设置为和headtext相等
      ui->tableWidget_2->setHorizontalHeaderLabels(headtext);//插入表头

      ui->tableWidget_2->setRowCount(0);
      while (query.next())   //读取下一行信息
      {
          int rowcount = ui->tableWidget_2->rowCount();

          ui->tableWidget_2->insertRow(rowcount);

          QTableWidgetItem *column = new QTableWidgetItem(query.value("交易号").toString());
          QTableWidgetItem *column1 = new QTableWidgetItem(query.value("时间").toString());
          QTableWidgetItem *column2 = new QTableWidgetItem(query.value("桌号").toString());
          QTableWidgetItem *column3 = new QTableWidgetItem(query.value("消费额").toString());
          QTableWidgetItem *column4 = new QTableWidgetItem(query.value("收银人").toString());

          ui->tableWidget_2->setItem(rowcount,0,column);
          ui->tableWidget_2->setItem(rowcount,1,column1);
          ui->tableWidget_2->setItem(rowcount,2,column2);
          ui->tableWidget_2->setItem(rowcount,3,column3);
          ui->tableWidget_2->setItem(rowcount,4,column4);
      }
}

//这里是界面上的几个按钮，都是刷新显示，但是其实就是重新调用上面的函数，重新加在一下数据库里面的函数
void childdorm::on_pushButton_7_clicked()//刷新用户
{
    User_print();
}

void childdorm::on_pushButton_19_clicked()//刷新账单
{
   Bill_print();
}

void childdorm::on_pushButton_17_clicked()//刷新饮品
{
    Init();
}

void childdorm::on_pushButton_12_clicked()//刷新菜单
{
     Init();
}



//这个是删除用户，但是里面有的地方写的不太好
void childdorm::on_pushButton_4_clicked()//删除用户
{
    //获取当前连接
     QSqlQuery query(db);
     //这个数组用来存放后面拼接出来的sql语句
     char str[100];
     //获取当前被选中的行索引
     int currow = ui->tableWidget->currentRow();
     //取出用户名
     QString name= ui->tableWidget->item(currow,0)->text();
     //把这个Qt字符串转化成C++标准字符串，因为后面的sprintf这个函数不能处理Qt的Qstring函数
     std::string s1 = name.toStdString();
     //这里是在判断，有没有选中，没选中某一行，就会返回-1
     if(currow == -1)
     {
         //这里有个bug，你先取索引，有用索引取了记录里的用户名，才判断是否是-1，很有可能在取用户名的地方崩溃
         QMessageBox::warning(this,"提示","操作有误!");
     }
     else
     {
         //如果成功选中，就在这里拼接sql语句，函数c_str是将字符串换成C语言风格，也就是sprintf能处理的字符数组形式
         sprintf(str,"delete from 用户 where 用户名 = '%s'",s1.c_str());
        //执行操作
         query.exec(str);
        //这里的arg是这里的固定用法，就是替换%1的
        //这段我认为就是判断是否删除成功的
        QString temp=QString::fromUtf8("select 密码 from 用户 where 用户名 ='%1'").arg(name);
         query.exec(temp);
        //这里也不规范，如果查到没有，你还去读取这个字段的数据
          query.next();
          if(query.value("密码").toString() != "\0")
          {
              QMessageBox::warning(this,"提示","删除失败!");
          }
          else
          {
              ui->tableWidget->removeRow(currow);//删除数据库里的数据以后，在显示的窗口上也删除当前行
              QMessageBox::warning(this,"提示","删除成功!");

          }

     }
}


//这里删除菜品的思路和上面的一致，只是所取的窗口不一样
void childdorm::on_pushButton_9_clicked() //删除菜
{
    QSqlQuery query(db);
    char str[100];
    int currow = ui->tableWidget_3->currentRow();//当前行
    QString number= ui->tableWidget_3->item(currow,0)->text();

    std::string s1 = number.toStdString();

    if(currow == -1)
    {
        QMessageBox::warning(this,"提示","操作有误!");
    }
    else
    {
        sprintf(str,"delete from 菜单 where 序号 = '%s'",s1.c_str());

        query.exec(str);

         QString temp=QString::fromUtf8("select 菜名 from 菜单 where 序号 ='%1'").arg(number);
         query.exec(temp);
         query.next();
         if(query.value("菜名").toString() != "\0")
         {
             QMessageBox::warning(this,"提示","删除失败!");
         }
         else
         {
             ui->tableWidget_3->removeRow(currow);//删除当前行
             QMessageBox::warning(this,"提示","删除成功!");

         }

    }
}


//删除饮品和上面的一致，只是窗口不一样
void childdorm::on_pushButton_14_clicked()//删除饮品
{
    QSqlQuery query(db);
    char str[100];
    int currow = ui->tableWidget_4->currentRow();//当前行
    QString number= ui->tableWidget_4->item(currow,0)->text();

    std::string s1 = number.toStdString();

    if(currow == -1)
    {
        QMessageBox::warning(this,"提示","操作有误!");
    }
    else
    {
        sprintf(str,"delete from 饮品 where 序号 = '%s'",s1.c_str());

        query.exec(str);

         QString temp=QString::fromUtf8("select 饮品名 from 菜单 where 序号 ='%1'").arg(number);
         query.exec(temp);
         query.next();
         if(query.value("饮品名").toString() != "\0")
         {
             QMessageBox::warning(this,"提示","删除失败!");
         }
         else
         {
             ui->tableWidget_4->removeRow(currow);//删除当前行
             QMessageBox::warning(this,"提示","删除成功!");

         }

    }
}
