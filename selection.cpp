#include "selection.h"
#include <QBrush>
#include <cstdio>

Selection::Selection() {
    printf("Selection created\n");
}

Selection::~Selection() {
    // НЕ удаляем объекты, так как они принадлежат основному контейнеру
    selectedShapes.clear();
    printf("Selection destroyed\n");
}

void Selection::addObject(Shape* shape) {
    selectedShapes.push_back(shape);
}

void Selection::removeAt(int index) {
    if (index >= 0 && index < (int)selectedShapes.size()) {
        selectedShapes.erase(selectedShapes.begin() + index);
    }
}

void Selection::removeElement(Shape* element) {
    for (int i = 0; i < getCount(); i++) {
        if (getObject(i) == element) {
            removeAt(i);
            return;
        }
    }
}


void Selection::clear() {
    selectedShapes.clear();
}

int Selection::getCount() const {
    return selectedShapes.size();
}

Shape* Selection::getObject(int index) {
    if (index >= 0 && index < (int)selectedShapes.size()) {
        return selectedShapes[index];
    }
    return nullptr;
}

bool Selection::hasElement(Shape* el) const {
    for (Shape* obj : selectedShapes) {
        if (obj == el) return true;
    }
    return false;
}

void Selection::draw(QPainter& painter) const {
    if (selectedShapes.empty()) {
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

    printf("Selection drawn: area (%d,%d %dx%d) - DASHED BLUE FRAME\n", r.x(), r.y(), r.width(), r.height());
}

QRect Selection::getArea() const {
    if (selectedShapes.empty()) {
        return QRect();
    }

    QRect r = selectedShapes[0]->getBounds();
    for (size_t i = 1; i < selectedShapes.size(); i++) {
        r = r.united(selectedShapes[i]->getBounds());
    }
    return r;
}

bool Selection::hasObjectInPoint(QPoint point) const {
    for (Shape* obj : selectedShapes) {
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

void Selection::moveSelections(int diffX, int diffY) {
    for (Shape* obj : selectedShapes) {
        if (obj) {
            QPoint newPos = obj->getPos() + QPoint(diffX, diffY);
            obj->move(newPos.x(), newPos.y());
        }
    }
    printf("Selection moved by (%d, %d)\n", diffX, diffY);
}

bool Selection::resizeSelections(int diffX, int diffY) {
    QRect bounds = getArea();
    if (bounds.isNull()) return false;

    // ВЫЧИСЛЯЕМ НОВЫЕ ГРАНИЦЫ ВЫДЕЛЕНИЯ
    QPoint newPos = bounds.topLeft();
    QSize newSize = bounds.size();

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
    if (newSize.width() < 10) newSize.setWidth(10);
    if (newSize.height() < 10) newSize.setHeight(10);

    // ЕДИНЫЙ АЛГОРИТМ МАСШТАБИРОВАНИЯ
    scaleToNewBounds(QRect(newPos, newSize));

    printf("Selection resized from %dx%d to %dx%d\n",
           bounds.width(), bounds.height(), newSize.width(), newSize.height());
    return true;
}

void Selection::scaleToNewBounds(const QRect& newBounds) {
    QRect oldBounds = getArea();
    if (oldBounds.isNull()) return;

    // ВЫЧИСЛЯЕМ КОЭФФИЦИЕНТЫ МАСШТАБИРОВАНИЯ
    double scaleX = static_cast<double>(newBounds.width()) / oldBounds.width();
    double scaleY = static_cast<double>(newBounds.height()) / oldBounds.height();

    printf("Scaling: old(%d,%d %dx%d) new(%d,%d %dx%d) scale(%.3f,%.3f)\n",
           oldBounds.x(), oldBounds.y(), oldBounds.width(), oldBounds.height(),
           newBounds.x(), newBounds.y(), newBounds.width(), newBounds.height(),
           scaleX, scaleY);

    // ПРИМЕНЯЕМ МАСШТАБИРОВАНИЕ КО ВСЕМ ОБЪЕКТАМ
    for (Shape* obj : selectedShapes) {
        if (!obj) continue;

        // ТЕКУЩИЕ КООРДИНАТЫ ОТНОСИТЕЛЬНО СТАРОЙ ОБЛАСТИ
        QPoint offset = obj->getPos() - oldBounds.topLeft();
        QSize currentSize = obj->getSize();

        // НОВЫЕ КООРДИНАТЫ ОТНОСИТЕЛЬНО НОВОЙ ОБЛАСТИ
        QPoint newOffset(
            qRound(offset.x() * scaleX),
            qRound(offset.y() * scaleY)
            );
        QSize newSize(
            qRound(currentSize.width() * scaleX),
            qRound(currentSize.height() * scaleY)
            );

        // МИНИМАЛЬНЫЙ РАЗМЕР ОБЪЕКТА
        newSize.setWidth(std::max(5, newSize.width()));
        newSize.setHeight(std::max(5, newSize.height()));

        // НОВАЯ ПОЗИЦИЯ
        QPoint newPos = newBounds.topLeft() + newOffset;

        obj->move(newPos.x(), newPos.y());
        obj->resize(newSize.width(), newSize.height());

        printf("Object scaled: pos(%d->%d) size(%d->%d) scale(%.3f,%.3f)\n",
               offset.x(), newOffset.x(), currentSize.width(), newSize.width(),
               scaleX, scaleY);
    }
}

bool Selection::checkBorder(const QRect& widgetBounds) {
    QRect shapeBounds = getArea();
    return widgetBounds.contains(shapeBounds);
}
