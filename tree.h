#ifndef TREE_H
#define TREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "observer.h"
#include "subject.h"
#include "paintarea.h"

class PaintArea;

class Tree : public QTreeWidget, public Observer, public Subject
{
    Q_OBJECT
public:
    friend class PaintArea;
    explicit Tree(QWidget *parent = nullptr);
    virtual ~Tree();

    void processNode(QTreeWidgetItem* parent_item, MyStorage<MyShape*>& shapes);
    void onSubjectChange(Subject*sub);

public slots:

    void myitemChanged(QTreeWidgetItem *item, int column);
private:
    QTreeWidgetItem* changedItem;

    int rows_counter;
    //PaintArea* _a;
};

#endif // Tree_H
