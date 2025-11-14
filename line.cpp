#include "line.h"
#include <QPainter>
#include <cmath>
#include <cstdio>

Line::Line(QPoint coordinates, QSize size, QColor color)
    : Shape(coordinates, size, color) {
    name_ = "Line";
    printf("Line created from (%d, %d) to (%d, %d)\n",
           coordinates.x(), coordinates.y(),
           coordinates.x() + size.width(), coordinates.y() + size.height());
}

Line::Line(const Line& other)
    : Shape(other.pos_, other.size_, other.color_), lineWidth_(other.lineWidth_) {
    name_ = "Line";
    isSelected_ = other.isSelected_;
    printf("Line copied\n");
}

Line& Line::operator=(const Line& other) {
    if (this != &other) {
        pos_ = other.pos_;
        size_ = other.size_;
        color_ = other.color_;
        isSelected_ = other.isSelected_;
        lineWidth_ = other.lineWidth_;
    }
    printf("Line assigned\n");
    return *this;
}

Line::~Line() {
    printf("Line destroyed\n");
}

bool Line::hasPointIn(QPoint point) const {
    QPoint p1 = pos_;
    QPoint p2 = pos_ + QPoint(size_.width(), size_.height());

    // Calculate distance from point to line
    double A = point.x() - p1.x();
    double B = point.y() - p1.y();
    double C = p2.x() - p1.x();
    double D = p2.y() - p1.y();

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    double param = -1;
    if (len_sq != 0) param = dot / len_sq;

    double xx, yy;
    if (param < 0) {
        xx = p1.x();
        yy = p1.y();
    } else if (param > 1) {
        xx = p2.x();
        yy = p2.y();
    } else {
        xx = p1.x() + param * C;
        yy = p1.y() + param * D;
    }

    double dx = point.x() - xx;
    double dy = point.y() - yy;
    double distance = sqrt(dx * dx + dy * dy);

    bool contains = distance <= lineWidth_ + 2; // +2 for selection tolerance
    printf("Line contains point (%d, %d): %s\n", point.x(), point.y(), contains ? "true" : "false");
    return contains;
}

void Line::draw(QPainter& painter) const {
    QPoint p1 = pos_;
    QPoint p2 = pos_ + QPoint(size_.width(), size_.height());

    if (isSelected_) {
        painter.setPen(QPen(Qt::red, lineWidth_ + 2));
    } else {
        painter.setPen(QPen(color_, lineWidth_));
    }

    painter.drawLine(p1, p2);
    printf("Line drawn from (%d, %d) to (%d, %d), color (%d,%d,%d)\n",
           p1.x(), p1.y(), p2.x(), p2.y(),
           color_.red(), color_.green(), color_.blue());
}

void Line::adjustToFitBounds(const QRect& widgetBounds) {
    QPoint p1 = pos_;
    QPoint p2 = pos_ + QPoint(size_.width(), size_.height());

    // Подгоняем обе точки линии
    p1.setX(std::clamp(p1.x(), 0, widgetBounds.width()));
    p1.setY(std::clamp(p1.y(), 0, widgetBounds.height()));
    p2.setX(std::clamp(p2.x(), 0, widgetBounds.width()));
    p2.setY(std::clamp(p2.y(), 0, widgetBounds.height()));

    pos_ = p1;
    size_ = QSize(p2.x() - p1.x(), p2.y() - p1.y());
}
