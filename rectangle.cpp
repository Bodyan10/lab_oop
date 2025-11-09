#include "rectangle.h"
#include <QPainter>
#include <cstdio>
#include <algorithm>

Rectangle::Rectangle()
    : topLeft_(QPoint(0, 0)), bottomRight_(QPoint(0, 0))
{
    name_ = "Прямоугольник";
}

Rectangle::Rectangle(const QPoint& topLeft, const QPoint& bottomRight)
    : topLeft_(topLeft), bottomRight_(bottomRight)
{
    name_ = "Прямоугольник";
    normalizePoints();
    printf("Создан прямоугольник: (%d, %d) - (%d, %d), размер: %dx%d\n",
           topLeft_.x(), topLeft_.y(), bottomRight_.x(), bottomRight_.y(),
           width(), height());
}

Rectangle::~Rectangle()
{
    printf("Прямоугольник удален\n");
}

Rectangle::Rectangle(const Rectangle& other)
    : topLeft_(other.topLeft_), bottomRight_(other.bottomRight_)
{
    name_ = "Прямоугольник";
    color_ = other.color_;
    isSelected_ = other.isSelected_;
}

Rectangle& Rectangle::operator=(const Rectangle& other)
{
    if (this != &other) {
        topLeft_ = other.topLeft_;
        bottomRight_ = other.bottomRight_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
    }
    return *this;
}

Shape* Rectangle::clone() const
{
    return new Rectangle(*this);
}

void Rectangle::draw(QPainter* painter) const
{
    if (!painter) return;

    // Не рисуем прямоугольник с нулевой площадью
    if (width() <= 0 || height() <= 0) {
        return;
    }

    if (isSelected_) {
        painter->setPen(QPen(Qt::red, 2));
    } else {
        painter->setPen(QPen(Qt::black, 1));
    }

    painter->setBrush(color_);
    painter->drawRect(topLeft_.x(), topLeft_.y(), width(), height());
}

void Rectangle::move(const QPoint& delta)
{
    topLeft_ += delta;
    bottomRight_ += delta;
}

void Rectangle::resize(const QPoint& startPoint, const QPoint& endPoint)
{
    // Для резиза просто устанавливаем новые точки
    topLeft_ = startPoint;
    bottomRight_ = endPoint;
    normalizePoints();
}

void Rectangle::createFromPoints(const QPoint& startPoint, const QPoint& endPoint)
{
    topLeft_ = startPoint;
    bottomRight_ = endPoint;
    normalizePoints();

    printf("Прямоугольник создан: (%d, %d) - (%d, %d), размер: %dx%d\n",
           topLeft_.x(), topLeft_.y(), bottomRight_.x(), bottomRight_.y(),
           width(), height());
}

bool Rectangle::contains(const QPoint& point) const
{
    if (width() <= 0 || height() <= 0) {
        return false;
    }
    return point.x() >= topLeft_.x() && point.x() <= bottomRight_.x() &&
           point.y() >= topLeft_.y() && point.y() <= bottomRight_.y();
}

QRect Rectangle::boundingRect() const
{
    return QRect(topLeft_, bottomRight_);
}

QPoint Rectangle::topLeft() const
{
    return topLeft_;
}

QPoint Rectangle::bottomRight() const
{
    return bottomRight_;
}

int Rectangle::width() const
{
    return bottomRight_.x() - topLeft_.x();
}

int Rectangle::height() const
{
    return bottomRight_.y() - topLeft_.y();
}

void Rectangle::normalizePoints()
{
    int left = std::min(topLeft_.x(), bottomRight_.x());
    int right = std::max(topLeft_.x(), bottomRight_.x());
    int top = std::min(topLeft_.y(), bottomRight_.y());
    int bottom = std::max(topLeft_.y(), bottomRight_.y());

    topLeft_ = QPoint(left, top);
    bottomRight_ = QPoint(right, bottom);
}
