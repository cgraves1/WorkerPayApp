#-------------------------------------------------
#
# Project created by QtCreator 2016-03-28T10:04:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkerPayApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    employee.cpp \
    invoice.cpp \
    part.cpp \
    deduction.cpp \
    wage.cpp \
    employeerecord.cpp \
    filterswindow.cpp

HEADERS  += mainwindow.h \
    employee.h \
    invoice.h \
    part.h \
    deduction.h \
    wage.h \
    employeerecord.h \
    filterswindow.h

FORMS    += mainwindow.ui \
    filterswindow.ui

RESOURCES += \
    resources.qrc
