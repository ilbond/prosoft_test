#include "pathfinder.h"

#include <QGraphicsView>
#include <QVBoxLayout>
#include <QSettings>
#include <QMessageBox>

PathFinder::PathFinder(QWidget *parent) :
    QWidget(parent),
    m_scene(new FindView(this)),
    m_generateButton(new QPushButton("Генерировать", this)),
    m_width(new QSpinBox(this)),
    m_height(new QSpinBox(this)) {

    QGraphicsView *view = new QGraphicsView(m_scene, this);
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_width->setPrefix("W");
    m_height->setPrefix("H");
    m_width->setValue(15);
    m_height->setValue(15);
    m_width->setMinimum(2);
    m_height->setMinimum(2);

    layout->addWidget(view);
    layout->addWidget(m_width);
    layout->addWidget(m_height);
    layout->addWidget(m_generateButton);
    setLayout(layout);

    connect(m_generateButton, &QPushButton::clicked, m_scene, [this]() {
        m_scene->generateGrid(m_width->value(), m_height->value());
    });

    connect(m_scene, &FindView::errorMessage, this, [this](const QString& msg) {
        QMessageBox::warning(this, tr("Внимание"), msg);
    });

    view->setMouseTracking(true); // Включаем отслеживание мыши
}

void PathFinder::closeEvent(QCloseEvent *event)
{
    writePositionSettings();
}

void PathFinder::writePositionSettings()
{
    QSettings settings("ilbond", "prosoft_test");

    settings.beginGroup("pathfinder");

    settings.setValue("geometry", saveGeometry());

    settings.setValue("width", m_width->value());
    settings.setValue("height", m_height->value());

    settings.setValue( "maximized", isMaximized() );
    if ( !isMaximized() ) {
        settings.setValue( "pos", pos() );
        settings.setValue( "size", size() );
    }

    settings.endGroup();
}

void PathFinder::readPositionSettings()
{
    QSettings settings("ilbond", "prosoft_test");

    settings.beginGroup("pathfinder");

    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());

    m_width->setValue(settings.value("width", 15).toInt());
    m_height->setValue(settings.value("height", 15).toInt());

    move(settings.value("pos", QPoint(200, 200)).toPoint());
    resize(settings.value("size", size()).toSize());
    if (settings.value("maximized", isMaximized()).toBool()) {
        showMaximized();
    }

    settings.endGroup();
}

