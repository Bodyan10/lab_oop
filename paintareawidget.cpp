#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QResizeEvent>
#include <paintareawidget.h>
#include <ellipse.h>
#include <triangle.h>
#include <rectangle.h>
#include <line.h>
#include <arrow.h>>
#include <QMessageBox>

PaintAreaWidget::PaintAreaWidget(QWidget *parent)
    : QWidget(parent),
    ctrlPressed(false),
    currentTool(Tool::Select),
    currentColor(Qt::red),
    isCreatingShape(false),
    creationStartPoint(QPoint()),
    tempShape(nullptr),
    isResizing(false),
    resizeStartPoint(QPoint())
{
    printf("PaintAreaWidget created\n");
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    myTree = new MyTreeWidget(&shapesContainer, &selection);
    this->selection.addObserver(this);
    this->shapesContainer.addObserver(this);

}

PaintAreaWidget::~PaintAreaWidget() {
    if (tempShape) {
        delete tempShape;
    }
    printf("PaintAreaWidget destroyed\n");
}

void PaintAreaWidget::setCurrentTool(Tool tool) {
    currentTool = tool;
}

void PaintAreaWidget::setCurrentColor(const QColor& color) {
    currentColor = color;
}

void PaintAreaWidget::applyColorToSelected(const QColor& color) {
    if (selection.getCount() > 0) {
        for (int i = 0; i < selection.getCount(); i++) {
            selection.getObject(i)->changeColor(color);
        }
        currentColor = color;
        update();
    } else {
        currentColor = color;
    }
}

void PaintAreaWidget::deleteSelected() {
    selection.clear();
    shapesContainer.removeSelected();
    for (int i = shapesContainer.getCount() - 1; i >= 0; i--) {
        if (Arrow* arrow = dynamic_cast<Arrow*>(shapesContainer.getObject(i))) {
            if (!arrow->getFrom() || !arrow->getTo()) {
                shapesContainer.removeAt(i);
            }
        }
    }
    update();
}

void PaintAreaWidget::selectAll() {
    clearSelection();
    for (int i = 0; i < shapesContainer.getCount(); i++) {
        MyShape* shape = shapesContainer.getObject(i);
        selection.addObject(shape);
        shape->setSelected(true);
    }
    update();
}

void PaintAreaWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    // 1. рисуем белый фон
    painter.fillRect(rect(), Qt::white);

    // 2. Рисуем фигуры
    for (int i = 0; i < shapesContainer.getCount(); i++) {
        MyShape* shape = shapesContainer.getObject(i);
        if (shape) {
            shape->draw(painter);
        }
    }

    // 3. Временная фигура при создании
    if (isCreatingShape && tempShape) {
        tempShape->draw(painter);
    }

    // 4. Рисуем выделение (рамку и маркеры) ПОСЛЕ фигур
    if (selection.getCount() > 0) {
        selection.draw(painter);
    }
}

void PaintAreaWidget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        QPoint mousePos = event->pos();

        // Проверяем, не кликнули ли на маркер изменения размера
        Selection::MousePosState resizeState = selection.checkMousePos(mousePos);
        if (resizeState != Selection::MousePosState::None && selection.getCount() > 0) {
            isResizing = true;
            resizeStartPoint = mousePos;
            return;
        }

        if (currentTool == Tool::Select) {
            // Режим выделения
            std::vector<MyShape*> shapesUnderMouse = findShapesAtPoint(mousePos);

            if (!shapesUnderMouse.empty()) {
                MyShape* clickedShape = shapesUnderMouse[0];
                //ctrl не нажат
                if (!ctrlPressed) {
                    clearSelection();
                    selection.addObject(clickedShape);
                    clickedShape->setSelected(true);
                } else {
                    // Ctrl + клик - переключить выделение
                    if (selection.hasElement(clickedShape)) {
                        selection.removeElement(clickedShape);
                        clickedShape->setSelected(false);
                    } else {
                        selection.addObject(clickedShape);
                        clickedShape->setSelected(true);
                    }
                }
            } else {
                // Клик по пустой области - снять выделение
                if (!ctrlPressed) {
                    clearSelection();
                }
            }
        } else {
            // Режим создания фигуры
            isCreatingShape = true;
            creationStartPoint = mousePos;

            switch (currentTool) {
            case Tool::Rectangle:
                tempShape = new Rectangle(mousePos, QSize(1, 1), currentColor, false);
                break;
            case Tool::Ellipse:
                tempShape = new Ellipse(mousePos, QSize(1, 1), currentColor, false);
                break;
            case Tool::Triangle:
                tempShape = new Triangle(mousePos, QSize(1, 1), currentColor, false);
                break;
            case Tool::Line:
                tempShape = new Line(mousePos, QSize(1, 1), currentColor, false, 2);
                break;
            default:
                break;
            }
        }
        update();
    }
}

void PaintAreaWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isResizing && (event->buttons() & Qt::LeftButton)) {
        // Режим изменения размера
        QPoint delta = event->pos() - resizeStartPoint;
        if (selection.resizeSelections(delta.x(), delta.y(), rect())) {
            resizeStartPoint = event->pos();
            update();
        }
    } else if (isCreatingShape && tempShape && (event->buttons() & Qt::LeftButton)) {
        // Режим создания фигуры
        QPoint currentPos = event->pos();

        int width = currentPos.x() - creationStartPoint.x();
        int height = currentPos.y() - creationStartPoint.y();

        if (currentTool == Tool::Line) {
            tempShape->resize(width, height);
            tempShape->adjustToFitBounds(rect());
        } else {
            // Для других фигур - только положительные размеры
            tempShape->resize(abs(width), abs(height));
            tempShape->adjustToFitBounds(rect());
        }

        update();
    }
}

void PaintAreaWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isCreatingShape && tempShape) {
        QPoint releasePos = event->pos();

        int finalWidth = releasePos.x() - creationStartPoint.x();
        int finalHeight = releasePos.y() - creationStartPoint.y();


        MyShape* finalShape = nullptr;

        switch (currentTool) {
        case Tool::Rectangle:
            finalShape = new Rectangle(creationStartPoint, QSize(abs(finalWidth), abs(finalHeight)), currentColor, false);
            break;
        case Tool::Ellipse:
            finalShape = new Ellipse(creationStartPoint, QSize(abs(finalWidth), abs(finalHeight)), currentColor, false);
            break;
        case Tool::Triangle:
            finalShape = new Triangle(creationStartPoint, QSize(abs(finalWidth), abs(finalHeight)), currentColor, false);
            break;
        case Tool::Line:
            finalShape = new Line(creationStartPoint, QSize(finalWidth, finalHeight), currentColor, false, 2);
            break;
        default:
            break;
        }

        if (finalShape) {
            // Проверяем границы
            finalShape->adjustToFitBounds(rect());

            shapesContainer.addObject(finalShape);
            clearSelection();
            finalShape->setSelected(true);
            selection.addObject(finalShape);
            printf("MyShape created and adjusted to fit\n");
        }

        delete tempShape;
        tempShape = nullptr;
        isCreatingShape = false;
        update();
    } else if (event->button() == Qt::LeftButton && isResizing) {
        isResizing = false;
    }
}

void PaintAreaWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Control:
        ctrlPressed = true;
        printf("Ctrl pressed\n");
        break;
    case Qt::Key_Delete:
        deleteSelected();
        break;
    case Qt::Key_A:
        if (event->modifiers() & Qt::ControlModifier) {
            selectAll();
        }
        break;
    case Qt::Key_Left:
        selection.moveSelections(-5, 0, rect());
        update();
        break;
    case Qt::Key_Right:
        selection.moveSelections(5, 0, rect());
        update();
        break;
    case Qt::Key_Up:
        selection.moveSelections(0, -5, rect());
        update();
        break;
    case Qt::Key_Down:
        selection.moveSelections(0, 5, rect());
        update();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
}

void PaintAreaWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

void PaintAreaWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    update();
}

std::vector<MyShape*> PaintAreaWidget::findShapesAtPoint(const QPoint& point) {
    std::vector<MyShape*> result;

    // Ищем фигуры с конца (верхние слои)
    for (int i = shapesContainer.getCount() - 1; i >= 0; i--) {
        MyShape* shape = shapesContainer.getObject(i);
        if (shape && shape->hasPointIn(point)) {
            result.push_back(shape);
        }
    }

    return result;
}

