#ifndef SELECTION_H
#define SELECTION_H

#include "shape.h"
#include <QPainter>
#include <QRect>

class Selection {
public:
    enum class MousePosState {
        TopLeft, TopRight, BottomLeft, BottomRight,
        Top, Bottom, Left, Right, None
    };

    Selection();
    ~Selection();

    void draw(QPainter& painter) const;
    QRect getArea() const;
    bool hasObjectInPoint(QPoint point) const;
    MousePosState checkMousePos(QPoint pos);
    void moveSelections(int diffX, int diffY);
    bool resizeSelections(int diffX, int diffY);
    bool checkBorder(const QRect& widget_border);

    // Методы для работы с контейнером
    void addObject(Shape* shape);
    void removeAt(int index);
    void removeElement(Shape* elenment);
    void clear();
    int getCount() const;
    Shape* getObject(int index);
    bool hasElement(Shape* el) const;

private:
    std::vector<Shape*> selectedShapes;
    enum class ResizePositions {
        TopLeft, Top, TopRight,
        Left, Right,
        BottomLeft, Bottom, BottomRight,
        None
    };
    ResizePositions currentResizePos_ = ResizePositions::None;
    void scaleToNewBounds(const QRect& newBounds);
};

#endif // SELECTION_H
