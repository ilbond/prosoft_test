#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>
#include <QSpinBox>

#include "findview.h"

class PathFinder : public QWidget {
    Q_OBJECT

public:
    PathFinder(QWidget *parent = nullptr);

    void readPositionSettings();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void writePositionSettings();

private:
    FindView *m_scene;

    QPushButton *m_generateButton;
    QSpinBox* m_width;
    QSpinBox* m_height;
};

#endif // PATHFINDER_H
