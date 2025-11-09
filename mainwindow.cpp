#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <cstdio>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    printf("MainWindow created\n");

    // Create tool group (mutually exclusive buttons)
    toolGroup = new QActionGroup(this);
    toolGroup->addAction(ui->actionSelectTool);
    toolGroup->addAction(ui->actionRectangleTool);
    toolGroup->addAction(ui->actionCircleTool);
    toolGroup->addAction(ui->actionTriangleTool);
    toolGroup->addAction(ui->actionLineTool);

    // Connect tool signals
    connect(ui->actionSelectTool, &QAction::triggered, this, &MainWindow::onSelectTool);
    connect(ui->actionRectangleTool, &QAction::triggered, this, &MainWindow::onRectangleTool);
    connect(ui->actionCircleTool, &QAction::triggered, this, &MainWindow::onCircleTool);
    connect(ui->actionTriangleTool, &QAction::triggered, this, &MainWindow::onTriangleTool);
    connect(ui->actionLineTool, &QAction::triggered, this, &MainWindow::onLineTool);

    // Connect other actions
    connect(ui->actionChangeColor, &QAction::triggered, this, &MainWindow::onChangeColor);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteSelected);
    connect(ui->actionSelectAll, &QAction::triggered, this, &MainWindow::onSelectAll);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);

    // Set initial tool
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

void MainWindow::onCircleTool()
{
    ui->paintArea->setCurrentTool(Tool::Circle);
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
    printf("Color dialog opened\n");
    QColor color = QColorDialog::getColor(Qt::blue, this, "Select Color");
    if (color.isValid()) {
        ui->paintArea->applyColorToSelected(color);
        printf("Color changed via dialog to (%d, %d, %d)\n", color.red(), color.green(), color.blue());
    } else {
        printf("Color selection cancelled\n");
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
