#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include <QPoint>
#include <QRect>

class Rectangle : public Shape
{
public:
    Rectangle();
    Rectangle(const QPoint& topLeft, const QPoint& bottomRight);
    ~Rectangle();
    Rectangle(const Rectangle& other);
    Rectangle& operator=(const Rectangle& other);
    Shape* clone() const override;

    void draw(QPainter* painter) const override;
    void move(const QPoint& delta) override;
    void resize(const QPoint& startPoint, const QPoint& endPoint) override;
    void createFromPoints(const QPoint& startPoint, const QPoint& endPoint) override;
    bool contains(const QPoint& point) const override;
    QRect boundingRect() const override;

    QPoint topLeft() const;
    QPoint bottomRight() const;
    int width() const;
    int height() const;

private:
    QPoint topLeft_;
    QPoint bottomRight_;

    void normalizePoints();
};

#endif // RECTANGLE_H
