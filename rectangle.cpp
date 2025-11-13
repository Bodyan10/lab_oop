#include "rectangle.h"
#include <QPainter>
#include <cstdio>

Rectangle::Rectangle(QPoint coordinates, QSize size, QColor color)
    : Shape(coordinates, size, color) {
    name_ = "Rectangle";
    printf("Rectangle created at (%d, %d) size %dx%d\n",
           coordinates.x(), coordinates.y(), size.width(), size.height());
}

Rectangle::Rectangle(const Rectangle& other)
    : Shape(other.pos_, other.size_, other.color_) {
    name_ = "Rectangle";
    isSelected_ = other.isSelected_;
    printf("Rectangle copied\n");
}

Rectangle& Rectangle::operator=(const Rectangle& other) {
    if (this != &other) {
        pos_ = other.pos_;
        size_ = other.size_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
    }
    printf("Rectangle assigned\n");
    return *this;
}

Rectangle::~Rectangle() {
    printf("Rectangle destroyed\n");
}

bool Rectangle::hasPointIn(QPoint point) const {
    QRect r(pos_, size_);
    bool contains = r.contains(point);
    printf("Rectangle contains point (%d, %d): %s\n", point.x(), point.y(), contains ? "true" : "false");
    return contains;
}

void Rectangle::draw(QPainter& painter) const {
    QRect r(pos_, size_);

    if (isSelected_) {
        painter.setPen(QPen(Qt::red, 2));
    } else {
        painter.setPen(QPen(Qt::black, 1));
    }

    painter.setBrush(color_);
    painter.drawRect(r);
    printf("Rectangle drawn at (%d, %d) size %dx%d, color (%d,%d,%d)\n",
           pos_.x(), pos_.y(), size_.width(), size_.height(),
           color_.red(), color_.green(), color_.blue());
}
