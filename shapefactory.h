#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H
#include "myshape.h"

#include <string>
#include "MyStorage.h"

class ShapeFactory{
public:
      static MyShape* createShape(std::string code);
      //static MyStorage<MyShape*
};

#endif // SHAPEFACTORY_H
