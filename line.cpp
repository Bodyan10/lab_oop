#include "line.h"
#include <QPainter>
#include <cstdio>

Line::Line() : Shape() {
}

Line::Line(QPoint coordinates, QSize size, QColor color, bool selected, int lineWidth) : Shape(coordinates, size, color, selected) {
    lineWidth_ = lineWidth;
    printf("Line(QPoint coordinates, QSize size, QColor color, bool selected, std::string name, int lineWidth) : Shape(coordinates, size, color, selected) {");
}

Line::Line(const Line& other) : Shape(other.pos_, other.size_, other.color_, other.isSelected_){
    lineWidth_ = other.lineWidth_;
    printf("Line(const Line& other) : Shape(other.pos_, other.size_, other.color_, other.isSelected_)");
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

void Line::draw(QPainter& painter) const {
    QPoint p1 = pos_;
    QPoint p2 = pos_ + QPoint(size_.width(), size_.height());

    painter.setPen(QPen(color_, lineWidth_));

    painter.drawLine(p1, p2);
    printf("Line drawn from (%d, %d) to (%d, %d), color (%d,%d,%d)\n",
           p1.x(), p1.y(), p2.x(), p2.y(),
           color_.red(), color_.green(), color_.blue());
}

bool Line::adjustToFitBounds(const QRect& widgetBounds) {

    if (widgetBounds.contains(getBounds())) {
        return true;
    }

    QPoint p1 = pos_;
    QPoint p2 = pos_ + QPoint(size_.width(), size_.height());

    // Подгоняем обе точки линии
    p1.setX(std::clamp(p1.x(), 0, widgetBounds.width()));
    p1.setY(std::clamp(p1.y(), 0, widgetBounds.height()));
    p2.setX(std::clamp(p2.x(), 0, widgetBounds.width()));
    p2.setY(std::clamp(p2.y(), 0, widgetBounds.height()));

    pos_ = p1;
    size_ = QSize(p2.x() - p1.x(), p2.y() - p1.y());
    return false;
}

void Line::save(FILE* stream) {
    fprintf(stream, "%c\n%d %d %d %d %d %d %d %d\n", getTypeCode(),
            pos_.x(), pos_.y(), size_.width(), size_.height(),
            color_.red(), color_.green(), color_.blue(), lineWidth_);
}

void Line::load(FILE* stream, ShapeFactory*) {
    int r, g, b, lw;
    if (fscanf(stream, "%d %d %d %d %d %d %d %d\n",
               &pos_.rx(), &pos_.ry(),
               &size_.rwidth(), &size_.rheight(),
               &r, &g, &b, &lw) == 8) {
        color_ = QColor(r, g, b);
        lineWidth_ = lw;
    }
}

char Line::getTypeCode() const {
    return 'L';
}




