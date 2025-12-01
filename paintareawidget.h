
#ifndef PAINTAREAWIDGET_H
#define PAINTAREAWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QResizeEvent>
#include "selection.h"
#include <Container.h>
#include <ShapeFactory.h>
#include <group.h>
#include <mytreewidget.h>

enum class Tool {
    Select,
    Rectangle,
    Ellipse,
    Triangle,
    Line
};

class PaintAreaWidget : public QWidget, public Observer
{
    Q_OBJECT

public:
    PaintAreaWidget(QWidget *parent = nullptr);
    ~PaintAreaWidget();

    void setCurrentTool(Tool tool);
    void setCurrentColor(const QColor& color);
    void applyColorToSelected(const QColor& color);
    void saveShapes(std::string filename);
    void loadShapes(std::string filename, ShapeFactory& factory);
    void groupSelected();
    void unGroupSelected();
    void onSubjectChanged() override;
    void onArrowTool();

    MyTreeWidget* myTree;

public slots:
    void deleteSelected();
    void selectAll();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Container<MyShape> shapesContainer;
    Selection selection;
    bool ctrlPressed;
    Tool currentTool;
    QColor currentColor;

    bool isCreatingShape;
    QPoint creationStartPoint;
    MyShape* tempShape;

    bool isResizing;
    QPoint resizeStartPoint;

    std::vector<MyShape*> findShapesAtPoint(const QPoint& point);
    void clearSelection();
    bool wouldCreateCycle(MyShape* from, MyShape* to);
};

#endif
