QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arrow.cpp \
    ellipse.cpp \
    group.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
    myshape.cpp \
    myshapefactory.cpp \
    mytreewidget.cpp \
    observablemixin.cpp \
    paintareawidget.cpp \
    rectangle.cpp \
    selection.cpp \
    triangle.cpp

HEADERS += \
    Container.h \
    Observer.h \
    ShapeFactory.h \
    arrow.h \
    ellipse.h \
    group.h \
    line.h \
    mainwindow.h \
    myshape.h \
    myshapefactory.h \
    mytreewidget.h \
    observablemixin.h \
    paintareawidget.h \
    rectangle.h \
    selection.h \
    triangle.h

FORMS += \
    mainwindow.ui

CODECFORSRC = UTF-8
QMAKE_CXXFLAGS += -finput-charset=UTF-8
QMAKE_CXXFLAGS += -fexec-charset=UTF-8

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
