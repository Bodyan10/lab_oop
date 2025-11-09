#include "shape.h"
#include <cstdio>

Shape::Shape()
{
    printf("Базовая фигура создана\n");
}

Shape::~Shape()
{
    printf("Фигура '%s' удалена\n", name_.c_str());
}

void Shape::setColor(const QColor& color)
{
    color_ = color;
}

bool Shape::isOutOfBounds(const QRect& workspaceBounds) const
{
    QRect shapeBounds = boundingRect();
    return !workspaceBounds.contains(shapeBounds);
}

void Shape::setSelected(bool selected)
{
    isSelected_ = selected;
}

QColor Shape::color() const
{
    return color_;
}

bool Shape::isSelected() const
{
    return isSelected_;
}

std::string Shape::name() const
{
    return name_;
}
