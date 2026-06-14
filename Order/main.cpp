#include "widget.h"
#include <QApplication>
#include<QSqlQuery>
#include <vector>
#include<QString>
#include<QTcpSocket>
#include "childdorm.h"
#include "structural.h"
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
     w.show();
    return a.exec();
}
