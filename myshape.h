#ifndef MYSHAPE_H
#define MYSHAPE_H
#include <QPainter>
#include <QPoint>
#include <QColor>
#include <string>
#include <fstream>
#include <QVariant>

class IResizeable{
public:
    virtual void resizeTop(int dy) = 0;
    virtual void resizeBottom(int dy) = 0;

    virtual void resizeLeft(int dx) = 0;
    virtual void resizeRight(int dx) = 0;
};

class ILoadableAndSaveable{
public:
    virtual void load(std::ifstream& f) = 0;
    virtual void save(std::ofstream& f) = 0;
};

class MyShape : public IResizeable, public ILoadableAndSaveable
{
public:
    virtual ~MyShape()=0;

    virtual void draw(QPainter&) = 0;
    virtual void draw(QPainter&, QColor bc) = 0;
    virtual void draw(QPainter &, QPoint p1, QPoint p2, QColor bc, QColor fc) = 0;

    virtual bool isInShape(QPoint) = 0;
    virtual void move(int dx, int dy) = 0;

    virtual MyShape* clone(QPoint p1, QPoint p2, QColor bc, QColor fc) = 0;

    virtual QRect getRect() = 0;

    virtual void setPenColor(QColor c) = 0;
    virtual void setBrushColor(QColor c) = 0;

    virtual QString name() = 0;
};

std::basic_istream<char>& operator>>(std::basic_istream<char>& f, QColor& c);
std::basic_ostream<char>& operator<<(std::basic_ostream<char>& f, const QColor& c);


#endif // MYSHAPE_H
