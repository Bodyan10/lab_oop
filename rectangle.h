#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape {
public:
    Rectangle();
    Rectangle(QPoint coordinates, QSize size, QColor color, bool selected, std::string name);
    Rectangle(const Rectangle& other);
    Rectangle& operator=(const Rectangle& other);
    ~Rectangle();

    void draw(QPainter &painter) const override;
};

#endif // RECTANGLE_H
