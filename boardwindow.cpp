#include "boardwindow.h"
#include <QSettings>
#include <QMenuBar>
#include <QApplication>

#include <cmath>
#include <iostream>

boardwindow::boardwindow(int size,QWidget *parent)
    : QMainWindow{parent},
    boardsize{size},
    b{(unsigned int) boardsize},
    tiles{(unsigned long) boardsize,std::vector<QLabel *>(boardsize,nullptr)},
    animationtable{(unsigned long) boardsize,std::vector<QPropertyAnimation *>(boardsize,nullptr)}{

    initializewidgets();

}

boardwindow::boardwindow(QString gamestring,QWidget *parent)
    : QMainWindow{parent},
    b{0}{
    QStringList toplist=gamestring.split("|");
    if(toplist.size()!=3)
        throw(std::runtime_error("Corrupt Saved Game"));

    boardsize=toplist[0].toInt();
    int score=toplist[1].toInt();

    QStringList list=toplist[2].split("\n");
    std::vector<std::vector<unsigned int>> newboard;
    QStringList listrow;
    std::vector<unsigned int> row;

    for(QStringList::iterator outer=list.begin();outer!=list.end();outer++){
        listrow=outer->split(",");
        for(QStringList::iterator inner=listrow.begin();inner!=listrow.end();inner++){
            row.push_back(inner->toInt());
        }
        newboard.push_back(row);
        row.clear();
    }
    b=board(boardsize,score,newboard);

    tiles=std::vector<std::vector<QLabel *>>((unsigned long) boardsize,std::vector<QLabel *>(boardsize,nullptr));
    animationtable=std::vector<std::vector<QPropertyAnimation *>>((unsigned long) boardsize,std::vector<QPropertyAnimation *>(boardsize,nullptr));
    initializewidgets();

    updatescore();
}

boardwindow::~boardwindow(){}//All QWidget * members have this as parent

void boardwindow::initializewidgets(){
    breakrize=false ;

    setsizes();

    setMinimumWidth(340);
    resize(QSize(winwidth,winhight));

    initializelables();

    initializetiles();

    initalizeanimations();

    updatetiles();

    createActions();
    createMenus();

    haswon=b.haswon();
}


void boardwindow::updatetiles(){
    std::vector<std::vector<unsigned int>> bs=b.getboardstate();
    int val=0;
    QString str;
    int fontsize;
    for(int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            val=bs[i][j];
            if(val==0){
                tiles[i][j]->hide();
            }
            else if(val<13){
                tiles[i][j]->setText(str.setNum(1L << val));
                fontsize=tilesfontsize(val);
                tiles[i][j]->setStyleSheet(QString(styles[0])+std::to_string(fontsize).c_str()+styles[1]+styletable[val-1]);
                tiles[i][j]->show();
            }
            else{
                tiles[i][j]->setText(str.setNum(1L << val));
                fontsize=tilesfontsize(val);
                tiles[i][j]->setStyleSheet(QString(styles[0])+std::to_string(fontsize).c_str()+styles[1]+styletable[12]);
                tiles[i][j]->show();
            }
        }
    }
}

int boardwindow::tilesfontsize(int value){
    int deffsize;
    int numdigits;
    numdigits=std::ceil(.30103 * value);//.30103==log10(2)
    if(numdigits>4){
         deffsize=std::round(30 * 4.0 / numdigits);
    }
    else{
        deffsize=30;
    }
    return std::round(deffsize * tilesize / 80.0);
}

void boardwindow::setsizes(){
    tileshstart=20,tilesvstart=75;
    tilesize=80;
    tilespacing=tilesize+10;

    winhight=tilesvstart+boardsize* tilespacing+10;
    winwidth=2*tileshstart+boardsize* tilespacing - 10;

    if(winwidth<340){
        winwidth=340;
        tileshstart=(winwidth-boardsize* tilespacing)/2;
    }

}

void boardwindow::setsizes(int newwidth){
    winwidth=newwidth;

    tilespacing=std::round(((double) winwidth- 2 * tileshstart +10)/boardsize);
    tilesize=tilespacing-10;

    winhight=tilesvstart+boardsize* tilespacing+10;
}


