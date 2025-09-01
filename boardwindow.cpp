#include "boardwindow.h"
#include <QSettings>

boardwindow::boardwindow(int size,QWidget *parent)
    : QMainWindow{parent},
    boardsize{size},
    b{(unsigned int) boardsize},
    tiles{(unsigned long) boardsize,std::vector<QLabel *>(boardsize,nullptr)}{

    QSettings settings;
    QString str;
    str.setNum(boardsize);
    highscore=settings.value(str+"/HighScore").toInt();

    tileshstart=20,tilesvstart=75;
    tilesize=80;
    tilespacing=tilesize+10;

    int hight=2*tilesvstart+boardsize* tilespacing;
    int width=2*tileshstart+boardsize* tilespacing;

    resize(QSize(width,hight));

    scorelabel=new QLabel(tr("Score\n 0"),this);
    scorelabel->setGeometry(10,15,100,40);
    scorelabel->setAlignment(Qt::AlignCenter);

    newgamebutton=new QPushButton(tr("New Game"),this);
    newgamebutton->setGeometry(width/2-50,15,100,40);
    connect(newgamebutton,&QPushButton::pressed,this,&boardwindow::game_end);

    str.setNum(highscore);
    highscorelabel=new QLabel(tr("High Score\n")+str,this);
    highscorelabel->setGeometry(width-100-10,15,100,40);
    highscorelabel->setAlignment(Qt::AlignCenter);

    boardlabel=new QLabel(this);
    boardlabel->setGeometry(tileshstart-10,tilesvstart-10,tilespacing*boardsize+10,tilespacing*boardsize+10);
    boardlabel->setFrameShape(QFrame::Panel);
    boardlabel->setStyleSheet("border-style: solid;"
                              "border-width: 5px;"
                              "border-radius: 20px;"
                              "border-color: grey");


    initializetiles();

    updatetiles();

}

boardwindow::~boardwindow(){}//All QWidget * members have this as parent

void boardwindow::updatetiles(){
    std::vector<std::vector<int>> bs=b.getboardstate();
    int val=0;
    QString str;
    for(int i=0;i<boardsize;i++){
        for(int j=0;j<boardsize;j++){
            val=bs[i][j];
            if(val==0){
                tiles[i][j]->hide();
            }
            else if(val<13){
                tiles[i][j]->setText(str.setNum(1L << val));
                tiles[i][j]->show();
                tiles[i][j]->setStyleSheet(QString(styles)+styletable[val-1]);
            }
            else{
                tiles[i][j]->setText(str.setNum(1L << val));
                tiles[i][j]->show();
                tiles[i][j]->setStyleSheet(QString(styles)+styletable[12]);
            }
        }
    }
}

void boardwindow::keyPressEvent(QKeyEvent *event){
    int keyPressed=event->key();
    switch (keyPressed) {
    case Qt::Key_W:
    case Qt::Key_Up:
        b.updateboard(board::direction::Up);
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        b.updateboard(board::direction::Down);
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        b.updateboard(board::direction::Left);
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        b.updateboard(board::direction::Right);
        break;
    default:
        break;
    }
    updatetiles();
    updatescore();
    if(b.haswon() && !haswon){
        ww=new winWindow(this);
        connect(ww->newgamebutton,&QPushButton::pressed,this,&boardwindow::game_end);
        haswon=true;
        ww->show();
    }
    if(b.haslost()){
        lw=new loseWindow(this);
        connect(lw->newgamebutton,&QPushButton::pressed,this,&boardwindow::game_end);
        lw->show();
    }
    QWidget::keyPressEvent(event);
}

void boardwindow::updatescore(){
    unsigned long score=b.getscore();
    QString str;
    str.setNum(score);
    scorelabel->setText(tr("Score: \n")+str);
    if(score>highscore){
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

const char boardwindow::styles[100]="font: Helvetica;"
                                      "font: bold 30px;"
                                      "border-radius: 10px;"
                                      "border-style: outset;";

