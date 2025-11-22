#include "group.h"
#include <shape.h>

Group::Group(){
};

Group::Group(QPoint coordinates, QSize size, QColor color, bool selected) : Shape(coordinates, size, color, selected) {
};

void Group::addShape(Shape* shape) {
    groupShapes.push_back(shape);
    QRect rect_group;
    rect_group = getBounds().united(shape->getBounds());
    pos_ = rect_group.topLeft();
    size_ = rect_group.size();
};

void Group::move(int x, int y) {
    for (Shape* shape: groupShapes) {
        shape->move(x, y);
    }
};

void Group::resize(int x, int y) {
    for (Shape* shape: groupShapes) {
        shape->resize(x, y);
    }
};

void Group::changeColor(QColor color) {
    for (Shape* shape: groupShapes) {
        shape->changeColor(color);
    }
};

bool Group::hasPointIn(QPoint point) const {
    for (Shape* shape: groupShapes) {
        if (!shape->hasPointIn(point)) {
            return false;
        }
    }
    return true;
};

void Group::draw(QPainter &painter) const {
    for (Shape* shape: groupShapes) {
        shape->draw(painter);
    }
};

void Group::setSelected(bool selected) {
    for (Shape* shape: groupShapes) {
        shape->setSelected(selected);
    }
};

bool Group::isSelected() const {
    for (Shape* shape: groupShapes) {
        if (!shape->isSelected()) {
            return false;
        }
    }
    return true;
};

bool Group::adjustToFitBounds(const QRect& widgetBounds) {
    for (Shape* shape: groupShapes) {
        if (!shape->adjustToFitBounds(widgetBounds)) {
            return false;
        }
    }
    return true;
};

void Group::load(std::string filename) {
    for (Shape* shape: groupShapes) {
        shape->load(filename);
    }
};

void Group::save(std::string filename) {
    for (Shape* shape: groupShapes) {
        shape->save(filename);
    }
};

Group::~Group() {
    for (Shape* shape: groupShapes) {
        if (shape) {
            delete shape;
        }
    }
};









