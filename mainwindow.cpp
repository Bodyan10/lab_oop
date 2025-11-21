#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdio>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->paintArea);

    printf("MainWindow created\n");


    toolGroup = new QActionGroup(this);
    toolGroup->addAction(ui->actionSelectTool);
    toolGroup->addAction(ui->actionRectangleTool);
    toolGroup->addAction(ui->actionEllipseTool);
    toolGroup->addAction(ui->actionTriangleTool);
    toolGroup->addAction(ui->actionLineTool);


    connect(ui->actionSelectTool, &QAction::triggered, this, &MainWindow::onSelectTool);
    connect(ui->actionRectangleTool, &QAction::triggered, this, &MainWindow::onRectangleTool);
    connect(ui->actionEllipseTool, &QAction::triggered, this, &MainWindow::onEllipseTool);
    connect(ui->actionTriangleTool, &QAction::triggered, this, &MainWindow::onTriangleTool);
    connect(ui->actionLineTool, &QAction::triggered, this, &MainWindow::onLineTool);

    connect(ui->actionChangeColor, &QAction::triggered, this, &MainWindow::onChangeColor);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteSelected);
    connect(ui->actionSelectAll, &QAction::triggered, this, &MainWindow::onSelectAll);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);

    ui->actionSelectTool->setChecked(true);
    ui->paintArea->setCurrentTool(Tool::Select);
}

MainWindow::~MainWindow()
{
    delete ui;
    printf("MainWindow destroyed\n");
}

void MainWindow::onSelectTool()
{
    ui->paintArea->setCurrentTool(Tool::Select);
    printf("Tool: Selection\n");
}

void MainWindow::onRectangleTool()
{
    ui->paintArea->setCurrentTool(Tool::Rectangle);
    printf("Tool: Rectangle\n");
}

void MainWindow::onEllipseTool()
{
    ui->paintArea->setCurrentTool(Tool::Ellipse);
    printf("Tool: Circle\n");
}

void MainWindow::onTriangleTool()
{
    ui->paintArea->setCurrentTool(Tool::Triangle);
    printf("Tool: Triangle\n");
}

void MainWindow::onLineTool()
{
    ui->paintArea->setCurrentTool(Tool::Line);
    printf("Tool: Line\n");
}

void MainWindow::onChangeColor()
{
    QColor color = QColorDialog::getColor(Qt::red, this, "Select Color");
    if (color.isValid()) {
        ui->paintArea->applyColorToSelected(color);
    }
}

void MainWindow::onDeleteSelected()
{
    ui->paintArea->deleteSelected();
    printf("Delete selected objects\n");
}

void MainWindow::onSelectAll()
{
    ui->paintArea->selectAll();
    printf("Select all objects\n");
}

void MainWindow::onExit()
{
    close();
}
