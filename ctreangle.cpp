#include <cmath>
#include "ctreangle.h"
/*QPoint points[3];

    points[0].setX(_x);
    points[0].setY(_y+_b);
    points[1].setX(_x + _a/2);
    points[1].setY(_y);
    points[2].setX(_x + _a);
    points[2].setY(_y+_b);

    painter.setPen(_bc);
    painter.setBrush(_fc);
    painter.drawPolygon(points);*/

CTreangle::CTreangle(int x_, int y_, int a_, int b_, QColor bc_, QColor fc_)
    :_x(x_), _y(y_), _a(a_),_b(b_),_bc(bc_),_fc(fc_)
{

}

CTreangle::~CTreangle()
{

}

void CTreangle::draw(QPainter &painter)
{
    QPoint points[3];

    points[0].setX(_x);
    points[0].setY(_y+_b);
    points[1].setX(_x + _a/2);
    points[1].setY(_y);
    points[2].setX(_x + _a);
    points[2].setY(_y+_b);

    painter.setPen(_bc);
    painter.setBrush(_fc);
    painter.drawPolygon(points,3);
}

void CTreangle::draw(QPainter &painter, QColor bc)
{
    QPoint points[3];

    points[0].setX(_x);
    points[0].setY(_y+_b);
    points[1].setX(_x + _a/2);
    points[1].setY(_y);
    points[2].setX(_x + _a);
    points[2].setY(_y+_b);

    painter.setPen(bc);
    painter.setBrush(_fc);
    painter.drawPolygon(points,3);
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
void CTreangle::draw(QPainter &painter, QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    QPoint points[3];
    normal_points(p1,p2);

    points[0].setX(p1.x());
    points[0].setY(p1.y()+p2.y()-p1.y());
    points[1].setX(p1.x() + (p2.x()-p1.x())/2);
    points[1].setY(p1.y());
    points[2].setX(p1.x() + (p2.x()-p1.x()));
    points[2].setY(p1.y()+p2.y()-p1.y());

    painter.setPen(bc);
    painter.setBrush(fc);
    painter.drawPolygon(points,3);
}

void CTreangle::move(int dx, int dy)
{
    _x += dx;
    _y += dy;
}

MyShape *CTreangle::clone(QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    normal_points(p1,p2);
    return new CTreangle(p1.x(),p1.y(), p2.x()-p1.x(), p2.y()-p1.y(), bc,fc);
}

QRect CTreangle::getRect()
{
    return QRect(_x ,_y ,_a ,_b );
}

bool CTreangle::isInShape(QPoint p)
{
    double k = -_b/(_a/2.0);
    double b = (_y+_b) - _x*k;
    if(p.x() >= _x + _a/2.0)
    {
        k *= -1;
        b =  _y - (_x + _a/2.0)*k;;
    }
    if(p.x() >= _x && p.x() <= (_x+_a) && p.y() >= _y && p.y() <= (_y+_b) && p.y()>=(p.x()*k + b))
        return true;
    else
        return false;
}

void CTreangle::resizeTop(int dy)
{
    if(_b - dy >= 0)
    {
        _y += dy;
        _b -= dy;
    }
}

void CTreangle::resizeBottom(int dy)
{
    if(_b + dy >= 0)
        _b += dy;
}

void CTreangle::resizeLeft(int dx)
{
    if(_a - dx >= 0)
    {
        _x += dx;
        _a -= dx;
    }
}

void CTreangle::resizeRight(int dx)
{
    if(_a + dx >= 0)
        _a += dx;
}

void CTreangle::load(std::ifstream &f)
{
    f >> _x >> _y >> _a >> _b >> _bc >> _fc;
}

void CTreangle::save(std::ofstream &f)
{
    f << "CTreangle" << '\n' << _x << ' ' << _y << ' ' << _a << ' ' << _b << ' ' << _bc << ' ' << _fc << '\n';
}
