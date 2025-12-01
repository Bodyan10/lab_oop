#include "myshape.h"
#include <cstdio>


MyShape::MyShape() {
}

MyShape::MyShape(QPoint coordinates, QSize size, QColor color, bool selected) {
    pos_ = coordinates;
    size_ = size;
    color_ = color;
    isSelected_ = selected;
    printf("MyShape(QPoint coordinates, QSize size, QColor color, bool selected)");
}

MyShape::~MyShape() {
    printf("MyShape destroyed\n");
}

QColor MyShape::getColor() const { return color_; }

void MyShape::move(int x, int y) {
    pos_ = pos_ + QPoint(x, y);
    notifyEveryone();
}

void MyShape::resize(int x, int y) {
    size_.setWidth(x);
    size_.setHeight(y);
}

void MyShape::changeColor(QColor color) {
    color_ = color;
}

QSize MyShape::getSize() const { return size_; }

QPoint MyShape::getPos() const { return pos_; }

QPoint MyShape::getCentralPos() const {
    return pos_ + QPoint(size_.width() / 2, size_.height() / 2);
}

QRect MyShape::getBounds() const {
    return QRect(pos_, size_);
}

void MyShape::setSelected(bool selected) {
    isSelected_ = selected;
    notifyEveryone();
}

bool MyShape::isSelected() const {
    return isSelected_;
}

bool MyShape::adjustToFitBounds(const QRect& widgetBounds) {
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

bool MyShape::hasPointIn(QPoint point) const {
    QRect rect = getBounds();
    if (rect.contains(point)) {
        return true;
    }
    return false;
}

bool MyShape::canMove(const QRect& widgetBounds, int diffx, int diffy){
    QPoint newPos = getPos() + QPoint(diffx, diffy);
    QSize currentSize = getSize();
    QRect newBounds(newPos, currentSize);

    return widgetBounds.contains(newBounds);
}

bool MyShape::canMoveAndResize(const QRect& widgetBound, const QPoint& new_pos, const QSize& new_size) {
    return widgetBound.contains(QRect(new_pos, new_size));
}

void MyShape::save(FILE* file) {
    fprintf(file, "%c\n%d %d %d %d %d %d %d\n",
            getTypeCode(), pos_.x(), pos_.y(), size_.width(), size_.height(),
            color_.red(), color_.green(), color_.blue());
}

void MyShape::load(FILE* file, ShapeFactory* factory) {
    Q_UNUSED(factory);
    int r, g, b;
    if (fscanf(file, "%d %d %d %d %d %d %d\n",
               &pos_.rx(), &pos_.ry(),
               &size_.rwidth(), &size_.rheight(),
               &r, &g, &b) == 7) {
        color_ = QColor(r, g, b);
    } else {
        printf("Error reading MyShape data\n");
    }
}

void MyShape::setParent(MyShape* parent) {
    parent_ = parent;
}

MyShape* MyShape::getParent() {
    return parent_;
}
