#include "shapefactory.h"
#include "cellipse.h"
#include "crectangle.h"
#include "ctreangle.h"
#include "cline.h"
#include "group_shape.h"
#include <QPoint>
#include <QColor>

MyShape* ShapeFactory::createShape(std::string code)
{
    if(code == "CEllipse")
        return new CEllipse(0,0,0,0,QColor(),QColor());
    else if(code == "CRectangle")
        return new CRectangle(0,0,0,0,QColor(),QColor());
    else if(code == "CTreangle")
        return new CTreangle(0,0,0,0,QColor(),QColor());
    else if(code == "CLine")
        return new CLine(QPoint(), QPoint() ,QColor(),QColor());
    else if(code == "Group")
        return new Group();
    else
        return nullptr;
}
