#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H
#include <shape.h>


class ShapeFactory {
public:
    virtual Shape* createShape(const char&) = 0;
    virtual ~ShapeFactory() = default;
};


#endif // SHAPEFACTORY_H
