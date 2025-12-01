
#ifndef ARROW_H
#define ARROW_H

#include "myshape.h"

class Arrow : public MyShape, public Observer {
public:
    Arrow(MyShape* from = nullptr, MyShape* to = nullptr, QColor color = Qt::red);
    ~Arrow();

    void draw(QPainter &painter) const override;
    char getTypeCode() const override;

    void move(int x, int y) override { Q_UNUSED(x); Q_UNUSED(y); }
    void resize(int x, int y) override { Q_UNUSED(x); Q_UNUSED(y); }

    void updatePosition();

    MyShape* getFrom() const { return fromObject; }
    MyShape* getTo() const { return toObject; }

    // Observer method
    void onSubjectChanged() override;

    // Для передачи границ виджета
    void setWidgetBounds(const QRect& bounds) { widgetBounds = bounds; }

    void disconnect();
    bool isConnected() const { return fromObject != nullptr && toObject != nullptr; }
    bool usesObject(MyShape* obj) const { return fromObject == obj || toObject == obj; }

    void save(FILE* file) override;
    void load(FILE* file, ShapeFactory* factory = nullptr) override {Q_UNUSED(file); Q_UNUSED(factory);};

private:
    MyShape* fromObject;
    MyShape* toObject;
    QPointF startPoint;
    QPointF endPoint;
    QPointF arrowHead1;
    QPointF arrowHead2;
    QRect widgetBounds;

    QPoint lastFromPos; // Для отслеживания перемещения

    void calculatePoints();
    QPointF findBorderPoint(MyShape* shape, const QPointF& target) const;
};
#endif // ARROW_H
