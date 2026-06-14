#include "widget.h"
#include "ui_widget.h"
#include <QDesktopWidget>
#include<QSqlQuery>
#include<QDebug>
#include<QString>
#include<cstring>
#include<QMessageBox>
#include<QSqlError>
#include<QHostAddress>
#include<QTableWidgetItem>
#include<QApplication>
#include <QVariant>
#include<unistd.h>
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //窗口的基本属性设置
    ui->setupUi(this);
    ui->tabWidget->setTabText(0,"主餐");
    ui->tabWidget->setTabText(1,"饮品");
    this->setFixedSize(1500,800);//背景图尺寸
    this->setWindowTitle(QStringLiteral("点餐窗口"));
   // this->move(QPoint(100,100));
    this->setWindowIcon(QIcon(":/image/28.png"));//APP图标

    //居中显示
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    //添加背景图片
    QPalette pal = this->palette();
    //pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/27.jpg")));//背景图
    //这行代码的意思是重新应用配置好的调色板
    setPalette(pal);
    //这个是数码管的管控件的设置，这里是总价格显示的数字的属性设置，flat意思是显示效果是平面的
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    //设置为只读模式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    //这两行代码是点击菜品会将对应菜品的序号放进序号框的功能
    connect(ui->tableWidget_2,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(onMenuItemClicked(QTableWidgetItem*)));
    connect(ui->tableWidget_3,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(onMenuItemClicked(QTableWidgetItem*)));

    Init();//初始化
    client();//客服端操作

    QStringList headtext2;
    headtext2<<"序号"<<"餐名"<<"价格"<<"数量"<<"备注";
    ui->tableWidget->setColumnCount(headtext2.count());//列表设置为和headtext相等
    ui->tableWidget->setHorizontalHeaderLabels(headtext2);//插入表头
    ui->tableWidget->setRowCount(0);
    m_sum = 0;

    child = new childdorm();
    child->hide();
    child->getData(this);   //通过this把父窗口的两个容器指针传到子窗口当中
    connect(child,SIGNAL(Interface()),this,SLOT(comeback()));//信号槽
    connect(child,SIGNAL(Inter_delete()),this,SLOT(Delete()));//信号槽
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Init()//初始化
{
    tcpsocket = new QTcpSocket(this);//创建客户端套接字
    connect(tcpsocket,&QTcpSocket::connected,//请求连接信号
           [=]()
    {
        //ui->textEdit->setText("成功连接服务器");
    }
    );
    //端口和ip
    QString ip = "127.0.0.1";
    qint16 port = 8888;
    tcpsocket->connectToHost(QHostAddress(ip),port);
    //这里其实就是赋初值
    strcpy(F_head.table,"\0");
    strcpy(F_head.number,"\0");
    strcpy(F_head.food,"order");  //放入标志位
    strcpy(F_head.price,"\0");
    strcpy(F_head.price,"\0");
    strcpy(F_head.post,"\0");
    FoodVec.push_back(F_head);//订单信息标志位放入容器中首元素

    strcpy(F_head.food,"order_up");  //放入标志位
    FoodVec_no.push_back(F_head);//订单信息标志位放入容器中首元素
}

void Widget::client()        //客服端操作
{
    //这里是请求菜单
    strcpy(F_head.table,"\0");
    strcpy(F_head.number,"\0");
    strcpy(F_head.food,"menu");  //放入标志位
    strcpy(F_head.price,"\0");
    strcpy(F_head.price,"\0");
    strcpy(F_head.post,"\0");
    foodVec.push_back(F_head);//订单信息标志位放入容器中首元素
    QByteArray a1;
    a1.resize(sizeof(FoodInfo)*foodVec.size());
    memcpy(a1.data(),foodVec.data(),sizeof(FoodInfo)*foodVec.size());
    tcpsocket->write(a1);//这样就将请求订单的信息发送出去
    //这是接收数据，接收服务端传来的数据
    connect(tcpsocket,&QTcpSocket::readyRead,
            [=]()
    {
        //定义数组接收服务端传回的数据
        QByteArray array = tcpsocket->readAll();
        flag(array);     //数据解析
    }
    );
}

