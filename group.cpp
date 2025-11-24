#include "group.h"
#include <shape.h>
#include <cstdio>
#include <algorithm>

Group::Group() : Shape() {
    printf("Group created\n");
}

Group::Group(QPoint coordinates, QSize size, QColor color, bool selected)
    : Shape(coordinates, size, color, selected) {
    printf("Group created with parameters\n");
}

Group::Group(const Group& other) : Shape(other) {
    printf("Group copied\n");
    // TODO: реализовать глубокое копирование детей
}

Group& Group::operator=(const Group& other) {
    if (this != &other) {
        Shape::operator=(other);
        // TODO: реализовать глубокое копирование детей
    }
    return *this;
}

Group::~Group() {
    printf("Group destroyed\n");
    // ⚠️ ВАЖНО: НЕ удаляем детей здесь!
    // Дети должны продолжать существовать после разгруппировки
    children_.clear(); // Просто очищаем вектор, не удаляя объекты
    relativeCoordinates_.clear();
}

void Group::updateRelativeCoordinates() {
    relativeCoordinates_.clear();

    if (children_.empty() || size_.width() == 0 || size_.height() == 0) {
        return;
    }

    for (Shape* child : children_) {
        QPoint childPos = child->getPos();
        QSize childSize = child->getSize();

        // Вычисляем относительные координаты относительно группы
        QPointF relativePos(
            static_cast<double>(childPos.x() - pos_.x()) / size_.width(),
            static_cast<double>(childPos.y() - pos_.y()) / size_.height()
            );

        QSizeF relativeSize(
            static_cast<double>(childSize.width()) / size_.width(),
            static_cast<double>(childSize.height()) / size_.height()
            );

        relativeCoordinates_.push_back(std::make_pair(relativePos, relativeSize));
    }
}

void Group::applyRelativeCoordinates() {
    if (children_.empty() || relativeCoordinates_.empty() ||
        children_.size() != relativeCoordinates_.size()) {
        return;
    }

    for (size_t i = 0; i < children_.size(); i++) {
        Shape* child = children_[i];
        const auto& relCoord = relativeCoordinates_[i];

        // Вычисляем абсолютные координаты из относительных
        QPoint newPos(
            pos_.x() + relCoord.first.x() * size_.width(),
            pos_.y() + relCoord.first.y() * size_.height()
            );

        QSize newSize(
            relCoord.second.width() * size_.width(),
            relCoord.second.height() * size_.height()
            );

        // Гарантируем минимальный размер
        newSize.setWidth(std::max(1, newSize.width()));
        newSize.setHeight(std::max(1, newSize.height()));

        // Применяем изменения
        QPoint currentPos = child->getPos();
        child->move(newPos.x() - currentPos.x(), newPos.y() - currentPos.y());
        child->resize(newSize.width(), newSize.height());
    }
}

// Composite methods
void Group::addShape(Shape* shape) {
    if (shape && shape != this) {
        children_.push_back(shape);
        updateGroupBounds();
        updateRelativeCoordinates();
    }
}

void Group::removeShape(Shape* shape) {
    for (auto it = children_.begin(); it != children_.end(); ++it) {
        if (*it == shape) {
            // Только удаляем указатель из вектора, НЕ удаляем объект!
            children_.erase(it);
            updateGroupBounds();
            updateRelativeCoordinates();
            return;
        }
    }
}

std::vector<Shape*> Group::getShapes() const {
    return children_;
}

// Shape operations
void Group::move(int x, int y) {
    // Перемещаем всех детей
    for (Shape* child : children_) {
        child->move(x, y);
    }
    // Обновляем свою позицию
    pos_ += QPoint(x, y);
}

void Group::resize(int newWidth, int newHeight) {
    if (children_.empty() || newWidth <= 0 || newHeight <= 0) {
        size_.setWidth(newWidth);
        size_.setHeight(newHeight);
        return;
    }

    // Устанавливаем новый размер
    size_.setWidth(newWidth);
    size_.setHeight(newHeight);

    // Применяем относительные координаты для масштабирования детей
    applyRelativeCoordinates();
}

void Group::changeColor(QColor color) {
    color_ = color;
    for (Shape* child : children_) {
        child->changeColor(color);
    }
}

