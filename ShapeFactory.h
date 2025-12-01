#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

class MyShape;

class ShapeFactory {
public:
    virtual MyShape* createShape(const char&) = 0;
    virtual ~ShapeFactory() = default;
};

#endif // SHAPEFACTORY_H