void Widget::flag(QByteArray array) //服务端信息判断
{
    vector<MenuInfo> menuVec; //定义一个菜单容器
    menuVec.resize(array.size()/sizeof(MenuInfo));//重新定义容器长度
    memcpy(menuVec.data(),array.data(),array.size());//把读取到的数据放进定义的容器里
    //这里是打印订单传回来的容器的首个数据，这个数据代表的是容器中数据的信息
    qDebug()<<menuVec[0].food;
    if(strcmp(menuVec[0].food,"menu")==0)
    {
        //如果传来的是菜单，就存一下，然后显示在窗口上
        MenuVec = menuVec;
        Menu_print();
    }
    else if(strcmp(menuVec[0].food,"chef_in")==0)
    {
        //如果传来的是chef_in就说明这个是厨房传来的，然后需要更新已经上菜和未上菜的菜单
        Chef_in(menuVec);
    }
    else if(strcmp(menuVec[0].food,"order_ok")==0)
    {
        QMessageBox::information(this, "提示", "订单提交成功!");
    }
    else if(strcmp(menuVec[0].food,"order_fail")==0)
    {
        QMessageBox::warning(this, "提示", "订单提交失败，请检查桌号和服务端状态!");
    }
    else if(strcmp(menuVec[0].food,"delete_ok")==0)
    {
        QMessageBox::information(this, "提示", "退餐成功!");
    }
    else if(strcmp(menuVec[0].food,"delete_fail")==0)
    {
        QMessageBox::warning(this, "提示", "操作失败!");
    }

}

void Widget::Chef_in(vector<MenuInfo> menuVec)//上菜
{
    //这个判断只是用来判断传回数组是不是合法，如果只有一个元素，这里应该就是不正常的，直接返回
    if(menuVec.size() <= 1)
    {
        return;
    }

    bool moved = false;
    //遍历整个未上菜的容器，
    for(int i = 0; i < FoodVec_no.size(); i++)
    {
        //这里是判断服务端给的数组中的这个条记录，一般只有一个元素，因为厨师端是一个一个上菜的
        //判断传来的这一条和未上菜的容器里的某一条记录一致
        //这里数量quality和state相比仅仅是因为那个结构体里没有quality，临时用一个state这个位置
        if(strcmp(FoodVec_no[i].number,menuVec[1].number) == 0 &&
                strcmp(FoodVec_no[i].price,menuVec[1].price) == 0 &&
                strcmp(FoodVec_no[i].quatity,menuVec[1].state) == 0)
        {
            //找到以后复制放到已上菜的容器中
            strcpy(F_head.table,FoodVec_no[i].table);
            strcpy(F_head.number,FoodVec_no[i].number);
            strcpy(F_head.food,FoodVec_no[i].food);
            strcpy(F_head.price,FoodVec_no[i].price);
            strcpy(F_head.quatity,FoodVec_no[i].quatity);
            strcpy(F_head.post,FoodVec_no[i].post);
            FoodVec_in.push_back(F_head);
            //删除未上菜的菜单中的
            FoodVec_no.erase(FoodVec_no.begin()+i);
            moved = true;
            break;
        }
    }

    //如果没有找到，刷新界面，然后退出
    if(!moved)
    {
        //调用窗口的这个函数，将新生成的容器传给子窗口
        child->getvec(FoodVec_no,FoodVec_in);
        return;
    }

    //如果找到了，并且成功更新两个容器，就会执行到这里
    for(int i = 1; i < FoodVec.size(); i++)
    {
        if(strcmp(FoodVec[i].number,menuVec[1].number) == 0 &&
                strcmp(FoodVec[i].price,menuVec[1].price) == 0 &&
                strcmp(FoodVec[i].quatity,menuVec[1].state) == 0 &&
                strcmp(FoodVec[i].table,"0") != 0)
        {
            //这里进行遍历，又借用了一下这个table位置，当作是否上菜的标志位了，和上面一样
            strcpy(FoodVec[i].table,"0");
            break;
        }
    }
    //如果找到，就更新这两个容器在子窗口上的显示
    child->getvec(FoodVec_no,FoodVec_in);
}

