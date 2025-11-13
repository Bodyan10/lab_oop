#include "ellipse.h"
#include <QPainter>
#include <cstdio>

Ellipse::Ellipse(QPoint coordinates, QSize size, QColor color)
    : Shape(coordinates, size, color) {
    name_ = "Ellipse";
    printf("Ellipse created at (%d, %d) size %dx%d\n",
           coordinates.x(), coordinates.y(), size.width(), size.height());
}

Ellipse::Ellipse(const Ellipse& other)
    : Shape(other.pos_, other.size_, other.color_) {
    name_ = "Ellipse";
    isSelected_ = other.isSelected_;
    printf("Ellipse copied\n");
}

Ellipse& Ellipse::operator=(const Ellipse& other) {
    if (this != &other) {
        pos_ = other.pos_;
        size_ = other.size_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
    }
    printf("Ellipse assigned\n");
    return *this;
}

Ellipse::~Ellipse() {
    printf("Ellipse destroyed\n");
}

bool Ellipse::hasPointIn(QPoint point) const {
    QPointF center = getCentralPos();
    float radiusX = size_.width() / 2.0f;
    float radiusY = size_.height() / 2.0f;

    float dx = point.x() - center.x();
    float dy = point.y() - center.y();
    float distance = (dx * dx) / (radiusX * radiusX) + (dy * dy) / (radiusY * radiusY);

    bool contains = distance <= 1.0;
    printf("Ellipse contains point (%d, %d): %s\n", point.x(), point.y(), contains ? "true" : "false");
    return contains;
}

void Ellipse::draw(QPainter& painter) const {
    if (isSelected_) {
        painter.setPen(QPen(Qt::red, 2));
    } else {
        painter.setPen(QPen(Qt::black, 1));
    }

    painter.setBrush(color_);
    painter.drawEllipse(pos_.x(), pos_.y(), size_.width(), size_.height());
    printf("Ellipse drawn at (%d, %d) size %dx%d, color (%d,%d,%d)\n",
           pos_.x(), pos_.y(), size_.width(), size_.height(),
           color_.red(), color_.green(), color_.blue());
}
