#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QPoint>
#include <QStack>

const int GRID_SIZE = 20; // размер клетки
const int CELL_SIZE = 20; // размер клетки в пикселях

enum CellType {
    EMPTY,
    WALL,
};

class Grid: public QObject {
    Q_OBJECT
public:
    Grid(int width, int height, QObject *parent = nullptr);

    void generateWalls(float wallDensity);
    std::vector<QPoint> findPath();

    std::vector<std::vector<CellType>> grid;
    QPoint startPoint{-1, -1};
    QPoint endPoint{-1, -1};
};

#endif // GRID_H
