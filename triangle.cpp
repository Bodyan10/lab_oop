#include "triangle.h"
#include <QPainter>
#include <QPolygon>
#include <cstdio>

Triangle::Triangle(QPoint coordinates, QSize size, QColor color)
    : Shape(coordinates, size, color) {
    name_ = "Triangle";
    printf("Triangle created at (%d, %d) size %dx%d\n",
           coordinates.x(), coordinates.y(), size.width(), size.height());
}

Triangle::Triangle(const Triangle& other)
    : Shape(other.pos_, other.size_, other.color_) {
    name_ = "Triangle";
    isSelected_ = other.isSelected_;
    printf("Triangle copied\n");
}

Triangle& Triangle::operator=(const Triangle& other) {
    if (this != &other) {
        pos_ = other.pos_;
        size_ = other.size_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
    }
    printf("Triangle assigned\n");
    return *this;
}

Triangle::~Triangle() {
    printf("Triangle destroyed\n");
}

bool Triangle::hasPointIn(QPoint point) const {
    QPolygon poly;
    poly << QPoint(pos_.x() + size_.width() / 2, pos_.y())
         << QPoint(pos_.x() + size_.width(), pos_.y() + size_.height())
         << QPoint(pos_.x(), pos_.y() + size_.height());

    bool contains = poly.containsPoint(point, Qt::OddEvenFill);
    printf("Triangle contains point (%d, %d): %s\n", point.x(), point.y(), contains ? "true" : "false");
    return contains;
}

void Triangle::draw(QPainter& painter) const {
    QPolygon poly;
    poly << QPoint(pos_.x() + size_.width() / 2, pos_.y())
         << QPoint(pos_.x() + size_.width(), pos_.y() + size_.height())
         << QPoint(pos_.x(), pos_.y() + size_.height());

    if (isSelected_) {
        painter.setPen(QPen(Qt::red, 2));
    } else {
        painter.setPen(QPen(Qt::black, 1));
    }

    painter.setBrush(color_);
    painter.drawPolygon(poly);
    printf("Triangle drawn, color (%d,%d,%d)\n",
           color_.red(), color_.green(), color_.blue());
}
