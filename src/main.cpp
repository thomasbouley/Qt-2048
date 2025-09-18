#include "startwindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Thomas Bouley");
    a.setOrganizationDomain("bouley.com");
    a.setApplicationName("2048");

    StartWindow w;
    QObject::connect(&a,&QApplication::aboutToQuit,&w,&StartWindow::save_state);

    QSettings set;
    if(!set.value("RestartGame").toBool())
        w.show();

    return a.exec();
}
