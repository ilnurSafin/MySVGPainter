#include "tree.h"
#include "group_shape.h"
#include <QDebug>
#include <string>

Tree::Tree(QWidget *parent)
    :QTreeWidget(parent)
{
    //setHeaderHidden(true);
    connect(this,&Tree::itemChanged, this, &Tree::myitemChanged);
    setColumnWidth(0,55);
    setMinimumWidth(170);
    setMaximumHeight(500);
}

Tree::~Tree()
{

}

void Tree::processNode(QTreeWidgetItem *parent_item, MyStorage<MyShape *> &shapes)
{

    for(MyStorage<MyShape*>::reverse_iterator i = shapes.rbegin(); i != shapes.rend(); ++i)
    {
        SelectedShape* sel = dynamic_cast<SelectedShape*>(*i);

        QTreeWidgetItem* item;
        if(parent_item == nullptr){
            item = new QTreeWidgetItem(this);
            if(sel != nullptr)
                item->setCheckState(0,Qt::Checked);
            else
                item->setCheckState(0,Qt::Unchecked);

            item->setText(0, std::to_string(rows_counter++).data());
        }
        else
        {
            item = new QTreeWidgetItem(parent_item);
            item->setText(0, " ");
        }


        item->setText(1, (*i)->name());
        //

        if((*i)->name() == "Group")
        {
            Group* g;
            if(sel == nullptr)
                g = dynamic_cast<Group*>(*i);
            else{
                MyShape* s = sel->source_shape_with_deleting();
                g = dynamic_cast<Group*>(s);
            }
            processNode(item, g->shapes);
        }
    }
}

void Tree::onSubjectChange(Subject *sub)
{
    rows_counter = 1;
    disconnect(this,&Tree::itemChanged, this, &Tree::myitemChanged);
    PaintArea* a = dynamic_cast<PaintArea*>(sub);
    if(a != nullptr){
        MyStorage<MyShape*>& shapes = a->shapes;
        clear();
        processNode(0, shapes);
    }
    connect(this,&Tree::itemChanged, this, &Tree::myitemChanged);
}

void Tree::myitemChanged(QTreeWidgetItem *item, int column)
{
    changedItem = item;
    //qDebug() << "Col: " << column << ", state:" << item->checkState(0);
    notifyEveryone();
}
