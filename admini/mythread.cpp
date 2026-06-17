#include "mythread.h"
#include <cstring>

QTcpSocket *mythread::chef_socket = NULL;
QTcpSocket *mythread::cashier_socket = NULL;
QTcpSocket *mythread::order_socket[21] = {NULL};

namespace
{
void copyText(char *dest, int destSize, const QString &value)
{
    // 把Qt字符串转成结构体里的字符数组，先清空再拷贝，避免上一次的数据残留
    QByteArray data = value.toUtf8();
    memset(dest, 0, destSize);
    strncpy(dest, data.constData(), destSize - 1);
}

void clearFood(FoodInfo &food)
{
    memset(&food, 0, sizeof(FoodInfo));
}

void clearMenu(MenuInfo &menu)
{
    memset(&menu, 0, sizeof(MenuInfo));
}
}

mythread::mythread(QTcpSocket *socket)
{
    tcpsocket = socket;
    db = QSqlDatabase::database();
    Init();
}

void mythread::run()
{
    connect(tcpsocket, &QTcpSocket::readyRead,
            [=]() {
        QByteArray array = tcpsocket->readAll();
        Flag(array);
    });
}

void mythread::Flag(QByteArray array)
{
    // 客户端发来的数据都是FoodInfo结构体数组，第一个元素的food字段作为标志位
    Foodvec.clear();
    Foodvec.resize(array.size() / sizeof(FoodInfo));
    if(Foodvec.empty())
    {
        return;
    }

    memcpy(Foodvec.data(), array.data(), sizeof(FoodInfo) * Foodvec.size());
    qDebug() << Foodvec[0].food;

    // 根据第一个结构体里的food字段判断是哪一种客户端请求
    // menu/order/chef/cashier分别对应点餐端、厨师端和收银端的不同操作
    if(strcmp(Foodvec[0].food, "menu") == 0)
    {
        order_menu();
    }
    else if(strcmp(Foodvec[0].food, "order") == 0 || strcmp(Foodvec[0].food, "order_delete") == 0)
    {
        order_food();
    }
    else if(strcmp(Foodvec[0].food, "chef") == 0)
    {
        chef_socket = tcpsocket;
        chef_sent();
    }
    else if(strcmp(Foodvec[0].food, "chef_in") == 0)
    {
        chef_in();
    }
    else if(strcmp(Foodvec[0].food, "cashier") == 0)
    {
        cashier_socket = tcpsocket;
        cashier_in();
    }
    else if(strcmp(Foodvec[0].food, "cashier_query") == 0)
    {
        cashier_query();
    }
    else if(strcmp(Foodvec[0].food, "cashier_refresh") == 0)
    {
        cashier_refresh();
    }
    else if(strcmp(Foodvec[0].food, "cashier_finish") == 0)
    {
        cashier_finish();
    }
}

void mythread::cashier_in()
{
    // 收银端登录时，Foodvec[1]里放的是用户名和密码
    if(Foodvec.size() <= 1 || cashier_socket == NULL)
    {
        return;
    }

    QSqlQuery query(db);
    QString name = Foodvec[1].food;
    QString pw = Foodvec[1].post;
    QString temp = QString::fromUtf8("select 密码 from 用户 where 用户名='%1'").arg(name);

    query.exec(temp);
    query.next();
    if(query.value("密码").toString() == pw)
    {
        strcpy(Foodvec[0].food, "cashier_in");
    }
    else
    {
        strcpy(Foodvec[0].food, "cashier_no");
    }

    QByteArray a1;
    a1.resize(sizeof(FoodInfo) * Foodvec.size());
    memcpy(a1.data(), Foodvec.data(), sizeof(FoodInfo) * Foodvec.size());
    cashier_socket->write(a1);
}

