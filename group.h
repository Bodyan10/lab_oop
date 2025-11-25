#ifndef GROUP_H
#define GROUP_H
#include <shape.h>
#include <utility>
#include <vector>

class Group : public Shape {
public:
    Group();
    Group(QPoint coordinates, QSize size, QColor color, bool selected);
    ~Group();

    // Group operations
    void addShape(Shape* shape);
    void removeShape(Shape* shape);
    void updateRelativeCoordinates();
    void updateGroupBounds();
    std::vector<Shape*> getShapes() const;

    // Shape operations
    void move(int x, int y) override;
    void resize(int x, int y) override;
    void changeColor(QColor color) override;

    bool hasPointIn(QPoint point) const override;
    void draw(QPainter &painter) const override;

    void setSelected(bool selected) override;
    bool isSelected() const override;

    bool canMove(const QRect& widgetBounds, int diffx, int diffy) override;

    void load(FILE* file, ShapeFactory* factory = nullptr) override;
    void save(FILE* file) override;

    char getTypeCode() const override;

private:
    std::vector<Shape*> children_;
    std::vector<std::pair<QPointF, QSizeF>> relativeCoordinates_; // Относительные координаты

    void applyRelativeCoordinates(); // Применяем относительные координаты
};

#endif // GROUP_H
