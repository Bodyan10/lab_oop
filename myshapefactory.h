#ifndef MYSHAPEFACTORY_H
#define MYSHAPEFACTORY_H
#include <ShapeFactory.h>
#include "myshape.h"

class MyShapeFactory : public ShapeFactory {
public:
    MyShape* createShape(const char&) override;
};

#endif // MYSHAPEFACTORY_H
