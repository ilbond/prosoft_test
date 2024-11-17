#include "grid.h"

#include <QHash>
#include <queue>

Grid::Grid(int width, int height, QObject *parent): QObject(parent) {
    grid.resize(height, std::vector<CellType>(width, EMPTY));
}

void Grid::generateWalls(float wallDensity) {
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (static_cast<float>(std::rand()) / RAND_MAX < wallDensity) {
                grid[y][x] = WALL;
            }
        }
    }
}

std::vector<QPoint> Grid::findPath() {
    if (startPoint == QPoint(-1, -1) || endPoint == QPoint(-1, -1)) {
        return {}; // Если точки не установлены, вернуть пустой путь
    }

    std::queue<QPoint> queue;
    QHash<QPoint, QPoint> parent; // Для отслеживания пути
    queue.push(startPoint);
    parent[startPoint] = QPoint(-1, -1);

    while (!queue.empty()) {
        QPoint current = queue.front();
        queue.pop();

        // Проверка достижения конечной точки
        if (current == endPoint) {
            QStack<QPoint> pathStack;
            QPoint step = endPoint;

            while (step != QPoint(-1, -1)) {
                pathStack.push(step);
                step = parent[step];
            }

            return std::vector<QPoint>(pathStack.begin(), pathStack.end());
        }

        // Проверка соседних клеток
        for (const QPoint &dir : {QPoint(1, 0), QPoint(0, 1), QPoint(-1, 0), QPoint(0, -1)}) {
            QPoint neighbor = current + dir;

            if (neighbor.x() >= 0 && neighbor.x() < grid[0].size() &&
                neighbor.y() >= 0 && neighbor.y() < grid.size() &&
                grid[neighbor.y()][neighbor.x()] != WALL &&
                !parent.count(neighbor)) {

                queue.push(neighbor);
                parent[neighbor] = current; // Сохраняем родителя
            }
        }
    }

    return {}; // Если путь не найден
}
