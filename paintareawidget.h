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

enum class Tool {
    Select,
    Rectangle,
    Ellipse,
    Triangle,
    Line
};

class PaintAreaWidget : public QWidget
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
    Container<Shape> shapesContainer;
    Selection selection;
    bool ctrlPressed;
    Tool currentTool;
    QColor currentColor;

    bool isCreatingShape;
    QPoint creationStartPoint;
    Shape* tempShape;

    bool isResizing;
    QPoint resizeStartPoint;

    std::vector<Shape*> findShapesAtPoint(const QPoint& point);
    void clearSelection();
};

#endif
