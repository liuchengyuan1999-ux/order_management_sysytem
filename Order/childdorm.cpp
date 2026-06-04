#include "childdorm.h"

#include "ui_childdorm.h"
#include <QApplication>
childdorm::childdorm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::childdorm)
{
    //获取父窗口变量
    //p = (Widget*) parent;  //获取父窗口指针

    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("点餐窗口"));
    //this->move(QPoint(100,100));
    this->setFixedSize(1500,800);//背景图尺寸
    this->setWindowIcon(QIcon(":/image/28.png"));//APP图标

    //设置为只读模式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 使表宽度自适应
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使表宽度自适应

    //居中显示
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    //add back picture
    QPalette pal = this->palette();//yunxu yong bi
   // pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/27.jpg")));//背景图
    setPalette(pal);

   //child = new Widget;
   //child->hide();

}

childdorm::~childdorm()
{
    delete ui;
}

void childdorm::on_pushButton_3_clicked()//加餐
{
    this->hide();
    emit Interface();
}


void childdorm::getData(QWidget *parent) //获取父窗口指针
{
    //获取父窗口变量
    Widget * p = (Widget*) parent;  //获取父窗口指针
    FoodVec = p->getVec();
    FoodVec_No = p->getVecNo();
    tcpsocket = p->getsocket();
}


void childdorm::getvec(vector<FoodInfo> FoodVec_no,vector<FoodInfo> FoodVec_in)  //打印订单
{
    QStringList headtext;
    headtext<<"序号"<<"餐名"<<"价格"<<"数量"<<"备注";
    ui->tableWidget->setColumnCount(headtext.count());//列表设置为和headtext相等
    ui->tableWidget->setHorizontalHeaderLabels(headtext);//插入表头
    ui->tableWidget->setRowCount(0);
    if(FoodVec != NULL && !FoodVec->empty())
    {
        ui->lineEdit->setText(FoodVec->at(0).table);
    }

    for (int j=1;j<(FoodVec_no).size();j++)
    {
        int rowcount = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(rowcount);

        QTableWidgetItem *column = new QTableWidgetItem(FoodVec_no[j].number);
        QTableWidgetItem *column1 = new QTableWidgetItem(FoodVec_no[j].food);
        QTableWidgetItem *column2 = new QTableWidgetItem(FoodVec_no[j].price);
        QTableWidgetItem *column3 = new QTableWidgetItem(FoodVec_no[j].quatity);
        QTableWidgetItem *column4 = new QTableWidgetItem(FoodVec_no[j].post);

        ui->tableWidget->setItem(rowcount,0,column);
        ui->tableWidget->setItem(rowcount,1,column1);
        ui->tableWidget->setItem(rowcount,2,column2);
        ui->tableWidget->setItem(rowcount,3,column3);
        ui->tableWidget->setItem(rowcount,4,column4);

     }

    QStringList headtext1;
    headtext1<<"序号"<<"餐名"<<"价格"<<"数量"<<"备注";
    ui->tableWidget_2->setColumnCount(headtext1.count());//列表设置为和headtext相等
    ui->tableWidget_2->setHorizontalHeaderLabels(headtext1);//插入表头
    ui->tableWidget_2->setRowCount(0);

    for (int j=0;j<(FoodVec_in).size();j++)
    {
        int rowcount = ui->tableWidget_2->rowCount();

        ui->tableWidget_2->insertRow(rowcount);

        QTableWidgetItem *column = new QTableWidgetItem(FoodVec_in[j].number);
        QTableWidgetItem *column1 = new QTableWidgetItem(FoodVec_in[j].food);
        QTableWidgetItem *column2 = new QTableWidgetItem(FoodVec_in[j].price);
        QTableWidgetItem *column3 = new QTableWidgetItem(FoodVec_in[j].quatity);
        QTableWidgetItem *column4 = new QTableWidgetItem(FoodVec_in[j].post);

        ui->tableWidget_2->setItem(rowcount,0,column);
        ui->tableWidget_2->setItem(rowcount,1,column1);
        ui->tableWidget_2->setItem(rowcount,2,column2);
        ui->tableWidget_2->setItem(rowcount,3,column3);
        ui->tableWidget_2->setItem(rowcount,4,column4);

     }
}

void childdorm::Delete_food()
{
    if(ui->tableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, QString::fromUtf8("\xE6\x8F\x90\xE7\xA4\xBA"), QString::fromUtf8("\xE6\x93\x8D\xE4\xBD\x9C\xE5\xA4\xB1\xE8\xB4\xA5\x21"));
        return;
    }

    int currow = ui->tableWidget->currentRow();
    if(currow == -1)
    {
        QMessageBox::warning(this, QString::fromUtf8("\xE6\x8F\x90\xE7\xA4\xBA"), QString::fromUtf8("\xE6\x93\x8D\xE4\xBD\x9C\xE5\xA4\xB1\xE8\xB4\xA5\x21"));
        return;
    }

    QString str = ui->tableWidget->item(currow,0)->text();
    QString str1 = ui->tableWidget->item(currow,1)->text();
    QString str2 = ui->tableWidget->item(currow,2)->text();
    QString str3 = ui->tableWidget->item(currow,3)->text();
    QString str4 = ui->tableWidget->item(currow,4)->text();

    QString S = ui->lineEdit->text();
    QByteArray ba = S.toUtf8();
    memcpy(F_head.table,ba.data(),ba.size()+1);
    ba = str.toUtf8();
    memcpy(F_head.number,ba.data(),ba.size()+1);
    ba = str1.toUtf8();
    memcpy(F_head.food,ba.data(),ba.size()+1);
    ba = str2.toUtf8();
    memcpy(F_head.price,ba.data(),ba.size()+1);
    ba = str3.toUtf8();
    memcpy(F_head.quatity,ba.data(),ba.size()+1);
    ba = str4.toUtf8();
    memcpy(F_head.post,ba.data(),ba.size()+1);

    ui->tableWidget->removeRow(currow);
    ui->tableWidget->clearSelection();
    ui->tableWidget->setCurrentCell(-1,-1);

    for(int i = 0; i < FoodVec_No->size(); i++)
    {
        if((strcmp(FoodVec_No->at(i).number,F_head.number) == 0) && (strcmp(FoodVec_No->at(i).post,F_head.post) == 0))
        {
            FoodVec_No->erase(FoodVec_No->begin()+i);
            break;
        }
    }

    for(int i = 0; i < FoodVec->size(); i++)
    {
        if((strcmp(FoodVec->at(i).number,F_head.number) == 0) && (strcmp(FoodVec->at(i).post,F_head.post) == 0))
        {
            FoodVec->erase(FoodVec->begin()+i);
            break;
        }
    }

    strcpy(FoodVec->at(0).food,"order_delete");
    QByteArray a1;
    a1.resize(sizeof(FoodInfo)*FoodVec->size());
    memcpy(a1.data(),FoodVec->data(),sizeof(FoodInfo)*FoodVec->size());
    tcpsocket->write(a1);
    strcpy(FoodVec->at(0).food,"order");
}

void childdorm::on_pushButton_clicked()//催餐
{
    QMessageBox::warning(this,"催餐","催餐成功!\n厨师正在努力中,请耐心等待!");
}

void childdorm::on_pushButton_2_clicked()//退餐
{
    emit Inter_delete();
}

void childdorm::on_pushButton_4_clicked()//退出
{
    QApplication::quit();
}
