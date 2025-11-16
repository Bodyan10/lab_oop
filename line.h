#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Line : public Shape {
public:
    Line(QPoint coordinates, QSize size, QColor color, bool selected, std::string name, int lineWidth);
    Line(const Line& other);
    Line& operator=(const Line& other);
    ~Line();

    void draw(QPainter &painter) const override;
    bool adjustToFitBounds(const QRect& widgetBounds) override;

private:
    int lineWidth_ = 2;
};

#endif // LINE_H
