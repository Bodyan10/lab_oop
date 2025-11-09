#include "selection.h"
#include <QBrush>
#include <algorithm>
#include <cstdio>

Selection::Selection() {
    printf("Selection created\n");
}

void Selection::draw(QPainter& painter) const {
    if (Container<Shape*>::size() == 0) {
        return;
    }

    QRect r = getArea();

    // Рисуем ПУНКТИРНУЮ рамку выделения (полупрозрачную)
    painter.setPen(QPen(QColor(0, 0, 255, 128), 2, Qt::DashLine)); // Полупрозрачный синий
    painter.setBrush(Qt::NoBrush); // Без заливки!
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
    if (Container<Shape*>::size() == 0) {
        return QRect();
    }

    QRect r = at(0)->getBounds();
    for (int i = 1; i < Container<Shape*>::size(); i++) {
        r = r.united(at(i)->getBounds());
    }
    return r;
}

bool Selection::hasObjectInPoint(QPoint point) const {
    for (const auto& obj : *this) {
        if (obj->hasPointIn(point)) {
            return true;
        }
    }
    return false;
}

Selection::MousePosState Selection::checkMousePos(QPoint pos) {
    QRect r = getArea();
    if (r.isNull()) return MousePosState::None;

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
    for (const auto& obj : *this) {
        QPoint newPos = obj->getPos() + QPoint(diffX, diffY);
        obj->move(newPos.x(), newPos.y());
    }
    printf("Selection moved by (%d, %d)\n", diffX, diffY);
}

bool Selection::resizeSelections(int diffX, int diffY) {
    QRect bounds = getArea();
    if (bounds.isNull()) return false;

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

    // Проверяем минимальный размер
    if (newSize.width() < 5) newSize.setWidth(5);
    if (newSize.height() < 5) newSize.setHeight(5);

    // Применяем изменения ко всем выделенным объектам
    for (auto& obj : *this) {
        QPointF relativePos(
            static_cast<float>(obj->getPos().x() - bounds.x()) / bounds.width(),
            static_cast<float>(obj->getPos().y() - bounds.y()) / bounds.height()
            );
        QSizeF relativeSize(
            static_cast<float>(obj->getSize().width()) / bounds.width(),
            static_cast<float>(obj->getSize().height()) / bounds.height()
            );

        QPoint objNewPos(
            newPos.x() + relativePos.x() * newSize.width(),
            newPos.y() + relativePos.y() * newSize.height()
            );
        QSize objNewSize(
            relativeSize.width() * newSize.width(),
            relativeSize.height() * newSize.height()
            );

        obj->move(objNewPos.x(), objNewPos.y());
        obj->resize(objNewSize.width(), objNewSize.height());
    }

    printf("Selection resized to %dx%d\n", newSize.width(), newSize.height());
    return true;
}

void Selection::updateRelativeInfo() {
    relativeInfo_.clear();
    QRect bounds = getArea();

    for (auto& obj : *this) {
        QPointF relativePos(
            static_cast<float>(obj->getPos().x() - bounds.x()) / bounds.width(),
            static_cast<float>(obj->getPos().y() - bounds.y()) / bounds.height()
            );
        QSizeF relativeSize(
            static_cast<float>(obj->getSize().width()) / bounds.width(),
            static_cast<float>(obj->getSize().height()) / bounds.height()
            );

        relativeInfo_[obj] = std::make_pair(relativePos, relativeSize);
    }
}
