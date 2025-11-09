#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <cstdio>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    printf("Главное окно создано\n");

    // Создаем группу для инструментов (взаимоисключающие кнопки)
    toolGroup = new QActionGroup(this);
    toolGroup->addAction(ui->actionSelectTool);
    toolGroup->addAction(ui->actionRectangleTool);
    toolGroup->addAction(ui->actionCircleTool);
    toolGroup->addAction(ui->actionTriangleTool);
    toolGroup->addAction(ui->actionLineTool);

    // Подключаем сигналы инструментов
    connect(ui->actionSelectTool, &QAction::triggered, this, &MainWindow::onSelectTool);
    connect(ui->actionRectangleTool, &QAction::triggered, this, &MainWindow::onRectangleTool);
    connect(ui->actionCircleTool, &QAction::triggered, this, &MainWindow::onCircleTool);
    connect(ui->actionTriangleTool, &QAction::triggered, this, &MainWindow::onTriangleTool);
    connect(ui->actionLineTool, &QAction::triggered, this, &MainWindow::onLineTool);

    // Подключаем остальные действия
    connect(ui->actionChangeColor, &QAction::triggered, this, &MainWindow::onChangeColor);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::onDeleteSelected);
    connect(ui->actionSelectAll, &QAction::triggered, this, &MainWindow::onSelectAll);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);

    // Устанавливаем начальный инструмент
    ui->actionSelectTool->setChecked(true);
    ui->paintArea->setCurrentTool(Tool::Select);
}

MainWindow::~MainWindow()
{
    delete ui;
    printf("Главное окно удалено\n");
}

void MainWindow::onSelectTool()
{
    ui->paintArea->setCurrentTool(Tool::Select);
    printf("Инструмент: Выделение\n");
}

void MainWindow::onRectangleTool()
{
    ui->paintArea->setCurrentTool(Tool::Rectangle);
    printf("Инструмент: Прямоугольник\n");
}

void MainWindow::onCircleTool()
{
    ui->paintArea->setCurrentTool(Tool::Circle);
    printf("Инструмент: Круг\n");
}

void MainWindow::onTriangleTool()
{
    ui->paintArea->setCurrentTool(Tool::Triangle);
    printf("Инструмент: Треугольник\n");
}

void MainWindow::onLineTool()
{
    ui->paintArea->setCurrentTool(Tool::Line);
    printf("Инструмент: Отрезок\n");
}

void MainWindow::onChangeColor()
{
    QColor color = QColorDialog::getColor(Qt::blue, this, "Выберите цвет");
    if (color.isValid()) {
        ui->paintArea->setCurrentColor(color);
        printf("Цвет изменен через диалог\n");
    }
}

void MainWindow::onDeleteSelected()
{
    ui->paintArea->deleteSelected();
    printf("Удаление выделенных объектов\n");
}

void MainWindow::onSelectAll()
{
    // Логика выделения всех уже в PaintAreaWidget
    printf("Выделить все\n");
}

void MainWindow::onExit()
{
    close();
}
