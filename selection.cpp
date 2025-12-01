#include "selection.h"
#include <QBrush>
#include <cstdio>

Selection::Selection() {
    printf("Selection created\n");
}

Selection::~Selection() {
    // Не удаляем объекты, так как они принадлежат основному контейнеру
    myStorage.clear();
    printf("Selection destroyed\n");
}

void Selection::draw(QPainter& painter) const {
    if (myStorage.empty()) {
        return;
    }

    QRect r = getArea();

    // Рисуем рамку выделения (полупрозрачную)
    painter.setPen(QPen(QColor(0, 0, 255, 128), 2, Qt::DashLine));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(r);

    // Рисуем маркеры изменения размера
    int handleSize = 8;
    QPoint alpha(-handleSize/2, -handleSize/2);

    QRect handles[8] = {
        // Угловые маркеры
        QRect(r.topLeft() + alpha, QSize(handleSize, handleSize)),
        QRect(r.topRight() + alpha, QSize(handleSize, handleSize)),
        QRect(r.bottomLeft() + alpha, QSize(handleSize, handleSize)),
        QRect(r.bottomRight() + alpha, QSize(handleSize, handleSize)),
        // Боковые маркеры
        QRect(QPoint(r.center().x() - handleSize/2, r.top() - handleSize/2), QSize(handleSize, handleSize)),
        QRect(QPoint(r.center().x() - handleSize/2, r.bottom() - handleSize/2), QSize(handleSize, handleSize)),
        QRect(QPoint(r.left() - handleSize/2, r.center().y() - handleSize/2), QSize(handleSize, handleSize)),
        QRect(QPoint(r.right() - handleSize/2, r.center().y() - handleSize/2), QSize(handleSize, handleSize))
    };

    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::blue, 2));
    painter.drawRects(handles, 8);
}

QRect Selection::getArea() const {
    if (myStorage.empty()) {
        return QRect();
    }

    QRect r = myStorage[0]->getBounds();
    for (size_t i = 1; i < myStorage.size(); i++) {
        r = r.united(myStorage[i]->getBounds());
    }
    return r;
}

bool Selection::hasObjectInPoint(QPoint point) const {
    for (MyShape* obj : myStorage) {
        if (obj && obj->hasPointIn(point)) {
            return true;
        }
    }
    return false;
}

Selection::MousePosState Selection::checkMousePos(QPoint pos) {
    QRect r = getArea();
    if (r.isNull()) {
        return MousePosState::None;
    }

    int handleSize = 8;
    QPoint alpha(-handleSize/2, -handleSize/2);

    QRect handles[8] = {
        QRect(r.topLeft() + alpha, QSize(handleSize, handleSize)),
        QRect(r.topRight() + alpha, QSize(handleSize, handleSize)),
        QRect(r.bottomLeft() + alpha, QSize(handleSize, handleSize)),
        QRect(r.bottomRight() + alpha, QSize(handleSize, handleSize)),
        QRect(QPoint(r.center().x() - handleSize/2, r.top() - handleSize/2), QSize(handleSize, handleSize)),
        QRect(QPoint(r.center().x() - handleSize/2, r.bottom() - handleSize/2), QSize(handleSize, handleSize)),
        QRect(QPoint(r.left() - handleSize/2, r.center().y() - handleSize/2), QSize(handleSize, handleSize)),
        QRect(QPoint(r.right() - handleSize/2, r.center().y() - handleSize/2), QSize(handleSize, handleSize))
    };

    // Проверяем маркеры
    if (handles[0].contains(pos)) {
        currentResizePos_ = ResizePositions::TopLeft;
        return MousePosState::TopLeft;
    }
    if (handles[1].contains(pos)) {
        currentResizePos_ = ResizePositions::TopRight;
        return MousePosState::TopRight;
    }
    if (handles[2].contains(pos)) {
        currentResizePos_ = ResizePositions::BottomLeft;
        return MousePosState::BottomLeft;
    }
    if (handles[3].contains(pos)) {
        currentResizePos_ = ResizePositions::BottomRight;
        return MousePosState::BottomRight;
    }
    if (handles[4].contains(pos)) {
        currentResizePos_ = ResizePositions::Top;
        return MousePosState::Top;
    }
    if (handles[5].contains(pos)) {
        currentResizePos_ = ResizePositions::Bottom;
        return MousePosState::Bottom;
    }
    if (handles[6].contains(pos)) {
        currentResizePos_ = ResizePositions::Left;
        return MousePosState::Left;
    }
    if (handles[7].contains(pos)) {
        currentResizePos_ = ResizePositions::Right;
        return MousePosState::Right;
    }

    currentResizePos_ = ResizePositions::None;
    return MousePosState::None;
}

void Selection::moveSelections(int diffX, int diffY, const QRect& widgetBounds) {
    printf("=== Selection::moveSelections called ===\n");
    printf("Diff: (%d,%d), Selection count: %zu\n", diffX, diffY, myStorage.size());

    // 1. проверяем фигуры перед перемещением
    for (MyShape* obj : myStorage) {
        bool canMove = obj->canMove(widgetBounds, diffX, diffY);
        if (!canMove) {
            return;
        }
    }

    // 2. Перемещаем все фигуры
    for (MyShape* obj : myStorage) {
        if (obj) {
            obj->move(diffX, diffY);
        }
    }

    updateShapesRelativeFrame();
}

