#ifndef LINE_H
#define LINE_H

#include "shape.h"
#include <QPoint>
#include <QRect>

class Line : public Shape
{
public:
    Line(const QPoint& start = QPoint(), const QPoint& end = QPoint());
    ~Line();
    Line(const Line& other);
    Line& operator=(const Line& other);
    Shape* clone() const override;

    void draw(QPainter* painter) const override;
    void createFromPoints(const QPoint& startPoint, const QPoint& endPoint) override;
    void move(const QPoint& delta) override;
    void resize(const QPoint& anchorPoint, const QPoint& newPoint) override;
    bool contains(const QPoint& point) const override;
    QRect boundingRect() const override;

    QPoint startPoint() const;
    QPoint endPoint() const;

private:
    QPoint start_;
    QPoint end_;
};

#endif // LINE_H
