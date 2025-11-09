#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"
#include <QPoint>
#include <QRect>

class Circle : public Shape
{
public:
    Circle();
    Circle(const QPoint& center, int radius);
    ~Circle();
    Circle(const Circle& other);
    Circle& operator=(const Circle& other);
    Shape* clone() const override;

    void draw(QPainter* painter) const override;
    void move(const QPoint& delta) override;
    void resize(const QPoint& startPoint, const QPoint& endPoint) override;
    void createFromPoints(const QPoint& startPoint, const QPoint& endPoint) override;
    bool contains(const QPoint& point) const override;
    QRect boundingRect() const override;

    QPoint center() const;
    int radius() const;

private:
    QPoint center_;
    int radius_;
};

#endif // CIRCLE_H
