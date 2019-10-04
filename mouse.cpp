//=============================================================================
#include "mainwindow.h"
//=============================================================================
#include <QMouseEvent>
//=============================================================================

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton))
    {
        setGeometry(pos().x() + ( event->x() - mw_position.dx),
                    pos().y() + ( event->y() - mw_position.dy),
                    width(),
                    height());
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mw_position.dx = event->x();
        mw_position.dy = event->y();
    }
}
