#include "group.h"
#include "myshape.h"
#include <cstdio>

Group::Group() : MyShape() {
    printf("Group created\n");
}

Group::Group(QPoint coordinates, QSize size, QColor color, bool selected)
    : MyShape(coordinates, size, color, selected) {
    printf("Group created with parameters\n");
}

Group::~Group() {
    printf("Group destroyed\n");
    for (MyShape* shape : children_) {
        delete shape;
    }
}
void Group::updateRelativeCoordinates() {
    relativeCoordinates_.clear();

    if (children_.empty() || size_.width() == 0 || size_.height() == 0) {
        return;
    }

    for (MyShape* child : children_) {
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
    if (children_.empty()) {
        return;
    }

    for (size_t i = 0; i < children_.size(); i++) {
        MyShape* child = children_[i];
        const auto& relCoord = relativeCoordinates_[i];

        // Вычисляем абсолютные координаты из относительных
        QPoint newPos(
            pos_.x() + relCoord.first.x() * size_.width(),
            pos_.y() + relCoord.first.y() * size_.height()
            );

        QSize newSize(
            relCoord.second.width() * size_.width(),
            relCoord.second.height() * size_.height()
            );;

        // Применяем изменения
        QPoint currentPos = child->getPos();
        child->move(newPos.x() - currentPos.x(), newPos.y() - currentPos.y());
        child->resize(newSize.width(), newSize.height());
    }
}

// Composite methods
void Group::addShape(MyShape* shape) {
    if (shape && shape != this) {
        children_.push_back(shape);
        updateGroupBounds();
        updateRelativeCoordinates();
    }
}

void Group::removeShape(MyShape* shape) {
    for (auto it = children_.begin(); it != children_.end(); ) {
        if (*it == shape) {
            it = children_.erase(it);  // Правильно обрабатываем итератор
            updateGroupBounds();
            updateRelativeCoordinates();
            break;  // Выходим после нахождения
        } else {
            ++it;
        }
    }
}

std::vector<MyShape*> Group::getShapes() const {
    return children_;
}

// MyShape operations
void Group::move(int x, int y) {
    // Перемещаем всех детей
    for (MyShape* child : children_) {
        child->move(x, y);
    }
    // Обновляем свою позицию
    pos_ += QPoint(x, y);
    notifyEveryone();
}

void Group::resize(int newWidth, int newHeight) {
    // Устанавливаем новый размер
    size_.setWidth(newWidth);
    size_.setHeight(newHeight);

    // Применяем относительные координаты для масштабирования детей
    applyRelativeCoordinates();
}

void Group::changeColor(QColor color) {
    color_ = color;
    for (MyShape* child : children_) {
        child->changeColor(color);
    }
}

bool Group::hasPointIn(QPoint point) const {
    for (MyShape* child : children_) {
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
    for (MyShape* child : children_) {
        child->draw(painter);
    }
    painter.setPen(QPen(Qt::gray, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(getBounds());
}

void Group::setSelected(bool selected) {
    isSelected_ = selected;
    for (MyShape* child : children_) {
        child->setSelected(selected);
    }
    notifyEveryone();
}

bool Group::isSelected() const {
    if (children_.empty()) {
        return false;
    }

    for (MyShape* child : children_) {
        if (!child->isSelected()) {
            return false;
        }
    }
    return true;
}

bool Group::canMove(const QRect& widgetBounds, int diffx, int diffy) {
    for (MyShape* child : children_) {
        if (!child->canMove(widgetBounds, diffx, diffy)) {
            return false;
        }
    }
    return true;
}

void Group::load(FILE* file, ShapeFactory* factory) {
    size_t shapeCount;

    if (fscanf(file, "%zu\n", &shapeCount) == 1) {

        for (size_t i = 0; i < shapeCount; i++) {
            char shapeType;
            fscanf(file, "%c\n", &shapeType);

            if (factory) {
                MyShape* shape = factory->createShape(shapeType);
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
    fprintf(file, "%c\n%zu\n",
            getTypeCode(),
            children_.size());

    for (MyShape* child : children_) {
        child->save(file);
    }
}

char Group::getTypeCode() const {
    return 'G';
}
