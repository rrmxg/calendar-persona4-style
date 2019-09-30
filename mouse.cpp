///////////////////////////////////////////////////////////////////////////////
#include "mainwindow.h"
///////////////////////////////////////////////////////////////////////////////
#include <QMouseEvent>
///////////////////////////////////////////////////////////////////////////////

void Mainwindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons().testFlag(Qt::LeftButton))
    {
        setGeometry(pos().x() + ( event->x() - g_position.dx),
                    pos().y() + ( event->y() - g_position.dy),
                    width(),
                    height());
    }
}

void Mainwindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        g_position.dx = event->x();
        g_position.dy = event->y();
    }
}
