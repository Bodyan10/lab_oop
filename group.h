#ifndef GROUP_H
#define GROUP_H
#include <shape.h>

class Group : public Shape {
public:
    Group();
    Group(QPoint coordinates, QSize size, QColor color, bool selected);

    void addShape(Shape*);

    void move(int x, int y) override;
    void resize(int x, int y) override;
    void changeColor(QColor color) override;

    bool hasPointIn(QPoint point) const override;
    void draw(QPainter &painter) const override;

    void setSelected(bool selected) override;
    bool isSelected() const override;

    bool adjustToFitBounds(const QRect& widgetBounds) override;

    void load(std::string filename) override; //Надо написать эти два метода
    void save(std::string filename) override;

    ~Group();
private:
    std::vector<Shape*> groupShapes;
};

#endif // GROUP_H
