#include "ellipse.h"
#include <QPainter>
#include <cstdio>

Ellipse::Ellipse(QPoint coordinates, QSize size, QColor color, bool selected, std::string name) : Shape(coordinates, size, color, selected) {
    name_ = name;
    printf("Ellipse(QPoint coordinates, QSize size, QColor color, bool selected, std::string name)");
}

Ellipse::Ellipse(const Ellipse& other) : Shape(other.pos_, other.size_, other.color_, other.isSelected_){
    name_ = other.name_;
    printf("Ellipse copied\n");
}

Ellipse& Ellipse::operator=(const Ellipse& other) {
    if (this != &other) {
        pos_ = other.pos_;
        size_ = other.size_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
        name_ = other.name_;
    }
    printf("Ellipse assigned\n");
    return *this;
}

Ellipse::~Ellipse() {
    printf("Ellipse destroyed\n");
}

void Ellipse::draw(QPainter& painter) const {
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(color_);
    painter.drawEllipse(pos_.x(), pos_.y(), size_.width(), size_.height());
}
