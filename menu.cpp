//=============================================================================
#include "mainwindow.h"
//=============================================================================
#include <QDesktopServices>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
//=============================================================================

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    mw_pMenu->exec(event->globalPos());
}

void MainWindow::menuAction(QAction *action)
{
    QString str = action->text().remove("&");

    if (str == "Exit")
        close();

    if (str == "Readme")
        QMessageBox::about(this,
                           "Readme",
                           "Hotkey                      \n"
                           "LeftArrown - back by date   \n"
                           "RightArrow - forward by date\n"
                           "Space - current date          ");

    if (str == "City name")
        mw_pDialog->exec();

    if (str == "Accuweather")
        QDesktopServices::openUrl(QUrl("https://www.accuweather.com"));
}
