#include "rectangle.h"
#include <QPainter>
#include <cstdio>

Rectangle::Rectangle() : MyShape() {
}

Rectangle::Rectangle(QPoint coordinates, QSize size, QColor color, bool selected) : MyShape(coordinates, size, color, selected) {
    printf("Rectangle(QPoint coordinates, QSize size, QColor color, bool selected, std::string name)");
}

Rectangle::Rectangle(const Rectangle& other) : MyShape(other.pos_, other.size_, other.color_, other.isSelected_){
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

void Rectangle::draw(QPainter& painter) const {
    QRect r(pos_, size_);

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(color_);
    painter.drawRect(r);
}

char Rectangle::getTypeCode() const {
    return 'R';
}
