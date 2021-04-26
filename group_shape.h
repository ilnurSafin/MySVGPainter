#ifndef GROUP_SHAPE_H
#define GROUP_SHAPE_H
#include "myshape.h"
#include "MyStorage.h"
#include "shapefactory.h"

class Tree;

class Group : public MyShape
{
public:
    friend class Tree;

    Group(){
        need_delete = true;
    }
    virtual ~Group(){
        if(need_delete)
            for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
                delete *i;
    }

    void pushShape(MyShape* sh)
    {
        shapes.push_front(sh);
    }


    virtual void draw(QPainter& p){
        //shapes.sort([](MyShape*l ,MyShape*r){return l->layer >= r->layer;});
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->draw(p);
    }
    virtual void draw(QPainter&p, QColor bc){
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->draw(p,bc);
    }
    virtual void draw(QPainter &p, QPoint p1, QPoint p2, QColor bc, QColor fc){
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->draw(p,p1,p2,bc,fc);
    }

    virtual bool isInShape(QPoint p){
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            if((*i)->isInShape(p))
                return true;
        return false;
    }
    virtual void move(int dx, int dy){
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->move(dx,dy);
    }

    virtual MyShape* clone(QPoint p1, QPoint p2, QColor bc, QColor fc){
        throw "MyShape* Group::clone(QPoint p1, QPoint p2, QColor bc, QColor fc) not implemented";
        return 0;
    }

    virtual QRect getRect(){
        int x=-1,y=-1;
        int w=0,h=0;
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
        {
            QRect r = (*i)->getRect();

            if(x == -1)
                x = r.x();
            else
                x = std::min(x, r.x());

            if(y == -1)
                y = r.y();
            else
                y = std::min(y, r.y());
        }
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
        {
            QRect r = (*i)->getRect();

            if( (x+w) < (r.x()+r.width())  )
            {
                w = (r.x()+r.width())-x;
            }
            if( (y+h) < (r.y()+r.height())  )
            {
                h = (r.y()+r.height())-y;
            }
        }
        return QRect(x,y,w,h);
    }

    MyStorage<MyShape*>& source_shapes()
    {
        need_delete = false;
        return shapes;
    }

    void setPenColor(QColor c){
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->setPenColor(c);
    }
    void setBrushColor(QColor c){
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->setBrushColor(c);
    }

    void resizeTop(int dy){
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->resizeTop(dy);
    }
    void resizeBottom(int dy){
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->resizeBottom(dy);
    }
    void resizeLeft(int dx)
    {
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->resizeLeft(dx);
    }
    void resizeRight(int dx)
    {
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->resizeRight(dx);
    }

    void load(std::ifstream& f){
        int size;
        f >> size;
        for(int i=0; i < size; ++i)
        {
            std::string type;
            f >> type;

            MyShape* s = ShapeFactory::createShape(type);
            s->load(f);
            shapes.push_back(s);
        }
    }
    void save(std::ofstream& f){
        f << "Group" << '\n' << shapes.size() << '\n';
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
            (*i)->save(f);
    }

    QString name(){
        return "Group";
    }
private:
    MyStorage<MyShape*> shapes;
    bool need_delete;
};

#endif // GROUP_SHAPE_H
