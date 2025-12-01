#ifndef MYSHAPE_H
#define MYSHAPE_H

#include <QPoint>
#include <QColor>
#include <QRect>
#include <QSize>
#include <QPainter>
#include <ShapeFactory.h>
#include <observablemixin.h>

class MyShape : public ObservableMixin {
public:
    MyShape();
    MyShape(QPoint coordinates, QSize size, QColor color, bool selected);
    virtual ~MyShape();

    // Базовые операции Composite
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

    virtual bool adjustToFitBounds(const QRect& widgetBounds);
    virtual bool canMove(const QRect& widgetBounds, int diffx, int diffy);
    virtual bool canMoveAndResize(const QRect& widgetBounds, const QPoint& new_pos, const QSize& new_size);

    virtual void load(FILE* file, ShapeFactory* factory);
    virtual void save(FILE* file);

    virtual char getTypeCode() const = 0;
    void setParent(MyShape* parent);
    MyShape* getParent();


protected:
    MyShape* parent_ = nullptr;
    QPoint pos_;
    QSize size_;
    QColor color_;
    bool isSelected_ = false;
};

#endif // MYSHAPE_H
