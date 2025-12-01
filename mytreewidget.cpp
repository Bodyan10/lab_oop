#include "mytreewidget.h"
#include <QHeaderView>
#include "myshape.h"
#include "group.h"
#include <QDebug>

MyTreeWidget::MyTreeWidget(Container<MyShape>* storage, Selection* selection, QWidget* parent)
    : QTreeWidget(parent), storage(storage), selection(selection),
    blockTreeSignals(false), blockSelectionUpdates(false)
{
    setHeaderLabel("Shapes");
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    this->storage->addObserver(this);
    this->selection->addObserver(this);

    connect(this, &QTreeWidget::itemSelectionChanged,
            this, &MyTreeWidget::onTreeSelectionChanged);

    rebuildTree();
}

MyTreeWidget::~MyTreeWidget()
{
}

void MyTreeWidget::onSubjectChanged()
{
    qDebug() << "MyTreeWidget::onSubjectChanged() called";
    // Если мы сами вызвали изменение selection, игнорируем
    if (blockSelectionUpdates) {
        qDebug() << "  - Ignoring (blockSelectionUpdates)";
        return;
    }

    rebuildTree();
}

void MyTreeWidget::rebuildTree()
{
    qDebug() << "MyTreeWidget::rebuildTree() called";

    blockTreeSignals = true;
    clear();

    // Строим дерево
    for (int i = 0; i < storage->getCount(); i++) {
        MyShape* shape = storage->getObject(i);
        if (shape) {
            processNode(nullptr, shape);
        }
    }

    // Синхронизируем выделение ИЗ selection в дерево
    clearSelection();
    for (int i = 0; i < selection->getCount(); i++) {
        MyShape* selectedShape = selection->getObject(i);
        QTreeWidgetItem* item = findItemForShape(selectedShape);
        if (item) {
            qDebug() << "  - Selecting item for shape in tree:" << selectedShape;
            item->setSelected(true);
        }
    }

    blockTreeSignals = false;
}

void MyTreeWidget::processNode(QTreeWidgetItem* parent, MyShape* shape)
{
    if (!shape) {
        return;
    }

    QTreeWidgetItem* item = new QTreeWidgetItem();

    //отключаем выделение для фигур в группе
    if (parent && dynamic_cast<Group*>(getShapeForItem(parent))) {
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    }

    QString typeName;
    char typeCode = shape->getTypeCode();
    switch (typeCode) {
    case 'R': typeName = "Rectangle"; break;
    case 'E': typeName = "Ellipse"; break;
    case 'T': typeName = "Triangle"; break;
    case 'L': typeName = "Line"; break;
    case 'A': typeName = "Arrow"; break;
    case 'G': typeName = "Group"; break;
    default: typeName = "Shape";
    }

    item->setText(0, typeName);
    item->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<void*>(shape)));

    // Устанавливаем выделение в соответствии с состоянием фигуры
    item->setSelected(shape->isSelected());

    if (parent) {
        parent->addChild(item);
    } else {
        addTopLevelItem(item);
    }

    // Рекурсивно обрабатываем детей групп
    if (Group* group = dynamic_cast<Group*>(shape)) {
        for (MyShape* child : group->getShapes()) {
            processNode(item, child);
        }
    }
}

void MyTreeWidget::onTreeSelectionChanged() {
    qDebug() << "MyTreeWidget::onTreeSelectionChanged() called";

    if (blockTreeSignals) {
        qDebug() << "  - Ignoring (blockTreeSignals)";
        return;
    }

    blockSelectionUpdates = true;

    // Очищаем выделение во всех фигурах
    for (int i = 0; i < storage->getCount(); i++) {
        MyShape* shape = storage->getObject(i);
        if (shape) {
            shape->setSelected(false);
        }
    }

    // Устанавливаем выделение только для выбранных в дереве
    QList<QTreeWidgetItem*> selectedItems = this->selectedItems();
    selection->clear();

    for (QTreeWidgetItem* item : selectedItems) {
        MyShape* shape = getShapeForItem(item);
        if (shape && storage->hasElement(shape)) { // Проверяем, что объект еще в контейнере
            shape->setSelected(true);
            selection->addObject(shape);
            qDebug() << "  - Adding shape to selection:" << shape;
        }
    }

    blockSelectionUpdates = false;
}

QTreeWidgetItem* MyTreeWidget::findItemForShape(MyShape* shape)
{
    for (int i = 0; i < topLevelItemCount(); i++) {
        QTreeWidgetItem* result = findItemForShapeRecursive(topLevelItem(i), shape);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

QTreeWidgetItem* MyTreeWidget::findItemForShapeRecursive(QTreeWidgetItem* item, MyShape* shape)
{
    if (!item) {
        return nullptr;
    }

    MyShape* itemShape = getShapeForItem(item);
    if (itemShape == shape) {
        return item;
    }

    for (int i = 0; i < item->childCount(); i++) {
        QTreeWidgetItem* result = findItemForShapeRecursive(item->child(i), shape);
        if (result) {
            return result;
        }
    }

    return nullptr;
}

MyShape* MyTreeWidget::getShapeForItem(QTreeWidgetItem* item)
{
    if (!item) return nullptr;

    QVariant data = item->data(0, Qt::UserRole);
    if (data.isValid()) {
        void* ptr = data.value<void*>();
        if (ptr) {
            return static_cast<MyShape*>(ptr);
        }
    }
    return nullptr;
}