void mythread::cashier_query()
{
    // 收银端根据桌号查询当前订单，这里查的是内存里的foodvec数组
    // foodvec数组是在点餐端提交订单时保存的，按桌号下标区分
    if(cashier_socket == NULL)
    {
        return;
    }

    int n = QString(Foodvec[0].table).toInt();
    // 如果该桌还没有点餐，就返回一个空的查询标志，收银端据此判断没有账单
    if(n <= 0 || n >= 21 || foodvec[n].empty())
    {
        Foodvec.clear();
        clearFood(F_head);
        strcpy(F_head.food, "cashier_query");
        Foodvec.push_back(F_head);
    }
    else
    {
        Foodvec = foodvec[n];
        strcpy(Foodvec[0].food, "cashier_query");
    }

    QByteArray a1;
    a1.resize(sizeof(FoodInfo) * Foodvec.size());
    memcpy(a1.data(), Foodvec.data(), sizeof(FoodInfo) * Foodvec.size());
    cashier_socket->write(a1);
}

void mythread::cashier_refresh()
{
    // 收银端刷新餐桌状态，直接从餐桌表读取当前每张桌子的状态
    if(cashier_socket == NULL)
    {
        return;
    }

    QSqlQuery query(db);
    query.exec(QString::fromUtf8("select *from 餐桌"));

    while(query.next())
    {
        clearFood(F_head);
        copyText(F_head.table, sizeof(F_head.table), query.value("桌号").toString());
        copyText(F_head.food, sizeof(F_head.food), query.value("状态").toString());
        Foodvec.push_back(F_head);
    }

    QByteArray a1;
    a1.resize(sizeof(FoodInfo) * Foodvec.size());
    memcpy(a1.data(), Foodvec.data(), sizeof(FoodInfo) * Foodvec.size());
    cashier_socket->write(a1);
}

void mythread::cashier_finish()
{
    // 收银端结账时，把当前桌号的订单写入账单和账单详情两张表
    QString Table = QString(Foodvec[0].table);
    int n = Table.toInt();
    QString Price = QString(Foodvec[0].price);
    QString Name = QString(Foodvec[0].post);
    QSqlQuery query(db);
    char sql[512];

    auto sendCashierFinishResult = [=](const char *result) {
        FoodInfo reply;
        clearFood(reply);
        strcpy(reply.food, result);

        QByteArray data;
        data.resize(sizeof(FoodInfo));
        memcpy(data.data(), &reply, sizeof(FoodInfo));
        cashier_socket->write(data);
    };

    if(n <= 0 || n >= 21)
    {
        sendCashierFinishResult("cashier_finish_fail");
        return;
    }

    if(foodvec[n].size() <= 1)
    {
        sendCashierFinishResult("cashier_finish_fail");
        return;
    }

    Foodvec = foodvec[n];

    // 这里根据已有账单最后一个交易号继续往后生成新的交易号
    query.exec(QString::fromUtf8("select *from 账单"));
    int count = 100000;
    while(query.next())
    {
        count = query.value("交易号").toInt();
    }
    if(count < 100001)
    {
        count = 100000;
    }
    count++;

    QString Num = QString::number(count);
    QString Time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    char num[10];
    char time[100];
    char table[5];
    char sprice[10];
    char name[100];
    copyText(num, sizeof(num), Num);
    copyText(time, sizeof(time), Time);
    copyText(table, sizeof(table), Table);
    copyText(sprice, sizeof(sprice), Price);
    copyText(name, sizeof(name), Name);

    bool success = db.transaction();

    sprintf(sql, "insert into 账单 values('%s','%s','%s','%s','%s')", num, time, table, sprice, name);
    success = query.exec(sql) && success;

    for(int j = 1; j < Foodvec.size(); j++)
    {
        // Foodvec[0]是标志位，从1开始才是真正的菜品数据
        sprintf(sql, "insert into 账单详情 values('%s','%s','%s','%s','%s','%s','%s','%s')",
                num, table, Foodvec[j].number, Foodvec[j].food, Foodvec[j].price,
                Foodvec[j].quatity, Foodvec[j].post, name);
        success = query.exec(sql) && success;
    }

    // 结账成功后，该桌重新变为空闲状态
    success = query.exec(QString::fromUtf8("update 餐桌 set 状态 = '未使用' where 桌号 = '%1'").arg(Table)) && success;

    if(success)
    {
        success = db.commit();
    }
    else
    {
        db.rollback();
    }

    if(success)
    {
        foodvec[n].clear();
        sprintf(sql, "delete from 厨房 where 桌号 = '%s'", table);
        query.exec(sql);
        sendCashierFinishResult("cashier_finish_ok");
    }
    else
    {
        sendCashierFinishResult("cashier_finish_fail");
    }
}

