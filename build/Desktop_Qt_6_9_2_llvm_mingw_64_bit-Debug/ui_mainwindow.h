/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "paintareawidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionDelete;
    QAction *actionSelectAll;
    QAction *actionChangeColor;
    QAction *actionSelectTool;
    QAction *actionRectangleTool;
    QAction *actionCircleTool;
    QAction *actionTriangleTool;
    QAction *actionLineTool;
    QWidget *centralwidget;
    PaintAreaWidget *paintArea;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1034, 812);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName("actionDelete");
        actionSelectAll = new QAction(MainWindow);
        actionSelectAll->setObjectName("actionSelectAll");
        actionChangeColor = new QAction(MainWindow);
        actionChangeColor->setObjectName("actionChangeColor");
        actionSelectTool = new QAction(MainWindow);
        actionSelectTool->setObjectName("actionSelectTool");
        actionSelectTool->setCheckable(true);
        actionSelectTool->setChecked(true);
        actionRectangleTool = new QAction(MainWindow);
        actionRectangleTool->setObjectName("actionRectangleTool");
        actionRectangleTool->setCheckable(true);
        actionCircleTool = new QAction(MainWindow);
        actionCircleTool->setObjectName("actionCircleTool");
        actionCircleTool->setCheckable(true);
        actionTriangleTool = new QAction(MainWindow);
        actionTriangleTool->setObjectName("actionTriangleTool");
        actionTriangleTool->setCheckable(true);
        actionLineTool = new QAction(MainWindow);
        actionLineTool->setObjectName("actionLineTool");
        actionLineTool->setCheckable(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        paintArea = new PaintAreaWidget(centralwidget);
        paintArea->setObjectName("paintArea");
        paintArea->setGeometry(QRect(20, 10, 1001, 701));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1034, 25));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menu);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menu);
        menu_3->setObjectName("menu_3");
        menu_4 = new QMenu(menu);
        menu_4->setObjectName("menu_4");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menu->menuAction());
        menu->addAction(menu_2->menuAction());
        menu->addAction(menu_3->menuAction());
        menu->addAction(menu_4->menuAction());
        menu->addAction(actionExit);
        menu_2->addAction(actionDelete);
        menu_2->addAction(actionSelectAll);
        menu_3->addAction(actionChangeColor);
        menu_4->addAction(actionSelectTool);
        menu_4->addAction(actionRectangleTool);
        menu_4->addAction(actionCircleTool);
        menu_4->addAction(actionTriangleTool);
        menu_4->addAction(actionLineTool);
        toolBar->addAction(actionSelectTool);
        toolBar->addAction(actionRectangleTool);
        toolBar->addAction(actionCircleTool);
        toolBar->addAction(actionTriangleTool);
        toolBar->addAction(actionLineTool);
        toolBar->addSeparator();
        toolBar->addAction(actionChangeColor);
        toolBar->addAction(actionDelete);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDelete->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
#if QT_CONFIG(shortcut)
        actionDelete->setShortcut(QCoreApplication::translate("MainWindow", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelectAll->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\264\320\265\320\273\320\270\321\202\321\214 \320\262\321\201\321\221", nullptr));
#if QT_CONFIG(shortcut)
        actionSelectAll->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        actionChangeColor->setText(QCoreApplication::translate("MainWindow", "\320\246\320\262\320\265\321\202", nullptr));
#if QT_CONFIG(shortcut)
        actionChangeColor->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelectTool->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
#if QT_CONFIG(shortcut)
        actionSelectTool->setShortcut(QCoreApplication::translate("MainWindow", "S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRectangleTool->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\321\217\320\274\320\276\321\203\320\263\320\276\320\273\321\214\320\275\320\270\320\272", nullptr));
#if QT_CONFIG(shortcut)
        actionRectangleTool->setShortcut(QCoreApplication::translate("MainWindow", "R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCircleTool->setText(QCoreApplication::translate("MainWindow", "\320\232\321\200\321\203\320\263", nullptr));
#if QT_CONFIG(shortcut)
        actionCircleTool->setShortcut(QCoreApplication::translate("MainWindow", "C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionTriangleTool->setText(QCoreApplication::translate("MainWindow", "\320\242\321\200\320\265\321\203\320\263\320\276\320\273\321\214\320\275\320\270\320\272", nullptr));
#if QT_CONFIG(shortcut)
        actionTriangleTool->setShortcut(QCoreApplication::translate("MainWindow", "T", nullptr));
#endif // QT_CONFIG(shortcut)
        actionLineTool->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\321\200\320\265\320\267\320\276\320\272", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "&\320\244\320\260\320\271\320\273", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "&\320\237\321\200\320\260\320\262\320\272\320\260", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "&\320\222\320\270\320\264", nullptr));
        menu_4->setTitle(QCoreApplication::translate("MainWindow", "&\320\230\320\275\321\201\321\202\321\200\321\203\320\274\320\265\320\275\321\202\321\213", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
