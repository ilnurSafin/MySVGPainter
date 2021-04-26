#include <cmath>
#include "crectangle.h"


CRectangle::CRectangle(int x_, int y_, int a_, int b_, QColor bc_, QColor fc_)
    :_x(x_), _y(y_), _a(a_),_b(b_),_bc(bc_),_fc(fc_)
{

}

CRectangle::~CRectangle()
{

}

void CRectangle::draw(QPainter &painter)
{
    painter.setPen(_bc);
    painter.setBrush(_fc);
    painter.drawRect(_x,_y,_a,_b);
}

void CRectangle::draw(QPainter &painter, QColor bc)
{
    painter.setPen(bc);
    painter.setBrush(_fc);
    painter.drawRect(_x,_y,_a,_b);
}

static void normal_points(QPoint& p1, QPoint& p2)
{
    QPoint rp1, rp2;

    rp1.setX(std::min(p1.x(), p2.x()));
    rp1.setY(std::min(p1.y(), p2.y()));

    rp2.setX(std::max(p1.x(), p2.x()));
    rp2.setY(std::max(p1.y(), p2.y()));
    p1 = rp1;
    p2 = rp2;
}


void CRectangle::draw(QPainter &painter, QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    painter.setPen(bc);
    painter.setBrush(fc);
    normal_points(p1,p2);
    painter.drawRect(p1.x(),p1.y(), p2.x()-p1.x(), p2.y()-p1.y());
}

void CRectangle::move(int dx, int dy)
{
    _x += dx;
    _y += dy;
}

MyShape *CRectangle::clone(QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    normal_points(p1,p2);
    return new CRectangle(p1.x(),p1.y(), p2.x()-p1.x(), p2.y()-p1.y(), bc,fc);
}

QRect CRectangle::getRect()
{
    return QRect(_x ,_y ,_a,_b);
}

bool CRectangle::isInShape(QPoint p)
{
    if(p.x() >= _x && p.x() <= (_x+_a) && p.y() >= _y && p.y() <= (_y+_b))
        return true;
    else
        return false;
}

void CRectangle::resizeTop(int dy)
{
    if(_b - dy >= 0)
    {
        _y += dy;
        _b -= dy;
    }
}

void CRectangle::resizeBottom(int dy)
{
    if(_b + dy >= 0)
        _b += dy;
}

void CRectangle::resizeLeft(int dx)
{
    if(_a - dx >= 0)
    {
        _x += dx;
        _a -= dx;
    }
}

void CRectangle::resizeRight(int dx)
{
    if(_a + dx >= 0)
        _a += dx;
}

void CRectangle::load(std::ifstream &f)
{
    f >> _x >> _y >> _a >> _b >> _bc >> _fc;
}

void CRectangle::save(std::ofstream &f)
{
    f << "CRectangle" << '\n' << _x << ' ' << _y << ' ' << _a << ' ' << _b << ' ' << _bc << ' ' << _fc << '\n';
}

