#include "triangle.h"
#include <QPainter>
#include <cstdio>
#include <algorithm>

Triangle::Triangle(const QPoint& p1, const QPoint& p2, const QPoint& p3)
    : p1_(p1), p2_(p2), p3_(p3)
{
    name_ = "Треугольник";
    updateBoundingRect();
    printf("Создан треугольник: (%d,%d), (%d,%d), (%d,%d)\n",
           p1_.x(), p1_.y(), p2_.x(), p2_.y(), p3_.x(), p3_.y());
}

Triangle::~Triangle()
{
    printf("Треугольник удален\n");
}

Triangle::Triangle(const Triangle& other)
    : p1_(other.p1_), p2_(other.p2_), p3_(other.p3_), boundingRect_(other.boundingRect_)
{
    name_ = "Треугольник";
    color_ = other.color_;
    isSelected_ = other.isSelected_;
}

Triangle& Triangle::operator=(const Triangle& other)
{
    if (this != &other) {
        p1_ = other.p1_;
        p2_ = other.p2_;
        p3_ = other.p3_;
        boundingRect_ = other.boundingRect_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
    }
    return *this;
}

Shape* Triangle::clone() const
{
    return new Triangle(*this);
}

void Triangle::draw(QPainter* painter) const
{
    if (!painter) return;

    if (isSelected_) {
        painter->setPen(QPen(Qt::red, 2));
    } else {
        painter->setPen(QPen(Qt::black, 1));
    }

    painter->setBrush(color_);

    QPolygon polygon;
    polygon << p1_ << p2_ << p3_;
    painter->drawPolygon(polygon);
}

void Triangle::move(const QPoint& delta)
{
    p1_ += delta;
    p2_ += delta;
    p3_ += delta;
    updateBoundingRect();
}

void Triangle::resize(const QPoint& anchorPoint, const QPoint& newPoint)
{
    int dist1 = (p1_ - anchorPoint).manhattanLength();
    int dist2 = (p2_ - anchorPoint).manhattanLength();
    int dist3 = (p3_ - anchorPoint).manhattanLength();

    if (dist1 <= dist2 && dist1 <= dist3) {
        p1_ = newPoint;
    } else if (dist2 <= dist1 && dist2 <= dist3) {
        p2_ = newPoint;
    } else {
        p3_ = newPoint;
    }

    updateBoundingRect();
    printf("Треугольник изменен\n");
}

bool Triangle::contains(const QPoint& point) const
{
    QPolygon polygon;
    polygon << p1_ << p2_ << p3_;
    return polygon.containsPoint(point, Qt::OddEvenFill);
}

QRect Triangle::boundingRect() const
{
    return boundingRect_;
}

std::vector<QPoint> Triangle::points() const
{
    return {p1_, p2_, p3_};
}

void Triangle::updateBoundingRect()
{
    int minX = std::min({p1_.x(), p2_.x(), p3_.x()});
    int maxX = std::max({p1_.x(), p2_.x(), p3_.x()});
    int minY = std::min({p1_.y(), p2_.y(), p3_.y()});
    int maxY = std::max({p1_.y(), p2_.y(), p3_.y()});

    boundingRect_ = QRect(minX, minY, maxX - minX, maxY - minY);
}

void Triangle::createFromPoints(const QPoint& startPoint, const QPoint& endPoint)
{
    // Создаем треугольник на основе прямоугольной области
    int width = endPoint.x() - startPoint.x();
    int height = endPoint.y() - startPoint.y();

    p1_ = startPoint;
    p2_ = QPoint(startPoint.x() + width, startPoint.y());
    p3_ = QPoint(startPoint.x(), startPoint.y() + height);

    updateBoundingRect();
}
