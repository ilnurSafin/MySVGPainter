#include "cline.h"

CLine::CLine(QPoint p1_, QPoint p2_, QColor bc_, QColor fc_)
    :_p1(p1_), _p2(p2_), _bc(bc_), _fc(fc_)
{

}

CLine::~CLine()
{

}

void CLine::draw(QPainter &painter)
{
    painter.setPen(_bc);
    painter.setBrush(_fc);
    painter.drawLine(_p1,_p2);
}

void CLine::draw(QPainter &painter, QColor bc)
{
    painter.setPen(bc);
    painter.setBrush(_fc);
    painter.drawLine(_p1,_p2);
}

void CLine::draw(QPainter &painter, QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    painter.setPen(bc);
    painter.setBrush(fc);
    painter.drawLine(p1,p2);
}

void CLine::move(int dx, int dy)
{
    _p1.setX(_p1.x() + dx);
    _p2.setX(_p2.x() + dx);

    _p1.setY(_p1.y() + dy);
    _p2.setY(_p2.y() + dy);
}

MyShape *CLine::clone(QPoint p1, QPoint p2, QColor bc, QColor fc)
{
    return new CLine(p1,p2,bc,fc);
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
QRect CLine::getRect()
{
    QPoint p1=_p1, p2=_p2;
    normal_points(p1,p2);
    return QRect(p1.x(),p1.y(), p2.x()-p1.x() ,p2.y()-p1.y() );
}

bool CLine::isInShape(QPoint p)
{
    QPoint p1, p2;
    if(_p1.x() <= _p2.x())
    {
        p1 = _p1;
        p2 = _p2;
    }
    else
    {
        p2 = _p1;
        p1 = _p2;
    }
    double k = (p2.y()-p1.y())/double(p2.x()-p1.x());
    double b = p1.y() - p1.x()*k;

    if(p.x() >= p1.x() && p.x() <= p2.x() && abs(p.x()*k + b - p.y())<=3)
        return true;
    else
        return false;
}

void CLine::resizeTop(int dy)
{
    QPoint &top_p = _p1.y()<_p2.y() ? _p1 : _p2;
    QPoint &bottom_p = _p1.y()>=_p2.y() ? _p1 : _p2;

    if(top_p.y() + dy < bottom_p.y())
        top_p.setY(top_p.y() + dy);
}

void CLine::resizeBottom(int dy)
{
    QPoint &top_p = _p1.y()<_p2.y() ? _p1 : _p2;
    QPoint &bottom_p = _p1.y()>=_p2.y() ? _p1 : _p2;

    if(bottom_p.y() + dy > top_p.y())
        bottom_p.setY(bottom_p.y() + dy);
}

void CLine::resizeLeft(int dx)
{
    QPoint &left_p = _p1.x()<_p2.x() ? _p1 : _p2;
    QPoint &right_p = _p1.x()>=_p2.x() ? _p1 : _p2;

    if(left_p.x() + dx < right_p.x())
        left_p.setX(left_p.x() + dx);
}

void CLine::resizeRight(int dx)
{
    QPoint &left_p = _p1.x()<_p2.x() ? _p1 : _p2;
    QPoint &right_p = _p1.x()>=_p2.x() ? _p1 : _p2;

    if(right_p.x() + dx > left_p.x())
        right_p.setX(right_p.x() + dx);
}

void CLine::load(std::ifstream &f)
{
    int t;
    f >> t;
    _p1.setX(t);
    f >> t;
    _p1.setY(t);
    f >> t;
    _p2.setX(t);
    f >> t;
    _p2.setY(t);

    QColor tc;
    f >> tc;
    _bc = tc;

    f >> tc;
    _fc = tc;
}

void CLine::save(std::ofstream &f)
{
    f << "CLine" << '\n' << _p1.x() << ' ' << _p1.y() << ' ' << _p2.x() << ' ' << _p2.y() << ' ' << _bc << ' ' << _fc << '\n';
}

