#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "myshape.h"

class Triangle : public MyShape {
public:
    Triangle();
    Triangle(QPoint coordinates, QSize size, QColor color, bool selected);
    Triangle(const Triangle& other);
    Triangle& operator=(const Triangle& other);
    ~Triangle();

    void draw(QPainter &painter) const override;
    char getTypeCode() const override;
};

#endif // TRIANGLE_H