void mythread::order_menu()
{
    // 点餐端的菜单不是直接读数据库，而是由管理端初始化后通过TCP发送过去
    QByteArray a1;
    a1.resize(sizeof(MenuInfo) * MenuVec.size());
    memcpy(a1.data(), MenuVec.data(), sizeof(MenuInfo) * MenuVec.size());
    tcpsocket->write(a1);
}

void mythread::chef_sent()
{
    // chef_socket为空说明厨师端还没有连接，这种情况下不能直接发送数据
    if(chef_socket == NULL)
    {
        return;
    }

    Foodvec.clear();
    QSqlQuery query(db);
    // 厨师端登录或点餐端提交后，管理端从厨房表读取订单发给厨师端
    // 所以厨师端刚打开看到的内容，来源就是数据库里的“厨房”表
    query.exec(QString::fromUtf8("select *from 厨房"));

    while(query.next())
    {
        clearFood(F_head);
        copyText(F_head.table, sizeof(F_head.table), query.value("桌号").toString());
        copyText(F_head.number, sizeof(F_head.number), query.value("序号").toString());
        copyText(F_head.food, sizeof(F_head.food), query.value("菜名").toString());
        copyText(F_head.price, sizeof(F_head.price), query.value("价格").toString());
        copyText(F_head.quatity, sizeof(F_head.quatity), query.value("数量").toString());
        copyText(F_head.post, sizeof(F_head.post), query.value("备注").toString());
        Foodvec.push_back(F_head);
    }

    if(Foodvec.empty())
    {
        // 厨房表没有订单时，也发一个特殊标志给厨师端，方便厨师端清空表格
        clearFood(F_head);
        strcpy(F_head.food, "chef_empty");
        Foodvec.push_back(F_head);
    }

    QByteArray a1;
    a1.resize(sizeof(FoodInfo) * Foodvec.size());
    memcpy(a1.data(), Foodvec.data(), sizeof(FoodInfo) * Foodvec.size());
    chef_socket->write(a1);
}

void mythread::chef_in()
{
    // 厨师端上菜时，第0个元素是chef_in标志，第1个元素才是具体上的菜
    if(Foodvec.size() <= 1)
    {
        return;
    }

    int n = QString(Foodvec[1].table).toInt();
    if(n < 0 || n >= 21)
    {
        return;
    }

    // 厨师端上菜后，管理端删除厨房表里对应的一条菜品记录
    QSqlQuery query(db);
    char sql[512];
    sprintf(sql, "delete from 厨房 where 桌号 = '%s' and 序号 = '%s' limit 1",
            Foodvec[1].table, Foodvec[1].number);
    query.exec(sql);

    // 如果点餐端已经断开，只删除数据库，不再往点餐端回发上菜通知
    if(order_socket[n] == NULL)
    {
        return;
    }

    // 再通知对应点餐端，让点餐端把这个菜移动到已上菜列表
    MenuVec.clear();
    clearMenu(M_head);
    strcpy(M_head.food, "chef_in");
    MenuVec.push_back(M_head);

    clearMenu(M_head);
    strcpy(M_head.number, Foodvec[1].number);
    strcpy(M_head.food, Foodvec[1].food);
    strcpy(M_head.price, Foodvec[1].price);
    strcpy(M_head.state, Foodvec[1].quatity);
    MenuVec.push_back(M_head);

    QByteArray a1;
    a1.resize(sizeof(MenuInfo) * MenuVec.size());
    memcpy(a1.data(), MenuVec.data(), sizeof(MenuInfo) * MenuVec.size());
    order_socket[n]->write(a1);
}

