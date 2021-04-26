#include "CEllipse.h"

/*CEllipse::CEllipse(int x_center_, int y_center_, int a_, int b_, QColor bc_, QColor fc_)
    :_x(x_center_), _y(y_center_), _a(a_), _b(b_), _bc(bc_),_fc(fc_)
{

}

CEllipse::~CEllipse()
{

}

bool CEllipse::isInShape(QPoint p)
{
    if((p.x()-_x)*(p.x()-_x)/double(_a*_a) + (p.y()-_y)*(p.y()-_y)/double(_b*_b) <= 1)
        return true;
    else
        return false;
}

void CEllipse::draw(QPainter &painter)
{
    painter.setPen(_bc);
    painter.setBrush(_fc);
    painter.drawEllipse(_x-_a,_y-_b,_a*2,_b*2);
}

void CEllipse::draw(QPainter &painter, QColor bc)
{
    painter.setPen(bc);
    painter.setBrush(_fc);
    painter.drawEllipse(_x-_a,_y-_b,_a*2,_b*2);
}

void CEllipse::draw(QPainter &painter, QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    painter.setPen(bc);
    painter.setBrush(fc);
    painter.drawEllipse(p1.x(),p1.y(),
                        p2.x()-p1.x(),p2.y()-p1.y());
}

void CEllipse::move(int dx, int dy)
{
    _x += dx;
    _y += dy;
}

MyShape *CEllipse::clone(QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    QPoint diff = p2-p1;
    int a = diff.x()/2;
    int b = diff.y()/2;
    return new CEllipse(p1.x()+a,p1.y()+b,
                        a,b,
                        bc,
                        fc);
}

QRect CEllipse::getRect()
{
    //if(_a >= _b)
        return QRect(_x - _a  ,_y - _b , 2*_a , 2*_b );
    //else
        //return QRectF(_x - _a - 3 ,_y - _b - 3, 2*_a + 6, 2*_b + 6);
}*/

CEllipse::CEllipse(int x_, int y_, int a_, int b_, QColor bc_, QColor fc_)
    :_x(x_), _y(y_), _a(a_),_b(b_),_bc(bc_),_fc(fc_)
{

}

CEllipse::~CEllipse()
{

}

void CEllipse::draw(QPainter &painter)
{
    painter.setPen(_bc);
    painter.setBrush(_fc);
    painter.drawEllipse(_x,_y,_a,_b);
}

void CEllipse::draw(QPainter &painter, QColor bc)
{
    painter.setPen(bc);
    painter.setBrush(_fc);
    painter.drawEllipse(_x,_y,_a,_b);
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


void CEllipse::draw(QPainter &painter, QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    painter.setPen(bc);
    painter.setBrush(fc);
    normal_points(p1,p2);
    painter.drawEllipse(p1.x(),p1.y(), p2.x()-p1.x(), p2.y()-p1.y());
}

void CEllipse::move(int dx, int dy)
{
    _x += dx;
    _y += dy;
}

MyShape *CEllipse::clone(QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    normal_points(p1,p2);
    return new CEllipse(p1.x(),p1.y(), p2.x()-p1.x(), p2.y()-p1.y(), bc,fc);
}

QRect CEllipse::getRect()
{
    return QRect(_x,_y,_a,_b);
}

bool CEllipse::isInShape(QPoint p)
{
    int x_center = _x + _a/2.0;
    int y_center = _y + _b/2.0;

    if(     (p.x()-x_center)*(p.x()-x_center)/double(_a*_a/4.0)
            +
            (p.y()-y_center)*(p.y()-y_center)/double(_b*_b/4.0)
            <= 1)
        return true;
    else
        return false;
}

void CEllipse::resizeTop(int dy)
{
    if(_b - dy >= 0)
    {
        _y += dy;
        _b -= dy;
    }
}

void CEllipse::resizeBottom(int dy)
{
    if(_b + dy >= 0)
        _b += dy;
}

void CEllipse::resizeLeft(int dx)
{
    if(_a - dx >= 0)
    {
        _x += dx;
        _a -= dx;
    }
}

void CEllipse::resizeRight(int dx)
{
    if(_a + dx >= 0)
        _a += dx;
}

void CEllipse::load(std::ifstream &f)
{
    f >> _x >> _y >> _a >> _b >> _bc >> _fc;
}

void CEllipse::save(std::ofstream &f)
{
    f << "CEllipse" << '\n' << _x << ' ' << _y << ' ' << _a << ' ' << _b << ' ' << _bc << ' ' << _fc << '\n';
}

