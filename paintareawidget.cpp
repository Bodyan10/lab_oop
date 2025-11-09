#include "paintAreaWidget.h"
#include <cstdio>
#include <algorithm>
#include <QApplication>

PaintAreaWidget::PaintAreaWidget(QWidget *parent)
    : QWidget(parent),
    ctrlPressed(false),
    currentTool(Tool::Select),
    currentColor(Qt::blue),
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
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
    setAutoFillBackground(true);
}

PaintAreaWidget::~PaintAreaWidget()
{
    if (tempShape) delete tempShape;
    printf("PaintAreaWidget destroyed\n");
}

void PaintAreaWidget::setCurrentTool(Tool tool)
{
    currentTool = tool;
    const char* toolNames[] = {"Selection", "Rectangle", "Circle", "Triangle", "Line"};
    printf("Tool changed: %s\n", toolNames[static_cast<int>(tool)]);
}

void PaintAreaWidget::setCurrentColor(const QColor& color)
{
    currentColor = color;
    printf("Current color changed to (%d, %d, %d)\n", color.red(), color.green(), color.blue());
}

void PaintAreaWidget::applyColorToSelected(const QColor& color)
{
    if (selection.size() > 0) {
        for (int i = 0; i < selection.size(); i++) {
            selection.at(i)->changeColor(color);
        }
        currentColor = color;
        update();
        printf("Color applied to %zu selected shapes\n", selection.size());
    } else {
        currentColor = color;
        printf("No shapes selected. Color set for future shapes\n");
    }
}

void PaintAreaWidget::deleteSelected()
{
    if (selection.size() > 0) {
        for (int i = 0; i < selection.size(); i++) {
            Shape* shape = selection.at(i);
            shapesContainer.removeElement(shape);
            delete shape;
        }
        selection.clear();
        update();
        printf("Deleted %zu selected shapes\n", selection.size());
    }
}

void PaintAreaWidget::selectAll()
{
    selection.clear();
    for (int i = 0; i < shapesContainer.size(); i++) {
        selection.addElement(shapesContainer.at(i));
        shapesContainer.at(i)->setSelected(true);
    }
    selection.updateRelativeInfo();
    update();
    printf("All %zu shapes selected\n", shapesContainer.size());
}

void PaintAreaWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. ЯВНО рисуем белый фон
    painter.fillRect(rect(), Qt::white);
    printf("DEBUG: White background drawn, rect: %dx%d\n", width(), height());

    // 2. Отладочная информация
    printf("DEBUG: PaintEvent - shapes: %zu, selection: %zu, creating: %s\n",
           shapesContainer.size(), selection.size(), isCreatingShape ? "true" : "false");

    // 3. Рисуем фигуры
    for (int i = 0; i < shapesContainer.size(); i++) {
        Shape* shape = shapesContainer.at(i);
        if (shape) {
            QRect bounds = shape->getBounds();
            printf("DEBUG: Drawing shape %d: %s at (%d,%d) size %dx%d\n",
                   i, shape->name().c_str(), bounds.x(), bounds.y(),
                   bounds.width(), bounds.height());
            shape->draw(painter);
        }
    }

    // 4. Временная фигура при создании
    if (isCreatingShape && tempShape) {
        QRect tempBounds = tempShape->getBounds();
        printf("DEBUG: Drawing TEMP shape: %s at (%d,%d) size %dx%d\n",
               tempShape->name().c_str(), tempBounds.x(), tempBounds.y(),
               tempBounds.width(), tempBounds.height());
        tempShape->draw(painter);
    }

    // 5. Рисуем выделение (рамку и маркеры) ПОСЛЕ фигур
    if (selection.size() > 0) {
        printf("DEBUG: Drawing selection with %zu shapes\n", selection.size());
        selection.draw(painter);
    }

    printf("DEBUG: PaintEvent finished\n\n");
}

void PaintAreaWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint mousePos = event->pos();
        printf("Mouse press at (%d, %d), tool: %d\n",
               mousePos.x(), mousePos.y(), static_cast<int>(currentTool));

        // Проверяем, не кликнули ли на маркер изменения размера
        Selection::MousePosState resizeState = selection.checkMousePos(mousePos);
        if (resizeState != Selection::MousePosState::None && selection.size() > 0) {
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

                if (!ctrlPressed) {
                    clearSelection();
                    selection.addElement(clickedShape);
                    clickedShape->setSelected(true);
                    selection.updateRelativeInfo();
                    printf("Shape '%s' selected\n", clickedShape->name().c_str());
                } else {
                    // Ctrl + клик - переключить выделение
                    if (selection.hasElement(clickedShape)) {
                        selection.removeElement(clickedShape);
                        clickedShape->setSelected(false);
                        printf("Shape '%s' deselected\n", clickedShape->name().c_str());
                    } else {
                        selection.addElement(clickedShape);
                        clickedShape->setSelected(true);
                        printf("Shape '%s' selected\n", clickedShape->name().c_str());
                    }
                    selection.updateRelativeInfo();
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
                tempShape = new Circle(mousePos, QSize(1, 1), currentColor);
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

void PaintAreaWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isResizing && (event->buttons() & Qt::LeftButton)) {
        // Режим изменения размера
        QPoint delta = event->pos() - resizeStartPoint;
        if (selection.resizeSelections(delta.x(), delta.y())) {
            resizeStartPoint = event->pos();
            update();
        }
    } else if (isCreatingShape && tempShape && (event->buttons() & Qt::LeftButton)) {
        // Режим создания фигуры
        QPoint currentPos = event->pos();
        int width = currentPos.x() - creationStartPoint.x();
        int height = currentPos.y() - creationStartPoint.y();

        tempShape->resize(abs(width), abs(height));
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

void PaintAreaWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (isResizing) {
            printf("Resize finished\n");
            isResizing = false;
            selection.updateRelativeInfo();
        } else if (isCreatingShape && tempShape) {
            QPoint releasePos = event->pos();
            int finalWidth = releasePos.x() - creationStartPoint.x();
            int finalHeight = releasePos.y() - creationStartPoint.y();

            bool shouldCreate = (abs(finalWidth) >= 10 && abs(finalHeight) >= 10);

            if (shouldCreate) {
                Shape* finalShape = nullptr;

                switch (currentTool) {
                case Tool::Rectangle:
                    finalShape = new Rectangle(creationStartPoint,
                                               QSize(abs(finalWidth), abs(finalHeight)),
                                               currentColor);
                    break;
                case Tool::Circle:
                    finalShape = new Circle(creationStartPoint,
                                            QSize(abs(finalWidth), abs(finalHeight)),
                                            currentColor);
                    break;
                case Tool::Triangle:
                    finalShape = new Triangle(creationStartPoint,
                                              QSize(abs(finalWidth), abs(finalHeight)),
                                              currentColor);
                    break;
                case Tool::Line:
                    finalShape = new Line(creationStartPoint,
                                          QSize(finalWidth, finalHeight),
                                          currentColor);
                    break;
                default:
                    break;
                }

                if (finalShape) {
                    shapesContainer.addElement(finalShape);
                    // Автоматически выделяем новую фигуру
                    clearSelection();
                    selection.addElement(finalShape);
                    finalShape->setSelected(true);
                    selection.updateRelativeInfo();
                    printf("Shape '%s' created and selected\n", finalShape->name().c_str());
                }
            } else {
                printf("Shape too small - creation cancelled\n");
            }

            delete tempShape;
            tempShape = nullptr;
            isCreatingShape = false;
            update();
        }
    }
}

void PaintAreaWidget::keyPressEvent(QKeyEvent *event)
{
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
        selection.moveSelections(-5, 0);
        update();
        break;
    case Qt::Key_Right:
        selection.moveSelections(5, 0);
        update();
        break;
    case Qt::Key_Up:
        selection.moveSelections(0, -5);
        update();
        break;
    case Qt::Key_Down:
        selection.moveSelections(0, 5);
        update();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    event->accept();
}

void PaintAreaWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
        printf("Ctrl released\n");
        event->accept();
    } else {
        QWidget::keyReleaseEvent(event);
    }
}

void PaintAreaWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    printf("Workspace resized: %dx%d\n", width(), height());
    update();
}

std::vector<Shape*> PaintAreaWidget::findShapesAtPoint(const QPoint& point)
{
    std::vector<Shape*> result;

    // Ищем фигуры с конца (верхние слои)
    for (int i = shapesContainer.size() - 1; i >= 0; i--) {
        Shape* shape = shapesContainer.at(i);
        if (shape && shape->hasPointIn(point)) {
            result.push_back(shape);
        }
    }

    printf("Found %zu shapes at point (%d, %d)\n", result.size(), point.x(), point.y());
    return result;
}

void PaintAreaWidget::clearSelection()
{
    for (int i = 0; i < selection.size(); i++) {
        selection.at(i)->setSelected(false);
    }
    selection.clear();
    printf("Selection cleared\n");
}
