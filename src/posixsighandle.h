#ifdef HAS_UNIX_SOCKETS
#ifndef POSIXSIGHANDLE_H
#define POSIXSIGHANDLE_H

#define POSIXSIGHANDLE

#include <csignal>

#include <QObject>
#include <QSocketNotifier>

#include <initializer_list>

class POSIXsighandle : public QObject
{
    Q_OBJECT

public:
    POSIXsighandle(std::initializer_list<int> sigs,QObject *parent = nullptr);

    // Unix signal handlers.
    static void signalHandler(int);

signals:
    void POSIXsig(int signo);

private slots:
    void readsigsocket();

private:

    static int sigFd[2];

    QSocketNotifier *snSig;
};

#endif // POSIXSIGHANDLE_H
#endif //HAS_UNIX_SOCKETS