//这个函数的意思就是把接收到的菜单放进ui显示的窗口里去
void Widget::Menu_print()//打印菜单
{
    QStringList headtext;
    headtext<<"序号"<<"餐名"<<"价格"<<"状态";
    ui->tableWidget_2->setColumnCount(headtext.count());     //列表设置为和headtext相等
    ui->tableWidget_2->setHorizontalHeaderLabels(headtext);  //插入表头
    ui->tableWidget_2->setRowCount(0);

    QStringList headtext1;
    headtext1<<"序号"<<"饮品名"<<"价格"<<"状态";
    ui->tableWidget_3->setColumnCount(headtext1.count());    //列表设置为和headtext相等
    ui->tableWidget_3->setHorizontalHeaderLabels(headtext1); //插入表头
    ui->tableWidget_3->setRowCount(0);

    for (int j=1;j< MenuVec.size();j++)
    {
        QString temp = QString(MenuVec[j].number);
        int number = temp.toInt();
        if( number < 20000)
        {
            int rowcount = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(rowcount);
            QTableWidgetItem *column = new QTableWidgetItem(MenuVec[j].number);
            QTableWidgetItem *column1 = new QTableWidgetItem(MenuVec[j].food);
            QTableWidgetItem *column2 = new QTableWidgetItem(MenuVec[j].price);
            QTableWidgetItem *column3 = new QTableWidgetItem(MenuVec[j].state);

            ui->tableWidget_2->setItem(rowcount,0,column);
            ui->tableWidget_2->setItem(rowcount,1,column1);
            ui->tableWidget_2->setItem(rowcount,2,column2);
            ui->tableWidget_2->setItem(rowcount,3,column3);
        }
        else
        {
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
     }
}


void Widget::on_pushButton_3_clicked()//提交订单
{
    //因为在结构体里面，桌号使用的是C语言风格的char类型的数组，所以这里进行了一个转换
    QString S = ui->lineEdit->text(); //桌号
    char s[5];
    QByteArray ba = S.toUtf8();
    memcpy(s,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止


    if(S == "\0")
    {
        QMessageBox::warning(this, "提示", "请填写桌号!");
    }
    else
    {
        //如果已经正确使用输入桌号
        strcpy(FoodVec[0].table,s);
        int rowcount = ui->tableWidget->rowCount();//获取总行数
        if(rowcount == 0)
        {
            //这个框里行数等于0，说明这里没添加菜品，直接返回
            return;
        }
        //如果有菜品，执行提交
        int validCount = 0;
        //遍历每一条记录
        for (int j=0;j<rowcount;j++)
        {
            QString str = ui->tableWidget->item(j,0)->text(); //获取序号
            QString str1 = ui->tableWidget->item(j,1)->text();//获取菜名
            QString str2 = ui->tableWidget->item(j,2)->text();//获取价格
            QString str3 = ui->tableWidget->item(j,3)->text();//获取价格
            QString str4 = ui->tableWidget->item(j,4)->text();//获取备注
            //这里是进行排除，不是有效的菜品来记录就跳过
            if(str.trimmed().isEmpty() || str1 == "menu")
            {
                continue;
            }

            validCount++;


            strcpy(F_head.table,s);
            ba = str.toUtf8();
            memcpy(F_head.number,ba.data(),ba.size()+1);  //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
            ba = str1.toUtf8();
            memcpy(F_head.food,ba.data(),ba.size()+1);    //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
            ba = str2.toUtf8();
            memcpy(F_head.price,ba.data(),ba.size()+1);   //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
            ba = str3.toUtf8();
            memcpy(F_head.quatity,ba.data(),ba.size()+1); //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
            ba = str4.toUtf8();
            memcpy(F_head.post,ba.data(),ba.size()+1);    //加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止

            FoodVec.push_back(F_head);//有效菜品信息放入容器中
            FoodVec_no.push_back(F_head);//有效菜品信息放入未上菜单容器中
        }

        if(validCount == 0)
        {
            //有效菜品为0
            return;
        }

        this->hide();
        child->getvec(FoodVec_no,FoodVec_in);
        child->show();

        //向服务端发送订单信息
        QByteArray a1;
        a1.resize(sizeof(FoodInfo)*FoodVec.size());
        memcpy(a1.data(),FoodVec.data(),sizeof(FoodInfo)*FoodVec.size());
        tcpsocket->write(a1);
    }

}

void Widget::on_pushButton_4_clicked()//退出
{
    QApplication::quit();
}

//这个函数是将点击的某条菜品的序号，放进点餐的输入框里
void Widget::onMenuItemClicked(QTableWidgetItem *item)
{
    if(item == NULL)
    {
        return;
    }

    QTableWidget *table = item->tableWidget();
    if(table == NULL || table->item(item->row(),0) == NULL)
    {
        return;
    }
    //输入菜品的序号框
    ui->lineEdit_2->setText(table->item(item->row(),0)->text());
    //让当前控件获得焦点
    ui->lineEdit_3->setFocus();
}

void Widget::on_pushButton_clicked()//添加菜品
{
    int flag = 0;
    char number[10];
    QString s = ui->lineEdit_2->text();//餐品序号
    if(s.trimmed().isEmpty())
    {
        return;
    }

    QByteArray ba = s.toUtf8();
    memcpy(number,ba.data(),ba.size()+1);//加1是为了最后的终结符，否则转换回来的时候不知道什么时候截止
    //这个类是一个单元格的类
    QTableWidgetItem *column = new QTableWidgetItem(s);
    QString s1;
    QString s2;
    //这里遍历是为了在整个菜单中找到我们选定的那个菜品的完整的信息，用s1和s2把搜到的菜品的价格和名称存一下
    for(int i = 1; i < MenuVec.size(); i++)
    {
        if(strcmp(MenuVec[i].number,number) == 0)
        {
            s1 = QString(MenuVec[i].food);
            s2 = QString(MenuVec[i].price);
            flag = 1;
        }
    }

    QTableWidgetItem *column1 = new QTableWidgetItem(s1);

    QTableWidgetItem *column2 = new QTableWidgetItem(s2);

    QString s3 = ui->lineEdit_3->text();//餐品数量
    QTableWidgetItem *column3 = new QTableWidgetItem(s3);

    QString s4 = ui->lineEdit_4->text();//备注
    QTableWidgetItem *column4 = new QTableWidgetItem(s4);

    //这是找到的标志位，将它放在同一行里
    if(flag == 1)
    {
        //这里是获取总行是，因为有个零行，所以这里插入的行数索引不用+1
        int rowcount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowcount);
        ui->tableWidget->setItem(rowcount,0,column);
        ui->tableWidget->setItem(rowcount,1,column1);
        ui->tableWidget->setItem(rowcount,2,column2);
        ui->tableWidget->setItem(rowcount,3,column3);
        ui->tableWidget->setItem(rowcount,4,column4);
        //更新总价格，这里显示是使用LCD的方式
        m_sum = m_sum+s2.toFloat()*s3.toInt();
        ui->lcdNumber->display(m_sum);
    }
    else
    {
        QMessageBox::warning(this, "提示", "输入有误!");
    }

}

void Widget::on_pushButton_2_clicked() //删除
{
    if(ui->tableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, "提示", "操作有误!");
        return;
    }

    //这个是获取当前行的函数
    int currow = ui->tableWidget->currentRow();
    if(currow == -1)
    {
        QMessageBox::warning(this, "提示", "操作有误!");
        return;
    }

    QTableWidgetItem *priceItem = ui->tableWidget->item(currow,2);
    QTableWidgetItem *quantityItem = ui->tableWidget->item(currow,3);
    if(priceItem != NULL && quantityItem != NULL)
    {
        //更新价格
        m_sum -= priceItem->text().toFloat() * quantityItem->text().toInt();
        if(m_sum < 0)
        {
            m_sum = 0;
        }
        ui->lcdNumber->display(m_sum);
    }

    //删除当前行，取消整个表格的选中状态，防止补上来的一行还是在高亮的选中状态
    ui->tableWidget->removeRow(currow);
    ui->tableWidget->clearSelection();
    //这一行是将整个表格设置为无效状态，避免还有行被选中
    ui->tableWidget->setCurrentCell(-1,-1);
}

void Widget::comeback()//信号槽,显示父窗口
{
    //判断该桌号是否点过餐，就是说容器开头等于结尾，说明没有数据
    if(FoodVec.begin() != FoodVec.end())
    {
        QString Table = FoodVec[0].table;
        //有点细节，这里是返回点餐界面，读取这个订单的桌号，放回桌号的方框，然后不允许修改桌号
        ui->lineEdit->setText(Table);
        ui->lineEdit->setFocusPolicy(Qt::NoFocus);  //设置为只读模式，其实是无法获得鼠标焦点了
    }
    //清空右侧订单
    ui->tableWidget->setRowCount(0);
    m_sum = 0;
    this->show();
}

void Widget::Delete()//信号槽,退餐
{
    child->Delete_food();
}
