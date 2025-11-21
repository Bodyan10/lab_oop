#include "shape.h"
#include <cstdio>

Shape::Shape() {
}

Shape::Shape(QPoint coordinates, QSize size, QColor color, bool selected) {
    pos_ = coordinates;
    size_ = size;
    color_ = color;
    isSelected_ = selected;
    printf("Shape(QPoint coordinates, QSize size, QColor color, bool selected)");
}

Shape::~Shape() {
    printf("Shape '%s' destroyed\n", name_.c_str());
}

QColor Shape::getColor() const { return color_; }

void Shape::move(int x, int y) {
    pos_ = QPoint(x, y);
}

void Shape::resize(int x, int y) {
    size_.setWidth(x);
    size_.setHeight(y);
}

void Shape::changeColor(QColor color) {
    color_ = color;
}

QSize Shape::getSize() const { return size_; }

QPoint Shape::getPos() const { return pos_; }

QPoint Shape::getCentralPos() const {
    return pos_ + QPoint(size_.width() / 2, size_.height() / 2);
}

QRect Shape::getBounds() const {
    return QRect(pos_, size_);
}

void Shape::setSelected(bool selected) {
    isSelected_ = selected;
}

bool Shape::isSelected() const {
    return isSelected_;
}

std::string Shape::name() const {
    return name_;
}

bool Shape::adjustToFitBounds(const QRect& widgetBounds) {
    QRect currentBounds = getBounds();

    // Если уже в границах - ничего не делаем
    if (widgetBounds.contains(currentBounds)) {
        return true;
    }

    // Подгоняем позицию левой и верхней точки
    int newX = std::clamp(pos_.x(), 0, widgetBounds.width());
    int newY = std::clamp(pos_.y(), 0, widgetBounds.height());

    // Подгоняем размер (правая и нижняя границы)
    int maxWidth = widgetBounds.width() - newX;
    int maxHeight = widgetBounds.height() - newY;

    size_.setWidth(std::min(size_.width(), maxWidth));
    size_.setHeight(std::min(size_.height(), maxHeight));

    // Гарантируем минимальный размер
    size_.setWidth(std::max(1, size_.width()));
    size_.setHeight(std::max(1, size_.height()));

    pos_ = QPoint(newX, newY);

    return false;
}

bool Shape::hasPointIn(QPoint point) const {
    QRect rect = getBounds();
    if (rect.contains(point)) {
        return true;
    }
    return false;
}

bool Shape::canMove(const QRect& widgetBounds, int diffx, int diffy){
    QPoint newPos = getPos() + QPoint(diffx, diffy);
    QSize currentSize = getSize();
    QRect newBounds(newPos, currentSize);

    return widgetBounds.contains(newBounds);
}

bool Shape::canMoveAndResize(const QRect& widgetBound, const QPoint& new_pos, const QSize& new_size) {
    return widgetBound.contains(QRect(new_pos, new_size));
}
