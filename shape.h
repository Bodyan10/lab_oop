#ifndef SHAPE_H
#define SHAPE_H

#include <QColor>
#include <QRect>
#include <QPoint>
#include <string>
#include <QPainter>

class Shape
{
public:
    Shape();
    virtual ~Shape();

    virtual void draw(QPainter* painter) const = 0;
    virtual void move(const QPoint& delta) = 0;
    virtual void resize(const QPoint& startPoint, const QPoint& endPoint) = 0;
    virtual bool contains(const QPoint& point) const = 0;
    virtual QRect boundingRect() const = 0;
    virtual Shape* clone() const = 0;

    // Новый метод для создания фигуры по двум точкам
    virtual void createFromPoints(const QPoint& startPoint, const QPoint& endPoint) = 0;

    void setColor(const QColor& color);
    bool isOutOfBounds(const QRect& workspaceBounds) const;
    void setSelected(bool selected);

    QColor color() const;
    bool isSelected() const;
    std::string name() const;

protected:
    QColor color_ = Qt::blue;
    bool isSelected_ = false;
    std::string name_;
};

#endif // SHAPE_H
