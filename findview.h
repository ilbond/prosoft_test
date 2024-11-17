#ifndef FINDVIEW_H
#define FINDVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsRectItem>

#include "grid.h"

class FindView: public QGraphicsScene
{
    Q_OBJECT
public:
    FindView(QObject *parent = nullptr);

public slots:
    void generateGrid(int width, int height);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

private slots:
    void mouseClick(int x, int y);
    void mouseMove(int x, int y);

    void drawGrid();
    void drawPath(int endx, int endy, const std::vector<QPoint>& path);
    void drawStart(int startx, int starty);

signals:
    void errorMessage(const QString& msg);

private:
    Grid *m_grid;
    QVector<QGraphicsRectItem*> m_pathRects;
    QGraphicsRectItem* m_startRect;
};

#endif // FINDVIEW_H
