#include <algorithm>
#include <QMouseEvent>
#include "paintarea.h"
#include "decorator_selected.h"
#include "group_shape.h"

#include "tree.h"
PaintArea::PaintArea(QWidget *parent) : QLabel(parent)
{
    setMinimumSize(500,500);
    setMaximumSize(500,500);
    setStyleSheet("background: white; border: 2px solid black");
    setText("");
    setMouseTracking(true);

    shape_to_draw = 0;
    /*shape_to_draw = new CEllipse(0,0,1,1,Qt::black,Qt::black);
    shape_to_draw = new CRectangle(0,0,1,1,Qt::black,Qt::black);
    shape_to_draw = new CLine(QPoint(),QPoint(),Qt::black,Qt::black);
    shape_to_draw = new CTreangle(0,0,1,1,Qt::black,Qt::black);*/
    frame_to_draw = Qt::black;
    fill_to_draw = Qt::transparent;
    drawing = false;
    //toplayer = 0;
    resizeable_shape = 0;
    moveable_shape = 0;
    place = SelectedShape::Outside;
}

PaintArea::~PaintArea()
{
    for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); ++i)
        delete *i;
}

void PaintArea::setShapeToDraw(MyShape *s)
{
    delete shape_to_draw;
    shape_to_draw = s;
}

void PaintArea::setPenColor(QColor c)
{
    frame_to_draw = c;
    for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); ++i)
    {
        SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
        if(sel != nullptr)
        {
            (*i)->setPenColor(c);
        }
    }
}

void PaintArea::setBrushColor(QColor c)
{
    fill_to_draw = c;
    for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); ++i)
    {
        SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
        if(sel != nullptr)
        {
            (*i)->setBrushColor(c);
        }
    }
}

void PaintArea::group()
{
    Group* group = new Group();
    for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end();)
    {
        SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
        if(sel != nullptr)
        {
            MyShape* source_shape = sel->source_shape();
            i = shapes.erase(i);
            /*if(i != shapes.begin())
                --i;*/
            delete sel;
            group->pushShape(source_shape);
        }else
            i++;
    }
    //group->layer = toplayer++;
    shapes.push_front(group);
    //shapes.sort([](MyShape*l ,MyShape*r){return l->layer<r->layer;});
    //sort(shapes, [](MyShape*l ,MyShape*r){return l->layer<r->layer;});
    repaint();
    notifyEveryone();
}

void PaintArea::ungroup()
{
    MyStorage<Group*> groups;
    for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end();)
    {
        SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
        if(sel != nullptr)
        {
            MyShape* source_shape = sel->source_shape_with_deleting();
            Group* group = dynamic_cast<Group*>(source_shape);
            if(group != nullptr)
            {
                i = shapes.erase(i);
                sel->source_shape(); //for sel.deleting = false;
                delete sel;
                groups.push_front(group);
            }
            else
                i++;
        }else
            i++;
    }
    for(MyStorage<Group*>::iterator g = groups.begin(); g != groups.end();g++)
    {
        MyStorage<MyShape*>& group_shapes = (*g)->source_shapes();

        for(MyStorage<MyShape*>::iterator s = group_shapes.begin(); s != group_shapes.end();s++)
        {
            shapes.push_front(*s);
        }
        delete *g;
    }
    repaint();
    notifyEveryone();
}

void PaintArea::deletingSelected()
{
    for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end();)
    {
        SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
        if(sel != nullptr)
        {
            i = shapes.erase(i);
            delete sel;
        }else
            i++;
    }
    repaint();
    notifyEveryone();
}

void PaintArea::save(QString fname)
{
    std::ofstream file(fname.toStdString());

    file << shapes.size() << '\n';
    for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
        (*i)->save(file);

    file.close();
}

void PaintArea::open(QString fname)
{
    clear();
    std::ifstream f(fname.toStdString());
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
    notifyEveryone();
}

void PaintArea::clear()
{
    shape_to_draw = 0;
    drawing = false;
    resizeable_shape = 0;
    moveable_shape = 0;
    place = SelectedShape::Outside;
    setCursor(Qt::ArrowCursor);
    shapes.clear();
    notifyEveryone();
}

