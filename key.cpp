//=============================================================================
#include "mainwindow.h"
//=============================================================================
#include <QKeyEvent>
//=============================================================================

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (mw_isKey)
    {
        switch (event->key())
        {
        case Qt::Key_Left:
            updatesAfterClicking(LEFT_DAY);
            break;
        case Qt::Key_Right:
            updatesAfterClicking(RIGHT_DAY);
            break;
        case Qt::Key_Space:
            updatesAfterClicking(ZERO);
            break;
        default:
            break;
        }
    }
}
