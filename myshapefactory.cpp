#include "myshapefactory.h"
#include <shape.h>
#include <rectangle.h>
#include <ellipse.h>
#include <triangle.h>
#include <line.h>

Shape* MyShapeFactory::createShape(const char& code) {
    Shape* shape = nullptr;
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
        default:;
    }
    return shape;
}
