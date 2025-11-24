#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Line : public Shape {
public:
    Line();
    Line(QPoint coordinates, QSize size, QColor color, bool selected, int lineWidth);
    Line(const Line& other);
    Line& operator=(const Line& other);
    ~Line();

    void draw(QPainter &painter) const override;
    bool adjustToFitBounds(const QRect& widgetBounds) override;
    char getTypeCode() const override;
    void save(FILE* file) override;
    void load(FILE* file, ShapeFactory* factory = nullptr) override;

private:
    int lineWidth_ = 2;
};

#endif // LINE_H
