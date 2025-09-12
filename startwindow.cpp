#include "startwindow.h"
#include <QSettings>
#include <QMenuBar>

#include <iostream>


StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setFixedSize(210, 80);

    setStyleSheet("QLabel {color: grey;"
                              "font: Helvetica;"
                              "font: bold 20px;}"
                  "QPushButton {color:  grey;"
                                 "font: Helvetica;"
                                 "font: bold 15px;"
                                 "background-color: lightgray;}");


    createActions();
    createMenus();

    int mh=0;
    if(!(menuBar()->isNativeMenuBar())){
        mh=menuBar()->size().height();
    }

    setFixedSize(210, 80+mh);

    infotext= new QLabel(tr("Enter board size:"), this);
    infotext->setGeometry(QRect(QPoint(10, 10+mh), QSize(163,25)));

    number= new QLineEdit("4", this);
    number->move(QPoint(183,10+mh));
    number->setFixedSize(17,25);
    number->setFont(QFont("Helvetica",15));

    validator=new QIntValidator(2,6,number);
    number->setValidator(validator);

    start= new QPushButton(tr("Start"), this);
    start->setGeometry(QRect(QPoint(20, 45+mh), QSize(60, 25)));
    start->setDefault(true);
    connect(start, &QPushButton::clicked, this, &StartWindow::start_pressed);


    cancel= new QPushButton(tr("Cancel"), this);
    cancel->setGeometry(QRect(QPoint(120, 45+mh), QSize(60, 25)));
    connect(cancel, &QPushButton::clicked, this, &StartWindow::cancel_pressed);

    bw=nullptr;

    QSettings set;
    try{
    if(set.value("RestartGame").toBool()){
        bw=new boardwindow(set.value("SavedGame").toString(),this);
        connect(bw,&boardwindow::game_end,this,&StartWindow::new_game);
        bw->show();
    }
    }
    catch(std::exception &e){
        delete bw;
        bw=nullptr;
        std::cerr<<"Game restart faled because of " << e.what() <<std::endl;
        set.setValue("RestartGame",false);
        set.sync();
    }

}

StartWindow::~StartWindow(){}

void StartWindow::createActions(){
    resetAct=new QAction(tr("Reset High Scores"),this);
    resetAct->setShortcut(QKeySequence("Ctrl+Shift+R"));
    connect(resetAct, &QAction::triggered,this,&StartWindow::reset_scores);
}

void StartWindow::createMenus(){
    fileMenu=menuBar()->addMenu(tr("File"));
    fileMenu->addAction(resetAct);

}

void StartWindow::start_pressed(){
    if(number->hasAcceptableInput()){
        bw=new boardwindow(number->text().toInt(),this);
        connect(bw,&boardwindow::game_end,this,&StartWindow::new_game);
        hide();
        bw->show();
    }

}

void StartWindow::cancel_pressed(){
    QApplication::quit();
}

void StartWindow::new_game(){
    delete bw;
    bw=nullptr;
    show();
}

void StartWindow::save_state(){
    QString str;
    QSettings settings;
    if(bw!=nullptr){
        str=bw->getgamestring();
        if(str.isEmpty()){
            settings.setValue("RestartGame",false);
        }
        else{
            settings.setValue("RestartGame",true);
            settings.setValue("SavedGame",str);
        }
    }
    else{
        settings.setValue("RestartGame",false);
    }
}

void StartWindow::reset_scores(){
    QSettings settings;
    settings.clear();
}



