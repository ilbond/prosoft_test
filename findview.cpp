#include "findview.h"

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsView>

FindView::FindView(QObject *parent): QGraphicsScene(parent), m_grid(nullptr), m_startRect(nullptr) {}

void FindView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneMouseMove && m_grid) {
        mouseMove(event->lastScenePos().x(), event->lastScenePos().y());
    } else {
        QGraphicsScene::mouseMoveEvent(event);
    }
}

void FindView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_grid) {
        mouseClick(event->buttonDownScenePos(Qt::MouseButton::LeftButton).x(), event->buttonDownScenePos(Qt::MouseButton::LeftButton).y());
    } else {
        QGraphicsScene::mousePressEvent(event);
    }
}

void FindView::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsView* view = views().value(0);
    const QGraphicsView::ViewportAnchor anchor = view->transformationAnchor();
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    qreal factor;
    if (event->delta() > 0) {
        factor = 1.1;
    } else {
        factor = 0.9;
    }
    view->scale(factor, factor);
    view->setTransformationAnchor(anchor);

    QGraphicsScene::wheelEvent(event);

}

void FindView::mouseClick(int x, int y) {
    // Изменение начала или конца
    int cellX = x / CELL_SIZE;
    int cellY = y / CELL_SIZE;

    // Проверяем, не выходит ли за пределы сетки
    if (cellX < 0 || cellX >= m_grid->grid[0].size() || cellY < 0 || cellY >= m_grid->grid.size()) {
        return;
    }

    if (m_grid->grid[cellY][cellX] == WALL) {
        emit errorMessage(tr("Нельзя выбрать клетку-стену"));
        return;
    }

    // Установка точек A и B
    if (m_grid->startPoint == QPoint(-1, -1)) {
        m_grid->startPoint = QPoint(cellX, cellY);
        drawStart(cellX, cellY);
        return;
    } else if (m_grid->endPoint == QPoint(-1, -1)) {
        m_grid->endPoint = QPoint(cellX, cellY);

        std::vector<QPoint> path = m_grid->findPath();
        if (path.empty()) {
            emit errorMessage(tr("Невозможно построить путь"));
        } else {
            drawPath(m_grid->endPoint.x(), m_grid->endPoint.y(), path);
        }
    }
}

void FindView::mouseMove(int x, int y) {
    int cellX = x / CELL_SIZE;
    int cellY = y / CELL_SIZE;

    // Проверяем, не выходит ли за пределы сетки
    if (cellX < 0 || cellX >= m_grid->grid[0].size() || cellY < 0 || cellY >= m_grid->grid.size()) {
        return;
    }

    if (m_grid->grid[cellY][cellX] == WALL) {
        return;
    }

    if (m_grid->startPoint == QPoint(-1, -1)) { // Визуализируем точку A, если она еще не установлена
        drawStart(cellX, cellY);
    } else if (m_grid->endPoint == QPoint(-1, -1)) { // Визуализируем путь, если точка A установлена
        m_grid->endPoint = QPoint(cellX, cellY);
        drawPath(cellX, cellY, m_grid->findPath());
        m_grid->endPoint = QPoint(-1, -1);
    }
}

void FindView::drawGrid() {
    for (int y = 0; y < m_grid->grid.size(); ++y) {
        for (int x = 0; x < m_grid->grid[y].size(); ++x) {
            QRectF rect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            QGraphicsRectItem *rectItem = addRect(rect);

            switch (m_grid->grid[y][x]) {
            case EMPTY:
                rectItem->setBrush(Qt::white);
                break;
            case WALL:
                rectItem->setBrush(Qt::black);
                break;
            }
        }
    }
}

void FindView::drawPath(int endx, int endy, const std::vector<QPoint> &path)
{
    if (!m_pathRects.empty()) {
        for (size_t i = 0; i < m_pathRects.size(); ++i) {
            removeItem(m_pathRects[i]);
        }
        m_pathRects.clear();
    }

    for (size_t i = 1; i + 1 < path.size(); ++i) {
        QRectF rect(path[i].x() * CELL_SIZE, path[i].y() * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        QGraphicsRectItem *rectItem = addRect(rect);
        rectItem->setBrush(Qt::blue);
        m_pathRects.append(rectItem);
    }
    QRectF rect(endx * CELL_SIZE, endy * CELL_SIZE, CELL_SIZE, CELL_SIZE);
    QGraphicsRectItem *rectItem = addRect(rect);
    rectItem->setBrush(Qt::red);
    m_pathRects.append(rectItem);
}

void FindView::drawStart(int startx, int starty)
{
    if (m_startRect) {
        removeItem(m_startRect);
    }

    QRectF rect(startx * CELL_SIZE, starty * CELL_SIZE, CELL_SIZE, CELL_SIZE);
    QGraphicsRectItem *rectItem = addRect(rect);
    rectItem->setBrush(Qt::green);
    m_startRect = rectItem;
}

void FindView::generateGrid(int width, int height) {
    clear();
    m_pathRects.clear();
    m_startRect = nullptr;

    m_grid = new Grid(width, height, this);
    m_grid->generateWalls(0.3f); // 30% вероятность стены
    drawGrid(); // Отрисовываем сетку
}
