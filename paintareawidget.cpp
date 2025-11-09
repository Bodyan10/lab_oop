#include "paintAreaWidget.h"
#include <cstdio>
#include <algorithm>
#include "rectangle.h"
#include "circle.h"
#include "triangle.h"
#include "line.h"

PaintAreaWidget::PaintAreaWidget(QWidget *parent)
    : QWidget(parent),
    ctrlPressed(false),
    currentTool(Tool::Select),
    currentColor(Qt::blue),
    isCreatingShape(false),
    creationStartPoint(QPoint()),
    tempShape(nullptr)
{
    printf("PaintAreaWidget создан\n");
    shapesContainer = new Container<Shape>();
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setAutoFillBackground(true);
}

PaintAreaWidget::~PaintAreaWidget()
{
    delete shapesContainer;
    if (tempShape) delete tempShape;
    printf("PaintAreaWidget удален\n");
}

void PaintAreaWidget::setCurrentTool(Tool tool)
{
    currentTool = tool;
    const char* toolNames[] = {"Выделение", "Прямоугольник", "Круг", "Треугольник", "Отрезок"};
    printf("Инструмент изменен: %s\n", toolNames[static_cast<int>(tool)]);
}

void PaintAreaWidget::setCurrentColor(const QColor& color)
{
    currentColor = color;
    printf("Цвет изменен: (%d, %d, %d)\n", color.red(), color.green(), color.blue());
}

void PaintAreaWidget::deleteSelected()
{
    deleteSelectedShapes();
}

void PaintAreaWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Отрисовываем белый фон
    painter.fillRect(rect(), Qt::white);

    // Отрисовываем все фигуры из контейнера
    for (int i = 0; i < shapesContainer->getCount(); i++) {
        Shape* shape = shapesContainer->getObject(i);
        if (shape) {
            shape->draw(&painter);
        }
    }

    // Отрисовываем временную фигуру при создании
    if (isCreatingShape && tempShape) {
        tempShape->draw(&painter);
    }

    // Отрисовываем рамку рабочей области
    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
}

void PaintAreaWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint mousePos = event->pos();

        if (currentTool == Tool::Select) {
            // Режим выделения
            std::vector<Shape*> shapesUnderMouse = findShapesAtPoint(mousePos);

            if (!shapesUnderMouse.empty()) {
                // Обрабатываем клик по фигуре(ам)
                if (!ctrlPressed) {
                    // Без Ctrl - снимаем выделение со всех и выделяем только верхнюю
                    clearSelection();
                    shapesUnderMouse[0]->setSelected(true);
                    printf("Выделена фигура: %s\n", shapesUnderMouse[0]->name().c_str());
                } else {
                    // С Ctrl - переключаем выделение верхней фигуры
                    Shape* topShape = shapesUnderMouse[0];
                    topShape->setSelected(!topShape->isSelected());
                    printf("Переключено выделение фигуры: %s (теперь %s)\n",
                           topShape->name().c_str(),
                           topShape->isSelected() ? "выделена" : "не выделена");
                }
            } else {
                // Клик по пустой области - снимаем выделение, если не зажат Ctrl
                if (!ctrlPressed) {
                    clearSelection();
                    printf("Снято выделение со всех фигур\n");
                }
            }
        } else {
            // Режим создания фигуры
            isCreatingShape = true;
            creationStartPoint = mousePos;

            // Создаем временную фигуру в зависимости от инструмента
            switch (currentTool) {
            case Tool::Rectangle:
                tempShape = new Rectangle();
                printf("Начато создание прямоугольника\n");
                break;
            case Tool::Circle:
                tempShape = new Circle();
                printf("Начато создание круга\n");
                break;
            case Tool::Triangle:
                tempShape = new Triangle();
                printf("Начато создание треугольника\n");
                break;
            case Tool::Line:
                tempShape = new Line();
                printf("Начато создание линии\n");
                break;
            default:
                break;
            }

            if (tempShape) {
                tempShape->setColor(currentColor);
                // Инициализируем фигуру начальными точками
                tempShape->createFromPoints(mousePos, mousePos);
            }
        }

        update();
    }
}

void PaintAreaWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isCreatingShape && tempShape && (event->buttons() & Qt::LeftButton)) {
        QPoint currentPos = event->pos();

        // Ограничиваем позицию границами виджета
        QPoint boundedPos = currentPos;
        boundedPos.setX(std::max(1, std::min(currentPos.x(), width() - 2)));
        boundedPos.setY(std::max(1, std::min(currentPos.y(), height() - 2)));

        // Обновляем фигуру
        tempShape->createFromPoints(creationStartPoint, boundedPos);
        update();
    }
}

void PaintAreaWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isCreatingShape && tempShape) {
        QPoint releasePos = event->pos();

        // Проверяем минимальный размер фигуры
        QRect bounds = tempShape->boundingRect();
        bool shouldCreate = (bounds.width() >= 10 && bounds.height() >= 10);

        if (shouldCreate) {
            // Создаем финальную фигуру через клонирование
            Shape* finalShape = tempShape->clone();
            finalShape->setColor(currentColor);
            shapesContainer->addObject(finalShape);
            printf("Фигура '%s' добавлена в контейнер. Всего объектов: %d\n",
                   finalShape->name().c_str(), shapesContainer->getCount());
        } else {
            printf("Фигура слишком мала - отмена создания\n");
        }

        delete tempShape;
        tempShape = nullptr;
        isCreatingShape = false;
        update();
    }
}

void PaintAreaWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Control:
        ctrlPressed = true;
        break;
    case Qt::Key_Delete:
        deleteSelectedShapes();
        break;
    case Qt::Key_A:
        if (event->modifiers() & Qt::ControlModifier) {
            // Ctrl+A - выделить все
            for (int i = 0; i < shapesContainer->getCount(); i++) {
                shapesContainer->getObject(i)->setSelected(true);
            }
            printf("Выделены все фигуры\n");
            update();
        }
        break;
    case Qt::Key_Left:
        moveSelectedShapes(QPoint(-5, 0));
        break;
    case Qt::Key_Right:
        moveSelectedShapes(QPoint(5, 0));
        break;
    case Qt::Key_Up:
        moveSelectedShapes(QPoint(0, -5));
        break;
    case Qt::Key_Down:
        moveSelectedShapes(QPoint(0, 5));
        break;
    default:
        // Если не наша клавиша - передаем родителю
        QWidget::keyPressEvent(event);
        return;
    }

    // Если обработали нашу клавишу, не передаем событие дальше
    event->accept();
}

void PaintAreaWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
        event->accept();
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

void PaintAreaWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    printf("Размер рабочей области изменен: %dx%d\n", width(), height());
    update();
}

std::vector<Shape*> PaintAreaWidget::findShapesAtPoint(const QPoint& point)
{
    std::vector<Shape*> result;

    // Ищем все фигуры, содержащие точку клика (с конца для получения верхних)
    for (int i = shapesContainer->getCount() - 1; i >= 0; i--) {
        Shape* shape = shapesContainer->getObject(i);
        if (shape && shape->contains(point)) {
            result.push_back(shape);
        }
    }

    return result;
}

void PaintAreaWidget::clearSelection()
{
    for (int i = 0; i < shapesContainer->getCount(); i++) {
        shapesContainer->getObject(i)->setSelected(false);
    }
}

void PaintAreaWidget::deleteSelectedShapes()
{
    int countBefore = shapesContainer->getCount();
    shapesContainer->removeSelected();
    int countAfter = shapesContainer->getCount();

    if (countBefore != countAfter) {
        printf("Удалено выделенных фигур: %d. Осталось: %d\n",
               countBefore - countAfter, countAfter);
        update();
    }
}

void PaintAreaWidget::moveSelectedShapes(const QPoint& delta)
{
    // Проверяем, можно ли переместить все выделенные фигуры
    if (!canMoveShapes(delta)) {
        printf("Перемещение невозможно - выход за границы\n");
        return;
    }

    bool moved = false;
    // Перемещаем все выделенные фигуры
    for (int i = 0; i < shapesContainer->getCount(); i++) {
        Shape* shape = shapesContainer->getObject(i);
        if (shape && shape->isSelected()) {
            shape->move(delta);
            moved = true;
        }
    }

    if (moved) {
        printf("Выделенные фигуры перемещены на (%d, %d)\n", delta.x(), delta.y());
        update();
    }
}

bool PaintAreaWidget::canMoveShapes(const QPoint& delta)
{
    // Проверяем, не выйдут ли выделенные фигуры за границы после перемещения
    for (int i = 0; i < shapesContainer->getCount(); i++) {
        Shape* shape = shapesContainer->getObject(i);
        if (shape && shape->isSelected()) {
            // Создаем временную копию для проверки
            Shape* testShape = shape->clone();
            testShape->move(delta);

            // Проверяем, не выходит ли фигура за границы рабочей области
            if (testShape->isOutOfBounds(rect())) {
                delete testShape;
                return false;
            }
            delete testShape;
        }
    }
    return true;
}
