#-------------------------------------------------
#
# Project created by QtCreator 2015-08-13T15:48:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Accounting
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    authorization.cpp \
    accounting.cpp \
    form.cpp \
    accountsform.cpp

HEADERS  += mainwindow.h \
    authorization.h \
    accounting.h \
    form.h \
    accountsform.h

FORMS    += mainwindow.ui \
    authorization.ui \
    form.ui \
    accountsform.ui
