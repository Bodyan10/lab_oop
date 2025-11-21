#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

class Ellipse : public Shape {
public:
    Ellipse();
    Ellipse(QPoint coordinates, QSize size, QColor color, bool selected, std::string name);
    Ellipse(const Ellipse& other); // Конструктор копирования
    Ellipse& operator=(const Ellipse& other); // Оператор присваивания
    ~Ellipse(); // Деструктор

    void draw(QPainter &painter) const override;
};

#endif // ELLIPSE_H
