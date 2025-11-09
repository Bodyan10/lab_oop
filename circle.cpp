#include "circle.h"
#include <QPainter>
#include <cstdio>
#include <cmath>

Circle::Circle()
    : center_(QPoint(0, 0)), radius_(0)
{
    name_ = "Круг";
}

Circle::Circle(const QPoint& center, int radius)
    : center_(center), radius_(radius)
{
    name_ = "Круг";
    printf("Создан круг: центр(%d, %d), радиус: %d\n",
           center_.x(), center_.y(), radius_);
}

Circle::~Circle()
{
    printf("Круг удален\n");
}

Circle::Circle(const Circle& other)
    : center_(other.center_), radius_(other.radius_)
{
    name_ = "Круг";
    color_ = other.color_;
    isSelected_ = other.isSelected_;
}

Circle& Circle::operator=(const Circle& other)
{
    if (this != &other) {
        center_ = other.center_;
        radius_ = other.radius_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
    }
    return *this;
}

Shape* Circle::clone() const
{
    return new Circle(*this);
}

void Circle::draw(QPainter* painter) const
{
    if (!painter) return;

    if (isSelected_) {
        painter->setPen(QPen(Qt::red, 2));
    } else {
        painter->setPen(QPen(Qt::black, 1));
    }

    painter->setBrush(color_);
    painter->drawEllipse(center_, radius_, radius_);
}

void Circle::move(const QPoint& delta)
{
    center_ += delta;
}

void Circle::resize(const QPoint& anchorPoint, const QPoint& newPoint)
{
    // anchorPoint всегда центр круга
    // newPoint определяет новый радиус
    int newRadius = static_cast<int>(std::sqrt(
        std::pow(newPoint.x() - center_.x(), 2) +
        std::pow(newPoint.y() - center_.y(), 2)
        ));

    // Минимальный радиус 1 пиксель
    radius_ = std::max(1, newRadius);
    printf("Круг изменен: радиус: %d\n", radius_);
}

bool Circle::contains(const QPoint& point) const
{
    int dx = point.x() - center_.x();
    int dy = point.y() - center_.y();
    int distanceSquared = dx * dx + dy * dy;

    return distanceSquared <= radius_ * radius_;
}

QRect Circle::boundingRect() const
{
    return QRect(center_.x() - radius_, center_.y() - radius_,
                 radius_ * 2, radius_ * 2);
}

QPoint Circle::center() const
{
    return center_;
}

int Circle::radius() const
{
    return radius_;
}

void Circle::createFromPoints(const QPoint& startPoint, const QPoint& endPoint)
{
    center_ = startPoint;
    int dx = endPoint.x() - startPoint.x();
    int dy = endPoint.y() - startPoint.y();
    radius_ = static_cast<int>(std::sqrt(dx * dx + dy * dy));

    printf("Круг создан: центр(%d, %d), радиус: %d\n",
           center_.x(), center_.y(), radius_);
}
