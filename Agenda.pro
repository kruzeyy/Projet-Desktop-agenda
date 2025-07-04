QT += core gui widgets
CONFIG += c++11
TARGET = Agenda
TEMPLATE = app
SOURCES += \
    main.cpp \
    mainwindow.cpp
HEADERS += \
    mainwindow.h
FORMS += mainwindow.ui
RESOURCES += resources.qrc

# For storing events in a JSON file

