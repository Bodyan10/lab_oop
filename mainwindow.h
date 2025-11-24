#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QColorDialog>
#include <QFileDialog>
#include <myshapefactory.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSelectTool();
    void onRectangleTool();
    void onEllipseTool();
    void onTriangleTool();
    void onLineTool();
    void onChangeColor();
    void onDeleteSelected();
    void onSelectAll();
    void onExit();
    void onSave();
    void onLoad();
    void onGroup();
    void onUnGroup();

private:
    Ui::MainWindow *ui;
    QActionGroup* toolGroup;
    MyShapeFactory shapeFactory;
};

#endif // MAINWINDOW_H
