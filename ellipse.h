#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "myshape.h"

class Ellipse : public MyShape {
public:
    Ellipse();
    Ellipse(QPoint coordinates, QSize size, QColor color, bool selected);
    Ellipse(const Ellipse& other); // Конструктор копирования
    Ellipse& operator=(const Ellipse& other); // Оператор присваивания
    ~Ellipse(); // Деструктор

    void draw(QPainter &painter) const override;
    char getTypeCode() const override;
};

#endif // ELLIPSE_H
