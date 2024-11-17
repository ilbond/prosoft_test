#include <QApplication>

#include "pathfinder.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    PathFinder window;
    window.setWindowTitle("Поиск пути");
    window.readPositionSettings();
    window.show();
    return app.exec();
}
