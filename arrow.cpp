#include "arrow.h"
#include "myshape.h"
#include <QPainter>
#include <QLineF>
#include <cmath>
#include <cstdio>

Arrow::Arrow(MyShape* from, MyShape* to, QColor color)
    : MyShape(QPoint(0, 0), QSize(0, 0), color, false),
    fromObject(from), toObject(to), widgetBounds(0, 0, 1000, 500)
{
    printf("Arrow created from %p to %p\n", from, to);

    // Инициализируем последнюю позицию
    if (fromObject) {
        lastFromPos = fromObject->getCentralPos();
        fromObject->addObserver(this);
        toObject->addObserver(this);
    }

    updatePosition();
}

Arrow::~Arrow() {
    printf("Arrow destroyed\n");
    disconnect();
}

void Arrow::onSubjectChanged() {
    if (!fromObject || !toObject) {
        disconnect();
        return;
    }

    // Получаем текущую позицию объекта from
    QPoint currentFromPos = fromObject->getCentralPos();

    // Вычисляем разницу перемещения
    QPoint delta = currentFromPos - lastFromPos;
    lastFromPos = currentFromPos;

    if (delta.x() != 0 || delta.y() != 0) {
        printf("Arrow: from moved by (%d, %d), moving to object\n", delta.x(), delta.y());

        // ПРОВЕРЯЕМ ГРАНИЦЫ для объекта to
        if (toObject->canMove(widgetBounds, delta.x(), delta.y()) && toObject->getParent() == nullptr) {
            toObject->move(delta.x(), delta.y());
        } else {
            printf("Arrow: to object cannot move - out of bounds\n");
        }
    }
    if (fromObject->isSelected() || toObject->isSelected()) {
        this->setSelected(true);
    } else {
        this->setSelected(false);
    }

    // Всегда обновляем позицию стрелки
    updatePosition();
}

void Arrow::calculatePoints() {
    if (!fromObject || !toObject) {
        startPoint = QPointF(0, 0);
        endPoint = QPointF(0, 0);
        return;
    }

    // Центры объектов
    QPointF fromCenter = fromObject->getCentralPos();
    QPointF toCenter = toObject->getCentralPos();

    // Точки на границах объектов
    startPoint = findBorderPoint(fromObject, toCenter);
    endPoint = findBorderPoint(toObject, fromCenter);

    // Линия от startPoint к endPoint
    QLineF line(startPoint, endPoint);

    // Нормализуем линию (минимальная длина)
    if (line.length() < 1.0) {
        endPoint = startPoint + QPointF(1, 0);
        line = QLineF(startPoint, endPoint);
    }

    // Длина и угол стрелки
    double arrowLength = 12.0;
    double angle = std::atan2(line.dy(), line.dx());

    // Стрелка на конце endPoint
    arrowHead1 = QPointF(
        endPoint.x() - arrowLength * std::cos(angle - M_PI / 6),
        endPoint.y() - arrowLength * std::sin(angle - M_PI / 6)
        );

    arrowHead2 = QPointF(
        endPoint.x() - arrowLength * std::cos(angle + M_PI / 6),
        endPoint.y() - arrowLength * std::sin(angle + M_PI / 6)
        );
}

QPointF Arrow::findBorderPoint(MyShape* shape, const QPointF& target) const {
    if (!shape) {
        return QPointF();
    }

    QRect bounds = shape->getBounds();
    QPointF center = shape->getCentralPos();

    // Если цель внутри объекта, возвращаем центр
    if (bounds.contains(target.toPoint())) {
        return center;
    }

    // Вектор от центра к цели
    QPointF direction = target - center;
    double length = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

    if (length < 1.0) {
        return center;
    }

    QPointF normalized(direction.x() / length, direction.y() / length);

    // Находим пересечение с границей прямоугольника
    double tEnter = -std::numeric_limits<double>::max();
    double tExit = std::numeric_limits<double>::max();

    // Проверяем каждую грань
    if (normalized.x() != 0.0) {
        double t1 = (bounds.left() - center.x()) / normalized.x();
        double t2 = (bounds.right() - center.x()) / normalized.x();
        tEnter = std::max(tEnter, std::min(t1, t2));
        tExit = std::min(tExit, std::max(t1, t2));
    }

    if (normalized.y() != 0.0) {
        double t1 = (bounds.top() - center.y()) / normalized.y();
        double t2 = (bounds.bottom() - center.y()) / normalized.y();
        tEnter = std::max(tEnter, std::min(t1, t2));
        tExit = std::min(tExit, std::max(t1, t2));
    }

    double t = tEnter > 0 ? tEnter : tExit;

    return QPointF(
        center.x() + normalized.x() * t,
        center.y() + normalized.y() * t
        );
}

void Arrow::draw(QPainter& painter) const {
    if (!fromObject || !toObject) {
        return;
    }

    // Толщина линии в зависимости от выделения
    int penWidth = isSelected_ ? 3 : 2;
    QColor lineColor = isSelected_ ? Qt::blue : color_;

    painter.setPen(QPen(lineColor, penWidth));
    painter.setBrush(Qt::NoBrush);

    // Рисуем основную линию
    painter.drawLine(startPoint, endPoint);

    // Рисуем стрелочку
    painter.setBrush(lineColor);
    QPolygonF arrowHead;
    arrowHead << endPoint << arrowHead1 << arrowHead2;
    painter.drawPolygon(arrowHead);
}

char Arrow::getTypeCode() const {
    return 'A';
}

void Arrow::updatePosition() {
    calculatePoints();

    if (!fromObject || !toObject) {
        pos_ = QPoint(0, 0);
        size_ = QSize(0, 0);
        return;
    }

    int minX = std::min(startPoint.x(), endPoint.x());
    int minY = std::min(startPoint.y(), endPoint.y());
    int maxX = std::max(startPoint.x(), endPoint.x());
    int maxY = std::max(startPoint.y(), endPoint.y());

    pos_ = QPoint(minX, minY);
    size_ = QSize(maxX - minX, maxY - minY);

}

void Arrow::disconnect() {
    if (fromObject) {
        fromObject->removeObserver(this);
        fromObject = nullptr;
    }
    if (toObject) {
        toObject->removeObserver(this);
        toObject = nullptr;
    }
}

void Arrow::save(FILE* file) {
    fprintf(file, "%c\n", getTypeCode());
}
