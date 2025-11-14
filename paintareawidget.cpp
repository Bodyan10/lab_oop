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
    setStyleSheet("background-color: white;");
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
    printf("Current color changed to (%d, %d, %d)\n", color.red(), color.green(), color.blue());
}

void PaintAreaWidget::applyColorToSelected(const QColor& color) {
    if (selection.getCount() > 0) {
        for (int i = 0; i < selection.getCount(); i++) {
            selection.getObject(i)->changeColor(color);
        }
        currentColor = color;
        update();
        printf("Color applied to %d selected shapes\n", selection.getCount());
    } else {
        currentColor = color;
        printf("No shapes selected. Color set for future shapes\n");
    }
}

void PaintAreaWidget::deleteSelected() {
    selection.clear();
    shapesContainer.removeSelected();
    update();
}

void PaintAreaWidget::selectAll() {
    clearSelection();
    for (int i = 0; i < shapesContainer.getCount(); i++) {
        Shape* shape = shapesContainer.getObject(i);
        selection.addObject(shape);
        shape->setSelected(true);
    }
    update();
    printf("All %d shapes selected\n", shapesContainer.getCount());
}

void PaintAreaWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. ЯВНО рисуем белый фон
    painter.fillRect(rect(), Qt::white);

    // 2. Рисуем фигуры
    for (int i = 0; i < shapesContainer.getCount(); i++) {
        Shape* shape = shapesContainer.getObject(i);
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
        printf("Mouse press at (%d, %d), tool: %d\n",
               mousePos.x(), mousePos.y(), static_cast<int>(currentTool));

        // Проверяем, не кликнули ли на маркер изменения размера
        Selection::MousePosState resizeState = selection.checkMousePos(mousePos);
        if (resizeState != Selection::MousePosState::None && selection.getCount() > 0) {
            isResizing = true;
            resizeStartPoint = mousePos;
            printf("Resize started at handle: %d\n", static_cast<int>(resizeState));
            return;
        }

        if (currentTool == Tool::Select) {
            // Режим выделения
            std::vector<Shape*> shapesUnderMouse = findShapesAtPoint(mousePos);

            if (!shapesUnderMouse.empty()) {
                Shape* clickedShape = shapesUnderMouse[0];
                printf("Clicked on shape: %s (already selected: %s)\n",
                       clickedShape->name().c_str(),
                       clickedShape->isSelected() ? "true" : "false");
                //ctrl не нажат
                if (!ctrlPressed) {
                    clearSelection();
                    selection.addObject(clickedShape);
                    clickedShape->setSelected(true);
                    printf("Shape '%s' selected\n", clickedShape->name().c_str());
                } else {
                    // Ctrl + клик - переключить выделение
                    if (selection.hasElement(clickedShape)) {
                        selection.removeElement(clickedShape);
                        clickedShape->setSelected(false);
                        printf("Shape '%s' deselected\n", clickedShape->name().c_str());
                    } else {
                        selection.addObject(clickedShape);
                        clickedShape->setSelected(true);
                        printf("Shape '%s' selected\n", clickedShape->name().c_str());
                    }
                }
            } else {
                // Клик по пустой области - снять выделение
                if (!ctrlPressed) {
                    clearSelection();
                    printf("Clicked on empty area - selection cleared\n");
                }
            }
        } else {
            // Режим создания фигуры
            isCreatingShape = true;
            creationStartPoint = mousePos;

            switch (currentTool) {
            case Tool::Rectangle:
                tempShape = new Rectangle(mousePos, QSize(1, 1), currentColor);
                break;
            case Tool::Circle:
                tempShape = new Ellipse(mousePos, QSize(1, 1), currentColor);
                break;
            case Tool::Triangle:
                tempShape = new Triangle(mousePos, QSize(1, 1), currentColor);
                break;
            case Tool::Line:
                tempShape = new Line(mousePos, QSize(1, 1), currentColor);
                break;
            default:
                break;
            }
            printf("Starting %s creation at (%d,%d)\n",
                   tempShape ? tempShape->name().c_str() : "unknown",
                   mousePos.x(), mousePos.y());
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
    } else if (currentTool == Tool::Select) {
        // Изменение курсора при наведении на маркеры
        Selection::MousePosState state = selection.checkMousePos(event->pos());
        Qt::CursorShape cursor = Qt::ArrowCursor;

        switch (state) {
        case Selection::MousePosState::TopLeft:
        case Selection::MousePosState::BottomRight:
            cursor = Qt::SizeFDiagCursor;
            break;
        case Selection::MousePosState::TopRight:
        case Selection::MousePosState::BottomLeft:
            cursor = Qt::SizeBDiagCursor;
            break;
        case Selection::MousePosState::Top:
        case Selection::MousePosState::Bottom:
            cursor = Qt::SizeVerCursor;
            break;
        case Selection::MousePosState::Left:
        case Selection::MousePosState::Right:
            cursor = Qt::SizeHorCursor;
            break;
        case Selection::MousePosState::None:
            cursor = Qt::ArrowCursor;
            break;
        }

        setCursor(cursor);
    }
}

void PaintAreaWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isCreatingShape && tempShape) {
        QPoint releasePos = event->pos();

        int finalWidth = releasePos.x() - creationStartPoint.x();
        int finalHeight = releasePos.y() - creationStartPoint.y();

        // Минимальный размер
        if (abs(finalWidth) > 2 && abs(finalHeight) > 2) {
            Shape* finalShape = nullptr;

            switch (currentTool) {
            case Tool::Rectangle:
                finalShape = new Rectangle(creationStartPoint,
                                           QSize(abs(finalWidth), abs(finalHeight)), currentColor);
                break;
            case Tool::Circle:
                finalShape = new Ellipse(creationStartPoint,
                                         QSize(abs(finalWidth), abs(finalHeight)), currentColor);
                break;
            case Tool::Triangle:
                finalShape = new Triangle(creationStartPoint,
                                          QSize(abs(finalWidth), abs(finalHeight)), currentColor);
                break;
            case Tool::Line:
                finalShape = new Line(creationStartPoint,
                                      QSize(finalWidth, finalHeight), currentColor);
                break;
            default:
                break;
            }

            if (finalShape) {
                // ПРОСТО ВЫЗЫВАЕМ ПОДГОНКУ ПЕРЕД ДОБАВЛЕНИЕМ
                finalShape->adjustToFitBounds(rect());

                shapesContainer.addObject(finalShape);
                clearSelection();
                selection.addObject(finalShape);
                finalShape->setSelected(true);
                printf("Shape created and adjusted to fit\n");
            }
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
        selection.moveSelections(-2, 0);
        for(int i = 0; i < selection.getCount(); i++) {
            Shape* shape = selection.getObject(i);
            shape->adjustToFitBounds(rect());
        }
        update();
        break;
    case Qt::Key_Right:
        selection.moveSelections(2, 0);
        for(int i = 0; i < selection.getCount(); i++) {
            Shape* shape = selection.getObject(i);
            shape->adjustToFitBounds(rect());
        }
        update();
        break;
    case Qt::Key_Up:
        selection.moveSelections(0, -2);
        for(int i = 0; i < selection.getCount(); i++) {
            Shape* shape = selection.getObject(i);
            shape->adjustToFitBounds(rect());
        }
        update();
        break;
    case Qt::Key_Down:
        selection.moveSelections(0, 2);
        for(int i = 0; i < selection.getCount(); i++) {
            Shape* shape = selection.getObject(i);
            shape->adjustToFitBounds(rect());
        }
        update();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    event->accept();
}

void PaintAreaWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
        printf("Ctrl released\n");
        event->accept();
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

void PaintAreaWidget::resizeEvent(QResizeEvent *event) {
    printf("Workspace resized: %dx%d\n", width(), height());
    QWidget::resizeEvent(event);
    update();
}

std::vector<Shape*> PaintAreaWidget::findShapesAtPoint(const QPoint& point) {
    std::vector<Shape*> result;

    // Ищем фигуры с конца (верхние слои)
    for (int i = shapesContainer.getCount() - 1; i >= 0; i--) {
        Shape* shape = shapesContainer.getObject(i);
        if (shape && shape->hasPointIn(point)) {
            result.push_back(shape);
        }
    }

    printf("Found %zu shapes at point (%d, %d)\n", result.size(), point.x(), point.y());
    return result;
}

void PaintAreaWidget::clearSelection() {
    for (int i = 0; i < selection.getCount(); i++) {
        selection.getObject(i)->setSelected(false);
    }
    selection.clear();
    printf("Selection cleared\n");
}

