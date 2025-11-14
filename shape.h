#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <QColor>
#include <QRect>
#include <QSize>
#include <QPainter>
#include <string>

enum class ShapeType {
    Rectangle = 0,
    Circle = 1,
    Triangle = 2,
    Line = 3,
    None = 100
};

class Shape {
public:
    Shape(QPoint coordinates = QPoint(), QSize size = QSize(), QColor color = Qt::blue);
    virtual ~Shape();

    virtual void move(int x, int y);
    virtual void resize(int x, int y);
    virtual void changeColor(QColor color);

    virtual ShapeType type() const { return ShapeType::None; }

    QSize getSize() const;
    QPoint getPos() const;
    QPoint getCentralPos() const;
    QColor getColor() const;
    virtual QRect getBounds() const;

    virtual bool hasPointIn(QPoint point) const = 0;
    virtual void draw(QPainter &painter) const = 0;

    void setSelected(bool selected);
    bool isSelected() const;
    std::string name() const;

    bool checkBounds(const QRect& widgetBounds);
    virtual void adjustToFitBounds(const QRect& widgetBounds);

protected:
    QPoint pos_;
    QSize size_;
    QColor color_;
    bool isSelected_ = false;
    std::string name_;
};

#endif // SHAPE_H
