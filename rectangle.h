#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape {
public:
    Rectangle(QPoint coordinates = QPoint(), QSize size = QSize(), QColor color = Qt::blue);
    ShapeType type() const override { return ShapeType::Rectangle; }
    bool hasPointIn(QPoint point) const override;
    void draw(QPainter &painter) const override;
};

#endif // RECTANGLE_H
