#ifndef SHAPE_H
#define SHAPE_H

#include <QPoint>
#include <QColor>
#include <QRect>
#include <QSize>
#include <QPainter>
#include <string>

class Shape {
public:
    Shape();
    Shape(QPoint coordinates, QSize size, QColor color, bool selected);
    virtual ~Shape();

    virtual void move(int x, int y);
    virtual void resize(int x, int y);
    virtual void changeColor(QColor color);

    QSize getSize() const;
    QPoint getPos() const;
    QPoint getCentralPos() const;
    QColor getColor() const;
    virtual QRect getBounds() const;

    virtual bool hasPointIn(QPoint point) const;
    virtual void draw(QPainter &painter) const = 0;

    virtual void setSelected(bool selected);
    virtual bool isSelected() const;
    std::string name() const;

    virtual bool adjustToFitBounds(const QRect& widgetBounds);
    bool canMove(const QRect& widgetBounds, int diffx, int diffy);
    bool canMoveAndResize(const QRect& widgetBounds, const QPoint& new_pos, const QSize& new_size);

    virtual void load(std::string filename); //Надо написать эти два метода
    virtual void save(std::string filename);

protected:
    QPoint pos_;
    QSize size_;
    QColor color_;
    bool isSelected_ = false;
    std::string name_;
};

#endif // SHAPE_H
