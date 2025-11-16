#include "rectangle.h"
#include <QPainter>
#include <cstdio>

Rectangle::Rectangle(QPoint coordinates, QSize size, QColor color, bool selected, std::string name) : Shape(coordinates, size, color, selected) {
    name_ = name;
    printf("Rectangle(QPoint coordinates, QSize size, QColor color, bool selected, std::string name)");
}

Rectangle::Rectangle(const Rectangle& other) : Shape(other.pos_, other.size_, other.color_, other.isSelected_){
    name_ = other.name_;
    printf("Rectangle copied\n");
}

Rectangle& Rectangle::operator=(const Rectangle& other) {
    if (this != &other) {
        pos_ = other.pos_;
        size_ = other.size_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
        name_ = other.name_;
    }
    printf("Rectangle assigned\n");
    return *this;
}

Rectangle::~Rectangle() {
    printf("Rectangle destroyed\n");
}

void Rectangle::draw(QPainter& painter) const {
    QRect r(pos_, size_);

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(color_);
    painter.drawRect(r);
}