void PaintAreaWidget::clearSelection() {
    for (int i = 0; i < selection.getCount(); i++) {
        MyShape* shape = selection.getObject(i);
        if (shape) {
            shape->setSelected(false);
        }
    }
    selection.clear();
}

void PaintAreaWidget::saveShapes(std::string filename) {
    shapesContainer.saveShapes(filename);
}

void PaintAreaWidget::loadShapes(std::string filename, ShapeFactory& factory) {
    shapesContainer.loadShapes(filename, factory);
}

void PaintAreaWidget::groupSelected() {
    if (selection.getCount() < 2) {
        return;
    }

    Group* newGroup = new Group();

    // Собираем выделенные фигуры в группу
    for (int i = 0; i < selection.getCount(); i++) {
        MyShape* shape = selection.getObject(i);
        newGroup->addShape(shape);
        shape->setParent(newGroup);

        // Удаляем указатели на фигуру из основного контейнера
        shapesContainer.removeElementPtr(shape);
    }

    newGroup->updateGroupBounds();

    // Добавляем группу в основной контейнер
    shapesContainer.addObject(newGroup);

    // Обновляем выделение
    clearSelection();
    selection.addObject(newGroup);
    selection.updateShapesRelativeFrame();

    update();
}

void PaintAreaWidget::unGroupSelected() {
    if (selection.getCount() != 1) {
        return;
    }

    Group* selected_group = dynamic_cast<Group*>(selection.getObject(0));
    if (selected_group) {

        // Получаем копию списка детей до удаления группы
        std::vector<MyShape*> children = selected_group->getShapes();

        for (MyShape* child : children) {
            selected_group->removeShape(child); // Удаляем указатели
        }

        clearSelection();

        std::vector<Observer*> observers_group = selected_group->getObservers();
        for (size_t i = 0; i < observers_group.size(); i++) {
            shapesContainer.removeElement(static_cast<Arrow*>(observers_group[i]));
        }

        // Удаляем группу из контейнера (это вызовет деструктор Group)
        shapesContainer.removeElement(selected_group);

        for (MyShape* child : children) {
            shapesContainer.addObject(child);
            selection.addObject(child);
            child->setSelected(true);
            child->setParent(nullptr);
        }

        selection.updateShapesRelativeFrame();
    }

    update();
}

void PaintAreaWidget::onSubjectChanged() {
    update();
}

void PaintAreaWidget::onArrowTool() {
    if (selection.getCount() == 2) {
        MyShape* from = selection.getObject(0);
        MyShape* to = selection.getObject(1);

        // Базовая валидация
        if (!from || !to || from == to) {
            printf("Invalid arrow: null objects or self-reference\n");
            return;
        }

        // Проверка на циклы
        if (wouldCreateCycle(from, to)) {
            QMessageBox::warning(
                this,
                "Cannot Create Arrow",
                "Creating this arrow would create a cyclic dependency.\n"
                "Please select different objects."
                );
            return;
        }

        Arrow* arrow = new Arrow(from, to, currentColor);
        arrow->setWidgetBounds(this->rect());
        shapesContainer.addObject(arrow);

        clearSelection();
        arrow->setSelected(true);
        selection.addObject(arrow);

        update();
        printf("Arrow created between two objects\n");
    }
}

bool PaintAreaWidget::wouldCreateCycle(MyShape* from, MyShape* to) {
    // Проверяем, существует ли уже путь от to к from
    // Если да, то добавление from->to создаст цикл

    std::vector<MyShape*> visited;
    std::vector<MyShape*> toProcess = {to};

    while (!toProcess.empty()) {
        MyShape* current = toProcess.back();
        toProcess.pop_back();

        // Если нашли путь to -> ... -> from, это цикл
        if (current == from) {
            return true;
        }

        // Пропускаем уже посещенные
        if (std::find(visited.begin(), visited.end(), current) != visited.end()) {
            continue;
        }

        visited.push_back(current);

        // Ищем стрелки, исходящие из current
        for (int i = 0; i < shapesContainer.getCount(); i++) {
            if (Arrow* arrow = dynamic_cast<Arrow*>(shapesContainer.getObject(i))) {
                if (arrow->getFrom() == current && arrow->getTo()) {
                    toProcess.push_back(arrow->getTo());
                }
            }
        }
    }

    return false;
}
