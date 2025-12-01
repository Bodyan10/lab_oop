#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include <QTreeWidget>
#include "observer.h"
#include "container.h"
#include "selection.h"

class MyShape;
class Group;
class Selection;

class MyTreeWidget : public QTreeWidget, public Observer
{
    Q_OBJECT

public:
    MyTreeWidget(Container<MyShape>* storage, Selection* selection, QWidget* parent = nullptr);
    ~MyTreeWidget();

    void onSubjectChanged() override;

private slots:
    void onTreeSelectionChanged();

private:
    void rebuildTree();
    void processNode(QTreeWidgetItem* parent, MyShape* shape);
    QTreeWidgetItem* findItemForShape(MyShape* shape);
    QTreeWidgetItem* findItemForShapeRecursive(QTreeWidgetItem* item, MyShape* shape);
    MyShape* getShapeForItem(QTreeWidgetItem* item);

    Container<MyShape>* storage;
    Selection* selection;
    bool blockTreeSignals;
    bool blockSelectionUpdates;
};

#endif // MYTREEWIDGET_H
