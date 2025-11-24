#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <QColor>
#include <QRect>
#include <QSize>
#include <QPainter>
#include <ShapeFactory.h>
#include <vector>

class Shape {
public:
    Shape();
    Shape(QPoint coordinates, QSize size, QColor color, bool selected);
    virtual ~Shape();

    // Базовые операции Composite
    virtual void move(int x, int y);
    virtual void resize(int x, int y);
    virtual void changeColor(QColor color);

    // Операции для Composite pattern
    virtual void addShape(Shape* shape) { Q_UNUSED(shape); } // По умолчанию ничего не делаем
    virtual void removeShape(Shape* shape) { Q_UNUSED(shape); }
    virtual std::vector<Shape*> getShapes() const { return std::vector<Shape*>(); }
    virtual bool isGroup() const { return false; }

    QSize getSize() const;
    QPoint getPos() const;
    QPoint getCentralPos() const;
    QColor getColor() const;
    virtual QRect getBounds() const;

    virtual bool hasPointIn(QPoint point) const;
    virtual void draw(QPainter &painter) const = 0;

    virtual void setSelected(bool selected);
    virtual bool isSelected() const;

    virtual bool adjustToFitBounds(const QRect& widgetBounds);
    virtual bool canMove(const QRect& widgetBounds, int diffx, int diffy);
    virtual bool canMoveAndResize(const QRect& widgetBounds, const QPoint& new_pos, const QSize& new_size);

    virtual void load(FILE* file, ShapeFactory* factory = nullptr);
    virtual void save(FILE* file);

    virtual char getTypeCode() const = 0;

protected:
    QPoint pos_;
    QSize size_;
    QColor color_;
    bool isSelected_ = false;
};

#endif // SHAPE_H
