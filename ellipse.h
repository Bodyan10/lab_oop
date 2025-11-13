#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

class Ellipse : public Shape {
public:
    Ellipse(QPoint coordinates = QPoint(), QSize size = QSize(), QColor color = Qt::red);
    Ellipse(const Ellipse& other); // Конструктор копирования
    Ellipse& operator=(const Ellipse& other); // Оператор присваивания
    ~Ellipse(); // Деструктор

    ShapeType type() const override { return ShapeType::Circle; }
    bool hasPointIn(QPoint point) const override;
    void draw(QPainter &painter) const override;
};

#endif // ELLIPSE_H
