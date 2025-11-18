#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QColorDialog>

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

private:
    Ui::MainWindow *ui;
    QActionGroup* toolGroup;
};

#endif // MAINWINDOW_H
