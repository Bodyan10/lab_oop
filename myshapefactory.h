#ifndef MYSHAPEFACTORY_H
#define MYSHAPEFACTORY_H
#include <ShapeFactory.h>
#include <shape.h>

class MyShapeFactory : public ShapeFactory {
public:
    Shape* createShape(const char&) override;
};

#endif // MYSHAPEFACTORY_H
