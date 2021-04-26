#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QLabel>

#include "MyStorage.h"
#include "cellipse.h"
#include "crectangle.h"
#include "cline.h"
#include "ctreangle.h"

#include "decorator_selected.h"

#include "observer.h"
#include "subject.h"

class Tree;

class PaintArea : public QLabel, public Subject, public Observer
{
    Q_OBJECT
public:
    friend class Tree;

    explicit PaintArea(QWidget *parent = nullptr);
    virtual ~PaintArea();

    void setShapeToDraw(MyShape* s);
    void setPenColor(QColor c );
    void setBrushColor(QColor c );

    void group();
    void ungroup();

    void deletingSelected();

    void save(QString fname);
    void open(QString fname);

    void clear();

    void onSubjectChange(Subject *sub);
signals:

public slots:
private:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

private:
    MyStorage<MyShape*> shapes;

    MyShape* shape_to_draw;
    QColor frame_to_draw;
    QColor fill_to_draw;
    QPoint fpoint_to_draw;
    QPoint spoint_to_draw;
    bool drawing;


    bool mouseMoved;
    MyShape* resizeable_shape;
    MyShape* moveable_shape;
    SelectedShape::Place place;
    //int toplayer;

};

#endif // PAINTAREA_H
