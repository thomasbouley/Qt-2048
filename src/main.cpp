#include "startwindow.h"
#include "posixsighandle.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Thomas Bouley");
    a.setOrganizationDomain("bouley.com");
    a.setApplicationName("2048");

#ifdef POSIXSIGHANDLE
    POSIXsighandle sighand({SIGTERM,SIGHUP,SIGINT},&a);
    QObject::connect(&sighand,&POSIXsighandle::POSIXsig,&a,&QApplication::quit);
#endif

    StartWindow w;
    QObject::connect(&a,&QApplication::aboutToQuit,&w,&StartWindow::save_state);

    QSettings set;
    if(!set.value("RestartGame").toBool())
        w.show();

    return a.exec();
}
