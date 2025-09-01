#include "startwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Thomas Bouley");
    a.setOrganizationDomain("bouley.com");
    a.setApplicationName("2048");


    StartWindow w;
    w.show();
    return a.exec();
}
