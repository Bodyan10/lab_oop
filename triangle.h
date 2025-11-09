#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include <QPoint>
#include <QRect>
#include <vector>

class Triangle : public Shape
{
public:
    Triangle(const QPoint& p1 = QPoint(), const QPoint& p2 = QPoint(), const QPoint& p3 = QPoint());
    ~Triangle();
    Triangle(const Triangle& other);
    Triangle& operator=(const Triangle& other);
    Shape* clone() const override;

    void draw(QPainter* painter) const override;
    void createFromPoints(const QPoint& startPoint, const QPoint& endPoint) override;
    void move(const QPoint& delta) override;
    void resize(const QPoint& anchorPoint, const QPoint& newPoint) override;
    bool contains(const QPoint& point) const override;
    QRect boundingRect() const override;

    std::vector<QPoint> points() const;

private:
    QPoint p1_, p2_, p3_;

    void updateBoundingRect();
    QRect boundingRect_;
};

#endif // TRIANGLE_H
