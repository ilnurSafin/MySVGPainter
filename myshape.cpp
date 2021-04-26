#include "myshape.h"

MyShape::~MyShape()
{

}

std::basic_istream<char>& operator>>(std::basic_istream<char>& f, QColor& c)
{
    int t;
    f >> t;
    c.setRed(t);
    f >> t;
    c.setGreen(t);
    f >> t;
    c.setBlue(t);
    f >> t;
    c.setAlpha(t);

    return f;
}
std::basic_ostream<char>& operator<<(std::basic_ostream<char>& f, const QColor& c)
{
    f << ' ';
    f << c.red();
    f << ' ';
    f << c.green();
    f << ' ';
    f << c.blue();
    f << ' ';
    f << c.alpha();
    f << ' ';

    return f;
}