void boardwindow::resizeEvent(QResizeEvent *event){
    setsizes(event->size().width());
    resizelables();
    resizetiles();
    updatetiles();
    if(!breakrize){
        breakrize=true;
        resize(QSize(winwidth,winhight));
    }
    else{
        breakrize=false;
    }
    QMainWindow::resizeEvent(event);
}

void boardwindow::keyPressEvent(QKeyEvent *event){
    group->stop();
    bool didmove=false;
    int keyPressed=event->key();
    switch (keyPressed) {
    case Qt::Key_W:
    case Qt::Key_Up:
        didmove=b.updateboard(board::direction::Up);
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        didmove=b.updateboard(board::direction::Down);
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        didmove=b.updateboard(board::direction::Left);
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        didmove=b.updateboard(board::direction::Right);
        break;
    default:
        break;
    }
    if(didmove)
        startanimations();
    updatescore();
    if(b.haswon() && !haswon){
        ww=new winWindow(this);
        connect(ww->newgamebutton,&QPushButton::clicked,this,&boardwindow::game_end);
        haswon=true;
        ww->show();
    }
    if(b.haslost()){
        lw=new loseWindow(this);
        connect(lw->newgamebutton,&QPushButton::clicked,this,&boardwindow::game_end);
        lw->show();
    }
    QWidget::keyPressEvent(event);
}

void boardwindow::closeEvent(QCloseEvent *event){
    QApplication::exit();
    event->accept();
}


void boardwindow::updatescore(){
    unsigned long score=b.getscore();
    QString str;
    str.setNum(score);
    scorelabel->setText(tr("Score: \n")+str);
    if(score>=highscore){
        QSettings settings;
        QString sizestr;
        sizestr.setNum(boardsize);
        highscore=score;
        settings.setValue(sizestr+"/HighScore",(int)highscore);
        highscorelabel->setText(tr("HighScore: \n")+str);
    }
}

void boardwindow::initializetiles(){
    for(int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            tiles[i][j]=new QLabel((std::to_string(i)+" , "+std::to_string(j)).c_str(),this);
            tiles[i][j]->setGeometry(tileshstart + j*tilespacing, tilesvstart + i*tilespacing,tilesize,tilesize);
            tiles[i][j]->setFrameShape(QFrame::Panel);
            tiles[i][j]->setAlignment(Qt::AlignCenter);
            tiles[i][j]->setStyleSheet("background-color: rgb(224, 0, 0);");

        }
    }
}

void boardwindow::initializelables(){
    scorelabel=new QLabel(tr("Score\n 0"),this);
    scorelabel->setGeometry(10,15,100,40);
    scorelabel->setAlignment(Qt::AlignCenter);

    newgamebutton=new QPushButton(tr("New Game"),this);
    newgamebutton->setGeometry(winwidth/2-50,15,100,40);
    connect(newgamebutton,&QPushButton::clicked,this,&boardwindow::game_end);


    QSettings settings;
    QString str;
    str.setNum(boardsize);
    highscore=settings.value(str+"/HighScore").toInt();

    str.setNum(highscore);
    highscorelabel=new QLabel(tr("High Score\n")+str,this);
    highscorelabel->setGeometry(winwidth-100-10,15,100,40);
    highscorelabel->setAlignment(Qt::AlignCenter);

    boardlabel=new QLabel(this);
    boardlabel->setGeometry(tileshstart-10,tilesvstart-10,tilespacing*boardsize+10,tilespacing*boardsize+10);
    boardlabel->setFrameShape(QFrame::Panel);
    boardlabel->setStyleSheet("border-style: solid;"
                              "border-width: 5px;"
                              "border-radius: 20px;"
                              "border-color: grey");
}


void boardwindow::resizetiles(){
    for(int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            tiles[i][j]->setGeometry(tileshstart + j*tilespacing, tilesvstart + i*tilespacing,tilesize,tilesize);
        }
    }
}

void boardwindow::resizelables(){
    newgamebutton->setGeometry(winwidth/2-50,15,100,40);
    highscorelabel->setGeometry(winwidth-100-10,15,100,40);
    boardlabel->setGeometry(tileshstart-10,tilesvstart-10,tilespacing*boardsize+10,tilespacing*boardsize+10);
}

