#ifndef DECORATOR_SELECTED_H
#define DECORATOR_SELECTED_H
#include "myshape.h"

static const int BORDER = 5;

class SelectedShape : public MyShape
{
public:
    enum Place{Outside, Inside, TopLeft, TopRight, BottomLeft, BottomRight, Bottom, Top, Left, Right};
    SelectedShape(MyShape* sh)
    {
        _sh = sh;
        need_delete = true;
    }

    virtual ~SelectedShape(){
        if(need_delete)
            delete _sh;
    }

    virtual void draw(QPainter& p)
    {
        _sh->draw(p);
        QPen pen;
        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        p.setPen(pen);
        p.setBrush(Qt::transparent);
        QRect r = _sh->getRect();
        r.setX(r.x() - BORDER);
        r.setY(r.y() - BORDER);
        r.setHeight(r.height() + BORDER);
        r.setWidth(r.width()   + BORDER);
        p.drawRect(r);
    }
    virtual void draw(QPainter&p, QColor bc)
    {
        _sh->draw(p,bc);
        QPen pen;
        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        p.setPen(pen);
        p.setBrush(Qt::transparent);
        QRect r = _sh->getRect();
        r.setX(r.x() - BORDER);
        r.setY(r.y() - BORDER);
        r.setHeight(r.height() + BORDER);
        r.setWidth(r.width()   + BORDER);
        p.drawRect(r);
    }
    virtual void draw(QPainter &p, QPoint p1, QPoint p2, QColor bc, QColor fc)
    {
        _sh->draw(p,p1,p2,bc,fc);
        QPen pen;
        pen.setColor(Qt::black);
        pen.setStyle(Qt::DashLine);
        p.setPen(pen);
        p.setBrush(Qt::transparent);
        QRect r = _sh->getRect();
        r.setX(r.x() - BORDER);
        r.setY(r.y() - BORDER);
        r.setHeight(r.height() + BORDER);
        r.setWidth(r.width()   + BORDER);
        p.drawRect(r);
    }

    virtual bool isInShape(QPoint p)
    {
        //return _sh->isInShape(p);

        QRect r = _sh->getRect();
        if(p.x() >= r.x() && p.x() <= (r.x()+r.width()) && p.y() >= r.y() && p.y() <= (r.y()+r.height()))
            return true;
        else
            return false;
    }
    bool         isInBorder(QPoint p)
    {
        QRect r = _sh->getRect();
        r.setX(r.x() - BORDER);
        r.setY(r.y() - BORDER);
        r.setHeight(r.height() + BORDER);
        r.setWidth(r.width()   + BORDER);
        if(p.x() >= r.x() && p.x() <= (r.x()+r.width()) && p.y() >= r.y() && p.y() <= (r.y()+r.height()))
            return true;
        else
            return false;
    }
    Place placeInShape(QPoint p)
    {
        QRect big = _sh->getRect();
        big.setX(big.x() - BORDER);
        big.setY(big.y() - BORDER);
        big.setHeight(big.height() + BORDER);
        big.setWidth(big.width()   + BORDER);

        QRect small = _sh->getRect();

        bool top = p.y()>=big.y() && p.y()<=small.y();
        bool bottom = p.y()>=(small.y()+small.height()) && p.y()<=(big.y()+big.height());

        bool left = p.x()>=big.x() && p.x()<=small.x();
        bool right = p.x()>=(small.x()+small.width()) && p.x()<=(big.x()+big.width());

        if(isInShape(p))
            return Inside;
        else if(! isInBorder(p))
            return Outside;
        else if(top & left)
            return TopLeft;
        else if(top & right)
            return TopRight;
        else if(top)
            return Top;
        else if(bottom & left)
            return BottomLeft;
        else if(bottom & right)
            return BottomRight;
        else if(bottom)
            return Bottom;
        else if(left)
            return Left;
        else if(right)
            return Right;
    }
    virtual void move(int dx, int dy)
    {
        _sh->move(dx,dy);
    }

    virtual MyShape* clone(QPoint p1, QPoint p2, QColor bc, QColor fc)
    {
        return _sh->clone(p1,p2,bc,fc);
    }
    virtual QRect getRect()
    {
        return _sh->getRect();
    }

    MyShape* source_shape_with_deleting()
    {
        need_delete = true;
        return _sh;
    }
    MyShape* source_shape()
    {
        need_delete = false;
        return _sh;
    }

    void setPenColor(QColor c){ _sh->setPenColor(c); }
    void setBrushColor(QColor c){ _sh->setBrushColor(c); }

    void resizeTop(int dy)
    {
        _sh->resizeTop(dy);
    }
    void resizeBottom(int dy)
    {
        _sh->resizeBottom(dy);
    }
    void resizeLeft(int dx)
    {
        _sh->resizeLeft(dx);
    }
    void resizeRight(int dx)
    {
        _sh->resizeRight(dx);
    }

    void load(std::ifstream& f)
    {
        _sh->load(f);
    }
    void save(std::ofstream& f)
    {
        _sh->save(f);
    }

    QString name(){
        return _sh->name();
    }
private:
    MyShape* _sh;
    bool need_delete;
};

#endif // DECORATOR_SELECTED_H
