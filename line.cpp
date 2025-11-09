#include "line.h"
#include <QPainter>
#include <cstdio>
#include <algorithm>

Line::Line(const QPoint& start, const QPoint& end)
    : start_(start), end_(end)
{
    name_ = "Отрезок";
    printf("Создан отрезок: (%d,%d) - (%d,%d)\n",
           start_.x(), start_.y(), end_.x(), end_.y());
}

Line::~Line()
{
    printf("Отрезок удален\n");
}

Line::Line(const Line& other)
    : start_(other.start_), end_(other.end_)
{
    name_ = "Отрезок";
    color_ = other.color_;
    isSelected_ = other.isSelected_;
}

Line& Line::operator=(const Line& other)
{
    if (this != &other) {
        start_ = other.start_;
        end_ = other.end_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
    }
    return *this;
}

Shape* Line::clone() const
{
    return new Line(*this);
}

void Line::draw(QPainter* painter) const
{
    if (!painter) return;

    if (isSelected_) {
        painter->setPen(QPen(Qt::red, 3));
    } else {
        painter->setPen(QPen(color_, 2));
    }

    painter->drawLine(start_, end_);
}

void Line::move(const QPoint& delta)
{
    start_ += delta;
    end_ += delta;
}

void Line::resize(const QPoint& anchorPoint, const QPoint& newPoint)
{
    // При создании линии anchorPoint - это начальная точка
    // newPoint - это конечная точка
    end_ = newPoint;
    printf("Линия изменена: (%d,%d) - (%d,%d)\n",
           start_.x(), start_.y(), end_.x(), end_.y());
}

bool Line::contains(const QPoint& point) const
{
    QRect bounds = boundingRect().adjusted(-3, -3, 3, 3);
    return bounds.contains(point);
}

QRect Line::boundingRect() const
{
    int minX = std::min(start_.x(), end_.x());
    int maxX = std::max(start_.x(), end_.x());
    int minY = std::min(start_.y(), end_.y());
    int maxY = std::max(start_.y(), end_.y());

    return QRect(minX, minY, maxX - minX, maxY - minY);
}

QPoint Line::startPoint() const
{
    return start_;
}

QPoint Line::endPoint() const
{
    return end_;
}

void Line::createFromPoints(const QPoint& startPoint, const QPoint& endPoint)
{
    start_ = startPoint;
    end_ = endPoint;
}
