#ifndef CLINE_H
#define CLINE_H
#include "myshape.h"
#include <QColor>

class CLine : public MyShape
{
public:
    CLine(QPoint p1_, QPoint p2_, QColor bc_, QColor fc_ );
    virtual ~CLine();


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
        return "CLine";
    }
private:
    QPoint _p1,_p2;
    QColor _bc, _fc;
};

#endif // CLINE_H
