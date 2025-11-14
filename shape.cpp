#include "shape.h"
#include <cstdio>

Shape::Shape(QPoint coordinates, QSize size, QColor color)
    : pos_(coordinates), size_(size), color_(color) {
    printf("Shape created\n");
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
    printf("Shape '%s' resized to %dx%d\n", name_.c_str(), x, y);
}

void Shape::changeColor(QColor color) {
    color_ = color;
    printf("Shape '%s' color changed to (%d, %d, %d)\n", name_.c_str(), color.red(), color.green(), color.blue());
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
    printf("Shape '%s' selection: %s\n", name_.c_str(), selected ? "selected" : "deselected");
}

bool Shape::isSelected() const {
    return isSelected_;
}

std::string Shape::name() const {
    return name_;
}

bool Shape::checkBounds(const QRect& widgetBounds) {
    QRect shapeBounds = getBounds();
    return widgetBounds.contains(shapeBounds);
}

void Shape::adjustToFitBounds(const QRect& widgetBounds) {
    QRect currentBounds = getBounds();

    // Если уже в границах - ничего не делаем
    if (widgetBounds.contains(currentBounds)) {
        return;
    }

    // Подгоняем позицию (левая и верхняя границы)
    int newX = std::clamp(pos_.x(), 0, widgetBounds.width() - 1);
    int newY = std::clamp(pos_.y(), 0, widgetBounds.height() - 1);

    // Подгоняем размер (правая и нижняя границы)
    int maxWidth = widgetBounds.width() - newX;
    int maxHeight = widgetBounds.height() - newY;

    size_.setWidth(std::min(size_.width(), maxWidth));
    size_.setHeight(std::min(size_.height(), maxHeight));

    // Гарантируем минимальный размер
    size_.setWidth(std::max(1, size_.width()));
    size_.setHeight(std::max(1, size_.height()));

    pos_ = QPoint(newX, newY);

    printf("Shape adjusted: pos(%d,%d) size(%dx%d)\n", newX, newY, size_.width(), size_.height());
}
