#include "myshapefactory.h"
#include "myshape.h"
#include "rectangle.h"
#include "ellipse.h"
#include "triangle.h"
#include "line.h"
#include <arrow.h>>
#include "group.h"

MyShape* MyShapeFactory::createShape(const char& code) {
    MyShape* shape = nullptr;
    switch(code) {
    case 'R':
        shape = new Rectangle();
        break;
    case 'T':
        shape = new Triangle();
        break;
    case 'E':
        shape = new Ellipse();
        break;
    case 'L':
        shape = new Line();
        break;
    case 'G':
        shape = new Group();
    default:;
    }
    return shape;
}
