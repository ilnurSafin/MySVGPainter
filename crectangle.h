#ifndef CRECTANGLE_H
#define CRECTANGLE_H
#include "myshape.h"
#include <QColor>

class CRectangle : public MyShape
{
public:
    CRectangle(int x_, int y_, int a_, int b_, QColor bc_, QColor fc_);
    virtual ~CRectangle();

    void draw(QPainter& painter);
    void draw(QPainter& painter, QColor bc);
    void draw(QPainter & painter, QPoint p1, QPoint p2, QColor bc, QColor fc);
    void move(int dx, int dy);

    MyShape *clone(QPoint p1, QPoint p2, QColor bc, QColor fc);

    QRect getRect();
    bool isInShape(QPoint p);

    void setPenColor(QColor c){ _bc = c; }
    void setBrushColor(QColor c){ _fc = c; }

    void resizeTop(int dy);
    void resizeBottom(int dy);

    void resizeLeft(int dx);
    void resizeRight(int dx);

    void load(std::ifstream& f);
    void save(std::ofstream& f);

    QString name(){
        return "CRectangle";
    }
private:
    int _x, _y, _a,_b;
    QColor _bc, _fc;
};

#endif // CRECTANGLE_H
