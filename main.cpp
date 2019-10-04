//=============================================================================
#include "mainwindow.h"
//=============================================================================
#include <QApplication>
//=============================================================================

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName  (ORGANIZATION_NAME  );
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName   (APPLICATION_NAME   );

    QLocale::setDefault(QLocale(QLocale::English));

    MainWindow w;
    w.show();

    return a.exec();
}
