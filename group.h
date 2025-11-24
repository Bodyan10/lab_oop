#ifndef GROUP_H
#define GROUP_H
#include <shape.h>
#include <utility>
#include <vector>

class Group : public Shape {
public:
    Group();
    Group(QPoint coordinates, QSize size, QColor color, bool selected);
    Group(const Group& other);
    Group& operator=(const Group& other);
    ~Group();

    // Composite operations
    void addShape(Shape* shape) override;
    void removeShape(Shape* shape) override;
    std::vector<Shape*> getShapes() const override;
    bool isGroup() const override { return true; }

    // Shape operations
    void move(int x, int y) override;
    void resize(int x, int y) override;
    void changeColor(QColor color) override;

    bool hasPointIn(QPoint point) const override;
    void updateGroupBounds();
    void updateRelativeCoordinates(); // Новый метод
    void draw(QPainter &painter) const override;

    void setSelected(bool selected) override;
    bool isSelected() const override;

    bool adjustToFitBounds(const QRect& widgetBounds) override;
    bool canMove(const QRect& widgetBounds, int diffx, int diffy) override;
    bool canMoveAndResize(const QRect& widgetBounds, const QPoint& new_pos, const QSize& new_size) override;

    void load(FILE* file, ShapeFactory* factory = nullptr) override;
    void save(FILE* file) override;

    char getTypeCode() const override;

private:
    std::vector<Shape*> children_;
    std::vector<std::pair<QPointF, QSizeF>> relativeCoordinates_; // Относительные координаты

    void applyRelativeCoordinates(); // Применяем относительные координаты
};

#endif // GROUP_H
