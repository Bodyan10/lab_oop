#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Line : public Shape {
public:
    Line(QPoint coordinates = QPoint(), QSize size = QSize(), QColor color = Qt::blue);
    Line(const Line& other);
    Line& operator=(const Line& other);
    ~Line();

    ShapeType type() const override { return ShapeType::Line; }
    bool hasPointIn(QPoint point) const override;
    void draw(QPainter &painter) const override;
    void adjustToFitBounds(const QRect& widgetBounds) override;

private:
    int lineWidth_ = 2;
};

#endif // LINE_H
