#include "losewindow.h"
#include <QApplication>

loseWindow::loseWindow(QWidget *parent)
    : QMainWindow{parent}{

    setFixedSize(250, 110);

    message=new QLabel(tr("Game Over!"),this);
    message->setGeometry(65,10,120,25);

    newgamebutton=new QPushButton("New Game",this);
    newgamebutton->setGeometry(10,50,100,50);

    closebutton=new QPushButton("Exit",this);
    closebutton->setGeometry(140,50,100,50);
    connect(closebutton,&QPushButton::released,this,&loseWindow::close_pressed);

}


loseWindow::~loseWindow(){}//All QWidget * members have this as parent

void loseWindow::close_pressed(){
    QApplication::quit();
}







winWindow::winWindow(QWidget *parent)
    : QMainWindow{parent}{

    setFixedSize(400, 150);

    message=new QLabel(tr("Game Won!"),this);
    message->setGeometry(145,10,110,25);

    contbutton=new QPushButton(tr("Contune Playing"),this);
    contbutton->setGeometry(10,50,150,50);
    connect(contbutton,&QPushButton::pressed,this,&winWindow::go_pressed);

    newgamebutton=new QPushButton(tr("New Game"),this);
    newgamebutton->setGeometry(170,50,100,50);

    closebutton=new QPushButton(tr("Exit"),this);
    closebutton->setGeometry(280,50,100,50);
    connect(closebutton,&QPushButton::pressed,this,&winWindow::close_pressed);

}


winWindow::~winWindow(){}//All QWidget * members have this as parent

void winWindow::close_pressed(){
    QApplication::quit();
}

void winWindow::go_pressed(){
    close();
}