bool Selection::resizeSelections(int diffX, int diffY, const QRect& widget_rect) {
    QRect new_bounds = frameOfSelected_;
    if (new_bounds.isNull()) {
        new_bounds = getArea();
    }

    // ВЫЧИСЛЯЕМ НОВЫЕ ГРАНИЦЫ ВЫДЕЛЕНИЯ
    QPoint newPos = new_bounds.topLeft();
    QSize newSize = new_bounds.size();

    switch (currentResizePos_) {
    case ResizePositions::TopLeft:
        newPos += QPoint(diffX, diffY);
        newSize -= QSize(diffX, diffY);
        break;
    case ResizePositions::Top:
        newPos += QPoint(0, diffY);
        newSize -= QSize(0, diffY);
        break;
    case ResizePositions::TopRight:
        newPos += QPoint(0, diffY);
        newSize += QSize(diffX, -diffY);
        break;
    case ResizePositions::Left:
        newPos += QPoint(diffX, 0);
        newSize -= QSize(diffX, 0);
        break;
    case ResizePositions::Right:
        newSize += QSize(diffX, 0);
        break;
    case ResizePositions::BottomLeft:
        newPos += QPoint(diffX, 0);
        newSize += QSize(-diffX, diffY);
        break;
    case ResizePositions::Bottom:
        newSize += QSize(0, diffY);
        break;
    case ResizePositions::BottomRight:
        newSize += QSize(diffX, diffY);
        break;
    case ResizePositions::None:
        return false;
    }

    // МИНИМАЛЬНЫЙ РАЗМЕР ВЫДЕЛЕНИЯ
    if (newSize.width() < 10) {
        newSize.setWidth(10);
    }
    if (newSize.height() < 10) {
        newSize.setHeight(10);
    }

    new_bounds = QRect(newPos, newSize);

    // Проверяем можно ли изменить размер выделенных объектов
    for (int i = 0; i < getCount(); i++) {
        MyShape* shape = myStorage[i];
        QPoint check_point(new_bounds.topLeft().x() + shapesRelativeFrame[i].first.x() * new_bounds.width(),
                           new_bounds.topLeft().y() + shapesRelativeFrame[i].first.y() * new_bounds.height());
        QSize check_size(new_bounds.width() * shapesRelativeFrame[i].second.width(),
                         new_bounds.height() * shapesRelativeFrame[i].second.height());
        if (!(shape->canMoveAndResize(widget_rect, check_point, check_size))) {
            return false;
        }
    }

    // изменяем размер выделенных объектов
    for (int i = 0; i < getCount(); i++) {
        if (myStorage[i]) {
            MyShape* shape = myStorage[i];

            // Вычисляем новые абсолютные координаты
            QPoint new_abs_pos(
                new_bounds.x() + shapesRelativeFrame[i].first.x() * new_bounds.width(),
                new_bounds.y() + shapesRelativeFrame[i].first.y() * new_bounds.height()
                );
            QSize new_size(
                new_bounds.width() * shapesRelativeFrame[i].second.width(),
                new_bounds.height() * shapesRelativeFrame[i].second.height()
                );

            // Вычисляем смещение от текущей позиции к новой
            QPoint current_pos = shape->getPos();
            int diffX = new_abs_pos.x() - current_pos.x();
            int diffY = new_abs_pos.y() - current_pos.y();

            shape->move(diffX, diffY);
            shape->resize(new_size.width(), new_size.height());
        }
    }

    frameOfSelected_ = new_bounds;
    return true;
}

void Selection::updateShapesRelativeFrame() {
    shapesRelativeFrame.clear();

    frameOfSelected_ = getArea();

    for (MyShape* obj : myStorage) {
        QPointF objRelativePos(static_cast<double>(obj->getPos().x() - frameOfSelected_.topLeft().x()) / frameOfSelected_.width(),
                               static_cast<double>(obj->getPos().y() - frameOfSelected_.topLeft().y()) / frameOfSelected_.height());

        QSizeF objRelativeSize(static_cast<double>(obj->getSize().width()) / frameOfSelected_.width(),
                               static_cast<double>(obj->getSize().height()) / frameOfSelected_.height());

        shapesRelativeFrame.push_back(std::make_pair(objRelativePos, objRelativeSize));
    }
}

void Selection::clear() {
    // Снимаем выделение со всех объектов перед очисткой
    myStorage.clear();
    notifyEveryone();
    updateShapesRelativeFrame();
}
void Selection::removeAt(int index) {
    if (index < 0 || index >= getCount()) {
        return;
    }

    myStorage.erase(myStorage.begin() + index);
    updateShapesRelativeFrame();
    notifyEveryone();
}

void Selection::removeSelected() {
    clear();
    updateShapesRelativeFrame();
    notifyEveryone();
}

void Selection::removeElement(MyShape* element) {
    for (int i = 0; i < getCount(); i++) {
        if (getObject(i) == element) {
            removeAt(i);
            return;
        }
    }
    updateShapesRelativeFrame();
    notifyEveryone();
}

void Selection::addObject(MyShape* new_object) {
    Container::addObject(new_object);
    updateShapesRelativeFrame();
}
