#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "myshape.h"

class Rectangle : public MyShape {
public:
    Rectangle();
    Rectangle(QPoint coordinates, QSize size, QColor color, bool selected);
    Rectangle(const Rectangle& other);
    Rectangle& operator=(const Rectangle& other);
    ~Rectangle();

    void draw(QPainter &painter) const override;
    char getTypeCode() const override;
};

#endif // RECTANGLE_H
