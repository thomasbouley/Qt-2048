#include "posixsighandle.h"

#ifdef POSIXSIGHANDLE

#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <system_error>


POSIXsighandle::POSIXsighandle(std::initializer_list<int> sigs,QObject *parent)
    : QObject(parent){
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigFd) != 0){
        throw std::system_error(errno, std::system_category(), "Failed to open socket for signal queue");
    }

    snSig = new QSocketNotifier(sigFd[1], QSocketNotifier::Read, this);
    connect(snSig, &QSocketNotifier::activated, this, &POSIXsighandle::readsigsocket);

    struct sigaction sa;
    sa.sa_handler = POSIXsighandle::signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_flags |= SA_RESTART;

    for(int sig : sigs){
        if(sigaction(sig, &sa, nullptr) != 0){
            throw std::system_error(errno, std::system_category(), "Failed to open SOCKET for SIGHUP");
        }
    }
}

void POSIXsighandle::signalHandler(int signo){
    ::write(sigFd[0],&signo,sizeof(int));
}

void POSIXsighandle::readsigsocket(){
    int sig;
    int err;
    snSig->setEnabled(false);
    ssize_t bytesr=0;
    bytesr=::read(sigFd[1],&sig,sizeof(int));
    err=errno;
    snSig->setEnabled(true);
    if(bytesr<0){
        throw std::system_error(err, std::system_category(), "Error reading signal queue");
    }
    else if(bytesr>0){
        emit POSIXsig(sig);
    }
}

int POSIXsighandle::sigFd[2]={0,0};

#endif //HAS_UNIX_SOCKETS
