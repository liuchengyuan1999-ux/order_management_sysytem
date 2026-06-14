#ifndef CHILDDORM_H
#define CHILDDORM_H

#include <QWidget>
#include<QDesktopWidget>
#include<QMessageBox>
#include<vector>
#include"widget.h"
#include "structural.h"
using namespace std;
namespace Ui {
class childdorm;
}

class Widget;

class childdorm : public QWidget
{
    Q_OBJECT
signals:
    void Interface();//显示父窗口
    void Inter_delete();//退餐
public:
    explicit childdorm(QWidget *parent = 0 );
    ~childdorm();
    void getvec(vector<FoodInfo> FoodVec_no,vector<FoodInfo> FoodVec_in);//打印订单
    void Delete_food();//退餐
    void getData(QWidget *parent);   //通过this把父窗口的指针传到子窗口当中
private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    
private:
    Ui::childdorm *ui;
    FoodInfo F_head;//订单结构体
    vector<FoodInfo> *FoodVec_No;//未上菜单容器
    vector<FoodInfo> *FoodVec;//已上菜单容器
    vector<FoodInfo> DeleteVec;//退餐容器
    QTcpSocket *tcpsocket;
    Widget * p;

};


#endif // CHILDDORM_H