bool Group::hasPointIn(QPoint point) const {
    for (Shape* child : children_) {
        if (child->hasPointIn(point)) {
            return true;
        }
    }
    return false;
}

void Group::updateGroupBounds() {
    if (children_.empty()) {
        pos_ = QPoint(0, 0);
        size_ = QSize(0, 0);
        relativeCoordinates_.clear();
        return;
    }

    // Находим общие границы всех детей
    QRect bounds = children_[0]->getBounds();
    for (size_t i = 1; i < children_.size(); i++) {
        bounds = bounds.united(children_[i]->getBounds());
    }

    pos_ = bounds.topLeft();
    size_ = bounds.size();

    // Обновляем относительные координаты
    updateRelativeCoordinates();
}

void Group::draw(QPainter &painter) const {
    for (Shape* child : children_) {
        child->draw(painter);
    }
    painter.setPen(QPen(Qt::gray, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(getBounds());
}

void Group::setSelected(bool selected) {
    isSelected_ = selected;
    for (Shape* child : children_) {
        child->setSelected(selected);
    }
}

bool Group::isSelected() const {
    // Группа считается выделенной, если все дети выделены
    if (children_.empty()) return false;

    for (Shape* child : children_) {
        if (!child->isSelected()) {
            return false;
        }
    }
    return true;
}

bool Group::adjustToFitBounds(const QRect& widgetBounds) {
    bool allAdjusted = true;

    // Сначала корректируем саму группу
    QRect currentBounds = getBounds();
    if (!widgetBounds.contains(currentBounds)) {
        int newX = std::clamp(pos_.x(), 0, widgetBounds.width() - size_.width());
        int newY = std::clamp(pos_.y(), 0, widgetBounds.height() - size_.height());

        if (newX != pos_.x() || newY != pos_.y()) {
            move(newX - pos_.x(), newY - pos_.y());
            allAdjusted = false;
        }

        // Корректируем размер если нужно
        int maxWidth = widgetBounds.width() - newX;
        int maxHeight = widgetBounds.height() - newY;

        if (size_.width() > maxWidth || size_.height() > maxHeight) {
            int newWidth = std::min(size_.width(), maxWidth);
            int newHeight = std::min(size_.height(), maxHeight);
            resize(newWidth, newHeight);
            allAdjusted = false;
        }
    }

    return allAdjusted;
}

bool Group::canMove(const QRect& widgetBounds, int diffx, int diffy) {
    for (Shape* child : children_) {
        if (!child->canMove(widgetBounds, diffx, diffy)) {
            return false;
        }
    }
    return true;
}

bool Group::canMoveAndResize(const QRect& widgetBounds, const QPoint& new_pos, const QSize& new_size) {
    // Проверяем новые границы группы
    QRect newBounds(new_pos, new_size);
    if (!widgetBounds.contains(newBounds)) {
        return false;
    }

    return true;
}

void Group::load(FILE* file, ShapeFactory* factory) {
    int r, g, b;
    size_t shapeCount;

    if (fscanf(file, "%d %d %d %d %d %d %d %zu\n",
               &pos_.rx(), &pos_.ry(),
               &size_.rwidth(), &size_.rheight(),
               &r, &g, &b, &shapeCount) == 8) {

        color_ = QColor(r, g, b);

        for (size_t i = 0; i < shapeCount; i++) {
            char shapeType;
            fscanf(file, "%c\n", &shapeType);

            if (factory) {
                Shape* shape = factory->createShape(shapeType);
                if (shape) {
                    shape->load(file, factory);
                    children_.push_back(shape);
                }
            }
        }
        updateGroupBounds(); // Это вызовет updateRelativeCoordinates()
    }
}

void Group::save(FILE* file) {
    fprintf(file, "%c\n%d %d %d %d %d %d %d %zu\n",
            getTypeCode(),
            pos_.x(), pos_.y(),
            size_.width(), size_.height(),
            color_.red(), color_.green(), color_.blue(),
            children_.size());

    for (Shape* child : children_) {
        child->save(file);
    }
}

char Group::getTypeCode() const {
    return 'G';
}
