#ifndef SELECTION_H
#define SELECTION_H

#include "myshape.h"
#include <QPainter>
#include <QRect>
#include <Container.h>

class Selection : public Container<MyShape> {
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
    void moveSelections(int diffX, int diffY, const QRect&);
    bool resizeSelections(int diffX, int diffY, const QRect& widget_rect);
    void clear() override;
    void removeElement(MyShape* element) override;
    void removeSelected() override;
    void removeAt(int index) override;
    void addObject(MyShape* new_object) override;
    void updateShapesRelativeFrame();

private:
    enum class ResizePositions {
        TopLeft, Top, TopRight,
        Left, Right,
        BottomLeft, Bottom, BottomRight,
        None
    };
    ResizePositions currentResizePos_ = ResizePositions::None;
    std::vector<std::pair<QPointF, QSizeF>> shapesRelativeFrame;
    QRect frameOfSelected_;
};

#endif // SELECTION_H
