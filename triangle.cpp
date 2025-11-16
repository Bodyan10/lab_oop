#include "triangle.h"
#include <QPainter>
#include <QPolygon>
#include <cstdio>

Triangle::Triangle(QPoint coordinates, QSize size, QColor color, bool selected, std::string name) : Shape(coordinates, size, color, selected) {
    name_ = name;
    printf("Triangle(QPoint coordinates, QSize size, QColor color, bool selected, std::string name)");
}

Triangle::Triangle(const Triangle& other) : Shape(other.pos_, other.size_, other.color_, other.isSelected_){
    name_ = other.name_;
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

void Triangle::draw(QPainter& painter) const {
    QPolygon poly;
    poly << QPoint(pos_.x() + size_.width() / 2, pos_.y())
         << QPoint(pos_.x() + size_.width(), pos_.y() + size_.height())
         << QPoint(pos_.x(), pos_.y() + size_.height());
    painter.setPen(QPen(Qt::black, 1));

    painter.setBrush(color_);
    painter.drawPolygon(poly);
    printf("Triangle drawn, color (%d,%d,%d)\n",
           color_.red(), color_.green(), color_.blue());
}