void mythread::order_food()
{
    // order表示提交订单，order_delete表示退餐，两个操作共用这一套保存逻辑
    bool isDelete = strcmp(Foodvec[0].food, "order_delete") == 0;

    auto sendOrderResult = [=](const char *result) {
        // 给点餐端回发操作结果，点餐端根据这个标志弹出对应提示
        MenuInfo reply;
        clearMenu(reply);
        strcpy(reply.food, result);

        QByteArray data;
        data.resize(sizeof(MenuInfo));
        memcpy(data.data(), &reply, sizeof(MenuInfo));
        tcpsocket->write(data);
    };

    if(!db.isOpen())
    {
        sendOrderResult(isDelete ? "delete_fail" : "order_fail");
        return;
    }

    int n = QString(Foodvec[0].table).toInt();
    // 桌号作为数组下标使用，所以这里限制在1到20之间
    if(n <= 0 || n >= 21)
    {
        sendOrderResult(isDelete ? "delete_fail" : "order_fail");
        return;
    }

    if(!isDelete && Foodvec.size() <= 1)
    {
        // 正常提交订单时，如果只有标志位没有菜品，就认为提交失败
        sendOrderResult("order_fail");
        return;
    }

    order_socket[n] = tcpsocket;
    foodvec[n] = Foodvec;
    // 保存点餐端套接字和该桌订单，后面厨师端上菜、收银端查询都要用到

    QSqlQuery query(db);
    char sql[512];
    QString Table = QString(Foodvec[0].table);
    QByteArray tableBytes = Table.toUtf8();

    // 每次提交都先删除该桌原来的厨房记录，再按当前未上菜列表重新插入
    sprintf(sql, "delete from 厨房 where 桌号 = '%s'", tableBytes.constData());
    bool success = query.exec(sql);
    bool hasFood = false;

    for(int j = 1; j < Foodvec.size(); j++)
    {
        // 点餐端把已经上菜的菜品桌号改为0，这类数据只保存在点餐端，不再写回厨房表
        if(strcmp(Foodvec[j].table, "0") != 0)
        {
            // 只有还没上菜的数据才重新写入厨房表，厨师端只处理这里面的数据
            hasFood = true;
            sprintf(sql, "insert into 厨房 values('%s','%s','%s','%s','%s','%s')",
                    Foodvec[j].table, Foodvec[j].number, Foodvec[j].food,
                    Foodvec[j].price, Foodvec[j].quatity, Foodvec[j].post);
            success = query.exec(sql) && success;
        }
    }

    if(isDelete)
    {
        sendOrderResult(success ? "delete_ok" : "delete_fail");
    }
    else
    {
        if(success && hasFood)
        {
            // 提交订单成功后，餐桌状态改成使用中，收银端刷新餐桌状态时会读到这个变化
            success = query.exec(QString::fromUtf8("update 餐桌 set 状态 = '使用中' where 桌号 = '%1'").arg(Table)) && success;
        }

        sendOrderResult(success && hasFood ? "order_ok" : "order_fail");
    }

    if(success && chef_socket != NULL)
    {
        // 如果厨师端已经连接，点餐端提交后立即推送最新订单，不需要厨师端手动刷新
        chef_sent();
    }
}

void mythread::Init()
{
    db = QSqlDatabase::database();
    if(!db.isOpen())
    {
        qDebug() << "数据库未打开";
        return;
    }

    MenuVec.clear();
    clearMenu(M_head);
    strcpy(M_head.food, "menu");
    MenuVec.push_back(M_head);

    // 菜品和饮品都读进同一个菜单容器，发给点餐端后由序号区分显示在哪个表格
    QSqlQuery query(db);
    query.exec(QString::fromUtf8("select *from 菜单"));
    while(query.next())
    {
        clearMenu(M_head);
        copyText(M_head.number, sizeof(M_head.number), query.value("序号").toString());
        copyText(M_head.food, sizeof(M_head.food), query.value("菜名").toString());
        copyText(M_head.price, sizeof(M_head.price), query.value("价格").toString());
        copyText(M_head.state, sizeof(M_head.state), query.value("状态").toString());
        MenuVec.push_back(M_head);
    }

    query.exec(QString::fromUtf8("select *from 饮品"));
    while(query.next())
    {
        clearMenu(M_head);
        copyText(M_head.number, sizeof(M_head.number), query.value("序号").toString());
        copyText(M_head.food, sizeof(M_head.food), query.value("饮品名").toString());
        copyText(M_head.price, sizeof(M_head.price), query.value("价格").toString());
        copyText(M_head.state, sizeof(M_head.state), query.value("状态").toString());
        MenuVec.push_back(M_head);
    }
}
