#ifndef STRUCTURAL_H
#define STRUCTURAL_H

typedef struct Food
{
    char table[5]; //桌号
    char number[10];//序号
    char food[100]; //餐名
    char price[10];//价格
    char quatity[10];//数量
    char post[200];//备注
}FoodInfo;

#endif // STRUCTURAL_H
