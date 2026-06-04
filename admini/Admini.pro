#-------------------------------------------------
#
# Project created by QtCreator 2024-05-21T18:05:44
#
#-------------------------------------------------

QT       += core gui network sql

CONFIG += utf8
QMAKE_CXXFLAGS += -finput-charset=utf-8 -fexec-charset=utf-8

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Admini
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# 字符编码配置

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    childdorm.cpp \
    add_users.cpp \
    change_pw.cpp \
    change_per.cpp \
    add_food.cpp \
    change_price.cpp \
    change_status.cpp \
    add_drinks.cpp \
    modify_drinks.cpp \
    modify_stutus.cpp \
    bill.cpp \
    mythread.cpp

HEADERS += \
        widget.h \
    childdorm.h \
    add_users.h \
    change_pw.h \
    change_per.h \
    add_food.h \
    change_price.h \
    change_status.h \
    add_drinks.h \
    modify_drinks.h \
    modify_stutus.h \
    bill.h \
    structural.h \
    mythread.h

FORMS += \
        widget.ui \
    childdorm.ui \
    add_users.ui \
    change_pw.ui \
    change_per.ui \
    add_food.ui \
    change_price.ui \
    change_status.ui \
    add_drinks.ui \
    modify_drinks.ui \
    modify_stutus.ui \
    bill.ui

RESOURCES += \
    image.qrc

QMAKE_LFLAGS += -no-pie

LIBS+=-lpthread
