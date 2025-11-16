#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"

class Triangle : public Shape {
public:
    Triangle(QPoint coordinates, QSize size, QColor color, bool selected, std::string name);
    Triangle(const Triangle& other);
    Triangle& operator=(const Triangle& other);
    ~Triangle();

    void draw(QPainter &painter) const override;
};

#endif // TRIANGLE_H
