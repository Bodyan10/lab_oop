#ifndef SELECTION_H
#define SELECTION_H

#include "container.h"
#include "shape.h"
#include <QPainter>
#include <QRect>
#include <unordered_map>

class Selection : public Container<Shape*> {
public:
    enum class MousePosState {
        TopLeft, TopRight, BottomLeft, BottomRight,
        Top, Bottom, Left, Right,
        None
    };

    Selection();

    void draw(QPainter& painter) const;
    QRect getArea() const;
    bool hasObjectInPoint(QPoint point) const;
    MousePosState checkMousePos(QPoint pos);
    void moveSelections(int diffX, int diffY);
    bool resizeSelections(int diffX, int diffY);

    void updateRelativeInfo();

private:
    enum class ResizePositions {
        TopLeft, Top, TopRight,
        Left, Right,
        BottomLeft, Bottom, BottomRight,
        None
    };

    QRect selectionBounds_;
    ResizePositions currentResizePos_ = ResizePositions::None;
    std::unordered_map<Shape*, std::pair<QPointF, QSizeF>> relativeInfo_;
};

#endif // SELECTION_H