std::string int_arr_2d_to_string(std::vector<std::vector<unsigned int>> arr_2d){
    std::string str;
    for(std::vector<std::vector<unsigned int>>::iterator outer=arr_2d.begin();
         outer!=arr_2d.end();outer++){
        for(std::vector<unsigned int>::iterator inner=outer->begin();inner != outer->end();inner++ ){
            str+=std::to_string(*inner)+",";
        }
        str.pop_back();
        str.push_back('\n');
    }
    str.pop_back();
    return  str;
}

QString boardwindow::getgamestring(){
    QString gamestate,temp;

    if(b.haslost())
        return gamestate;

    temp.setNum(b.getsize());
    gamestate+=temp+"|";
    temp.setNum(b.getscore());
    gamestate+=temp+"|";
    gamestate+=int_arr_2d_to_string(b.getboardstate());

    return gamestate;
}

void boardwindow::createActions(){
    resetAct=new QAction(tr("Reset High Scores"),this);
    resetAct->setShortcut(QKeySequence("Ctrl+Shift+R"));
    connect(resetAct, &QAction::triggered,this,&boardwindow::reset_highscore);
    newAct=new QAction(tr("New Game"),this);
    newAct->setShortcut(QKeySequence("Ctrl+N"));
    connect(newAct, &QAction::triggered,this,&boardwindow::game_end);
}

void boardwindow::createMenus(){
    fileMenu=menuBar()->addMenu(tr("File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(resetAct);
}


void boardwindow::reset_highscore(){
    QSettings settings;
    settings.clear();
    highscore=0;
    updatescore();
}


void boardwindow::initalizeanimations(){
    group=new QParallelAnimationGroup(this);
    for(int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            animationtable[i][j]=new QPropertyAnimation(tiles[i][j],"pos",this);
            group->addAnimation(animationtable[i][j]);
        }
    }
    connect(group,&QParallelAnimationGroup::stateChanged,this,&boardwindow::anim_finished);
}

void boardwindow::startanimations(){
    board::direction ld=b.getlastmove();
    QPoint dir;
    switch (ld) {
    case board::direction::Up:
        dir.setX(0);
        dir.setY(-tilesize);
        break;
    case board::direction::Down:
        dir.setX(0);
        dir.setY(tilesize);
        break;
    case board::direction::Left:
        dir.setX(-tilesize);
        dir.setY(0);
        break;
    case board::direction::Right:
        dir.setX(tilesize);
        dir.setY(0);
        break;
    }

    std::vector<std::vector<int>> amounts=b.getanimationinfo();
    QPoint cur;
    for(int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            cur=QPoint(tileshstart + j*tilespacing, tilesvstart + i*tilespacing);
            animationtable[i][j]->setDuration(50);
            animationtable[i][j]->setStartValue(cur);
            animationtable[i][j]->setEndValue(cur+amounts[i][j]*dir);
        }
    }
    group->start();
}

void boardwindow::anim_finished(QAbstractAnimation::State newState, QAbstractAnimation::State oldState){
    if(newState==QAbstractAnimation::Stopped){
        resizetiles();
        updatetiles();
    }
}

const char boardwindow::styletable[13][70]={"color: rgb(118,  110, 102); background-color: rgb(236, 228, 219)",
                                 "color: rgb(118, 110, 102); background-color: rgb(235, 224, 203)",
                                 "color: rgb(249, 246, 242); background-color: rgb(233, 179, 129)",
                                 "color: rgb(249, 246, 242); background-color: rgb(232, 153, 108)",
                                 "color: rgb(249, 246, 242); background-color: rgb(231, 130, 103)",
                                 "color: rgb(249, 246, 242); background-color: rgb(229, 104, 71 )",
                                 "color: rgb(249, 246, 242); background-color: rgb(232, 207, 127)",
                                 "color: rgb(249, 246, 242); background-color: rgb(232, 204, 114)",
                                 "color: rgb(249, 246, 242); background-color: rgb(231, 200, 101)",
                                 "color: rgb(249, 246, 242); background-color: rgb(231, 197, 89 )",
                                 "color: rgb(249, 246, 242); background-color: rgb(231, 195, 79 )",
                                 "color: rgb(249, 246, 242); background-color: rgb(60 , 58 , 51 )",
                                 "color: rgb(249, 246, 242); background-color: rgb(45 , 44 , 38 )"};

const char boardwindow::styles[2][100]={"font: Helvetica;"
                                      "font: bold ","px;"
                                      "border-radius: 10px;"
                                      "border-style: outset;"};