void PaintArea::onSubjectChange(Subject *sub)
{
    Tree* tree = dynamic_cast<Tree*>(sub);
    if(tree){
        QTreeWidgetItem* item = tree->changedItem;
        int purpose_row = item->text(0).toInt();
        int row = 1;
        MyStorage<MyShape*>::reverse_iterator i = shapes.rbegin();
        for(; row < purpose_row && i != shapes.rend(); ++i, ++row)
        {
        }


        SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
        if(sel == nullptr)
        {
            sel = new SelectedShape(*i);
            *i = sel;
        }else{
            MyShape* source_shape = sel->source_shape();
            *i = source_shape;
            delete sel;
        }
    }
    repaint();
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);


    QPainter painter(this);
    for(MyStorage<MyShape*>::reverse_iterator i = shapes.rbegin(); i != shapes.rend(); ++i)
    {
        (*i)->draw(painter);
        /*if(! (*i)->isSelected())//просто отрисовка
            (*i)->draw(painter);
        else //отрисовка с пунктирным прямоугольником
        {
            (*i)->draw(painter);
            QPen pen;
            pen.setColor(Qt::black);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
            painter.setBrush(Qt::transparent);
            painter.drawRect((*i)->getRect());
            //(*i)->draw(painter, color_of_selection);
        }*/
    }

    if(drawing && shape_to_draw != nullptr && cursor().shape() == Qt::ArrowCursor)
    {
        shape_to_draw->draw(painter,fpoint_to_draw, spoint_to_draw, frame_to_draw, fill_to_draw);
    }

}

void PaintArea::mouseReleaseEvent(QMouseEvent *ev)
{
    QLabel::mouseReleaseEvent(ev);

    if(drawing == false && mouseMoved == false)//click
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); ++i)
        {
            if((*i)->isInShape(fpoint_to_draw))
            {
                /*if( ! (*i)->isSelected())
                    (*i)->setSelected(true);
                else
                    (*i)->setSelected(false);*/
                SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
                if(sel == nullptr)
                {
                    sel = new SelectedShape(*i);
                    *i = sel;
                }else{
                    MyShape* source_shape = sel->source_shape();
                    *i = source_shape;
                    delete sel;
                }
                mouseMoveEvent(ev);
                notifyEveryone();
                break;
            }
        }
    if(drawing == true && fpoint_to_draw != spoint_to_draw && shape_to_draw != nullptr)
    {
        shapes.push_front(shape_to_draw->clone(fpoint_to_draw,spoint_to_draw,frame_to_draw, fill_to_draw));
        //(*shapes.begin())->layer = toplayer++;
        drawing = false;
        notifyEveryone();
    }
    repaint();
}

void PaintArea::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent(ev);
    fpoint_to_draw = ev->pos();
    /*if(ev->pos().x()>=0 && ev->pos().x() <= size().width())
        fpoint_to_draw.setX(ev->pos().x());
    if(ev->pos().y()>=0 && ev->pos().y() <= size().height())
        fpoint_to_draw.setY(ev->pos().y());*/
    mouseMoved = false;
}

