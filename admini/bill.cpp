#include "bill.h"
#include "ui_bill.h"
#include <QDesktopWidget>
#include <QTableWidgetItem>

Bill::Bill(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bill)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setFixedSize(1000,700);//背景图尺寸
    this->setWindowIcon(QIcon(":/image/44.png"));//APP图标

   //窗口居中显示
   QDesktopWidget* desktop = QApplication::desktop();
   move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

   //设置为只读模式
   ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/31.jpg")));//背景图
    setPalette(pal);

    db = QSqlDatabase::database(); //获取已有数据库连接
    if(!db.isOpen())
    {
       QMessageBox::warning(this,"提示","数据库连接失败");
    }
}

Bill::~Bill()
{
    delete ui;
}

void Bill::on_pushButton_clicked()//关闭
{
    close();
}

void Bill::on_pushButton_2_clicked()//查询
{
    QString Num = ui->lineEdit->text().trimmed();  //获得交易号

    if(Num.isEmpty())
    {
        QMessageBox::warning(this,"提示","请输入交易号");
        return;
    }

    QSqlQuery query(db);
    // 按交易号查询账单详情
    query.prepare(QStringLiteral("select `交易号`,`桌号`,`序号`,`菜名`,`价格`,`数量`,`备注`,`收银人` "
                                 "from `账单详情` where trim(`交易号`) = :num"));
    query.bindValue(":num", Num);
    bool ret = query.exec();//数据库执行语句
    if(!ret)
    {
        QSqlError error = query.lastError();
        QString str = QString("错误信息:%1,%2").arg(error.driverText()).arg(error.databaseText());
        QMessageBox::warning(this,"提示",str);
        return;
    }

    QStringList headtext;
    headtext<<"交易号"<<"桌号"<<"序号"<<"菜名"<<"价格"<<"数量"<<"备注"<<"收银人";
    ui->tableWidget->setColumnCount(headtext.count());//列表设置为和headtext相等
    ui->tableWidget->setHorizontalHeaderLabels(headtext);//插入表头
    ui->tableWidget->setRowCount(0);

    while (query.next())   //读取下一行信息
    {
        int rowcount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowcount);
        QTableWidgetItem *column = new QTableWidgetItem(query.value(0).toString());
        QTableWidgetItem *column1 = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *column2 = new QTableWidgetItem(query.value(2).toString());
        QTableWidgetItem *column3 = new QTableWidgetItem(query.value(3).toString());
        QTableWidgetItem *column4 = new QTableWidgetItem(query.value(4).toString());
        QTableWidgetItem *column5 = new QTableWidgetItem(query.value(5).toString());
        QTableWidgetItem *column6 = new QTableWidgetItem(query.value(6).toString());
        QTableWidgetItem *column7 = new QTableWidgetItem(query.value(7).toString());

        ui->tableWidget->setItem(rowcount,0,column);
        ui->tableWidget->setItem(rowcount,1,column1);
        ui->tableWidget->setItem(rowcount,2,column2);
        ui->tableWidget->setItem(rowcount,3,column3);
        ui->tableWidget->setItem(rowcount,4,column4);
        ui->tableWidget->setItem(rowcount,5,column5);
        ui->tableWidget->setItem(rowcount,6,column6);
        ui->tableWidget->setItem(rowcount,7,column7);
    }

    if(ui->tableWidget->rowCount() == 0)
    {
        QMessageBox::warning(this,"提示","未查询到该交易号的账单详情");
    }
}
