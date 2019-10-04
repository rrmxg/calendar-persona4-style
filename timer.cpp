//=============================================================================
#include "mainwindow.h"
//=============================================================================
#include <QTimerEvent>
//=============================================================================

void MainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mw_timer)
    {
        updateTime();
    }

    if (event->timerId() == mw_timerAnimation)
    {
        updateAnimation();
    }
}

void MainWindow::updateAnimation()
{
    if ((mw_prefab[1].dayOfWeek.x() > prefabX[0]
         && mw_directionMovement == -1)
         ||
        (mw_prefab[0].dayOfWeek.x() < prefabX[1]
         && mw_directionMovement == 1))
    {
        for (int i = 0; i < 11; i++)
        {
            mw_prefab[i].dayOfWeek.move(mw_prefab[i].dayOfWeek.x()
                                        + mw_directionMovement,
                                        mw_prefab[i].dayOfWeek.y());
            mw_prefab[i].date     .move(mw_prefab[i].date     .x()
                                        + mw_directionMovement,
                                        mw_prefab[i].date     .y());
            mw_prefab[i].weather  .move(mw_prefab[i].weather  .x()
                                        + mw_directionMovement,
                                        mw_prefab[i].weather  .y());
        }
    }
    else
    {
        killTimer(mw_timerAnimation);

        updatePrefabs();

        resetAnimation();
    }
}

void MainWindow::resetAnimation()
{
    for (int i = 0; i < 11; i++)
    {
        mw_prefab[i].dayOfWeek.move(prefabX[i], prefabY[0]);
        mw_prefab[i].date     .move(prefabX[i], prefabY[1]);
        mw_prefab[i].weather  .move(prefabX[i], prefabY[2]);
    }

    mw_isKey = true;
}