void PaintArea::mouseMoveEvent(QMouseEvent *ev)
{
    QLabel::mouseMoveEvent(ev);
    //spoint_to_draw = ev->pos();
    if(ev->pos().x()>=0 && ev->pos().x() <= size().width())
        spoint_to_draw.setX(ev->pos().x());
    if(ev->pos().y()>=0 && ev->pos().y() <= size().height())
        spoint_to_draw.setY(ev->pos().y());
    mouseMoved = true;
    if(ev->buttons() == Qt::NoButton)
    {
        //bool in_selected = false;
        place = SelectedShape::Outside;
        for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
        {
            SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
            if(sel != nullptr)
            {
                place = sel->placeInShape(ev->pos());
                if(place==SelectedShape::Inside)
                {
                    moveable_shape = (*i);//если нужно двигать все, то закоменти это
                    break;
                }
                else if(place==SelectedShape::Bottom ||
                        place==SelectedShape::Top ||
                        place==SelectedShape::Left ||
                        place==SelectedShape::Right ||
                        place==SelectedShape::TopLeft ||
                        place==SelectedShape::TopRight ||
                        place==SelectedShape::BottomLeft ||
                        place==SelectedShape::BottomRight)
                {
                    resizeable_shape = sel->source_shape_with_deleting();
                    break;
                }
                /*if(sel->isInShape(ev->pos()))
                    in_selected = true;*/
            }
        }
        /*if( ! in_selected)
            setCursor(Qt::ArrowCursor);
        else{
            setCursor(Qt::SizeAllCursor);
        }*/
        switch (place) {
        case SelectedShape::Outside:
            setCursor(Qt::ArrowCursor);
            break;
        case SelectedShape::Inside:
            setCursor(Qt::SizeAllCursor);
            break;
        case SelectedShape::BottomRight:
        case SelectedShape::TopLeft:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case SelectedShape::BottomLeft:
        case SelectedShape::TopRight:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case SelectedShape::Bottom:
        case SelectedShape::Top:
            setCursor(Qt::SizeVerCursor);
            break;
        case SelectedShape::Left:
        case SelectedShape::Right:
            setCursor(Qt::SizeHorCursor);
            break;
        default:
            setCursor(Qt::WaitCursor);
            break;
        }
    }
    else if(ev->buttons() & Qt::LeftButton && cursor().shape() == Qt::ArrowCursor)
    {
        drawing = true;
        repaint();
    }
    else if(ev->buttons() & Qt::LeftButton && cursor().shape() == Qt::SizeAllCursor)
    {
        int dx = spoint_to_draw.x()-fpoint_to_draw.x();
        int dy = spoint_to_draw.y()-fpoint_to_draw.y();

        QRect r = moveable_shape->getRect();
        QSize s = size();
        if(dx > 0 && r.x()+r.width()+dx > s.width())
        {
            dx = 0;
        }else if(dx < 0 && r.x()+dx < 0)
        {
            dx = 0;
        }

        if(dy > 0 && r.y()+r.height()+dy > s.height())
        {
            dy = 0;
        }else if(dy < 0 && r.y()+dy < 0)
        {
            dy = 0;
        }
        moveable_shape->move(dx,dy);
        /*for(MyStorage<MyShape*>::iterator i = shapes.begin(); i != shapes.end(); i++)
        {
            SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);
            if(sel != nullptr)
            {
                sel->move(dx,dy);
            }

        }*/

        //fpoint_to_draw = ev->pos();
        if(ev->pos().x()>=0 && ev->pos().x() <= size().width())
                fpoint_to_draw.setX(ev->pos().x());
        if(ev->pos().y()>=0 && ev->pos().y() <= size().height())
                fpoint_to_draw.setY(ev->pos().y());
        repaint();
    }
    else if(ev->buttons() & Qt::LeftButton && (cursor().shape() == Qt::SizeVerCursor||cursor().shape() == Qt::SizeHorCursor
                                               ||
                                               cursor().shape() == Qt::SizeFDiagCursor||cursor().shape() == Qt::SizeBDiagCursor))
    {
        int dx = spoint_to_draw.x()-fpoint_to_draw.x();
        int dy = spoint_to_draw.y()-fpoint_to_draw.y();

        QRect r = resizeable_shape->getRect();
        QSize s = size();
        if(dx > 0 && r.x()+r.width()+dx > s.width() && (place==SelectedShape::Right || place==SelectedShape::TopRight || place==SelectedShape::BottomRight))
        {
            dx = 0;
        }else if(dx < 0 && r.x()+dx < 0 && (place==SelectedShape::Left || place==SelectedShape::TopLeft || place==SelectedShape::BottomLeft))
        {
            dx = 0;
        }

        if(dy > 0 && r.y()+r.height()+dy > s.height() && (place==SelectedShape::Bottom || place==SelectedShape::BottomLeft || place==SelectedShape::BottomRight))
        {
            dy = 0;
        }else if(dy < 0 && r.y()+dy < 0 && (place==SelectedShape::Top || place==SelectedShape::TopLeft || place==SelectedShape::TopRight))
        {
            dy = 0;
        }

        /*if(place == SelectedShape::Top)
        {
            resizeable_shape->resizeTop(dy);
        }else if(pla){//Bottom
            resizeable_shape->resizeBottom(dy);
        }*/

        switch (place) {
        case SelectedShape::BottomRight:
            resizeable_shape->resizeBottom(dy);
            resizeable_shape->resizeRight(dx);
            break;
        case SelectedShape::TopLeft:
            resizeable_shape->resizeTop(dy);
            resizeable_shape->resizeLeft(dx);
            break;
        case SelectedShape::BottomLeft:
            resizeable_shape->resizeLeft(dx);
            resizeable_shape->resizeBottom(dy);
            break;
        case SelectedShape::TopRight:
            resizeable_shape->resizeRight(dx);
            resizeable_shape->resizeTop(dy);
            break;
        case SelectedShape::Bottom:
            resizeable_shape->resizeBottom(dy);
            break;
        case SelectedShape::Top:
            resizeable_shape->resizeTop(dy);
            break;
        case SelectedShape::Left:
            resizeable_shape->resizeLeft(dx);
            break;
        case SelectedShape::Right:
            resizeable_shape->resizeRight(dx);
            break;
        default:
            break;
        }

        if(ev->pos().x()>=0 && ev->pos().x() <= size().width())
                fpoint_to_draw.setX(ev->pos().x());
        if(ev->pos().y()>=0 && ev->pos().y() <= size().height())
                fpoint_to_draw.setY(ev->pos().y());
        repaint();
    }
}
