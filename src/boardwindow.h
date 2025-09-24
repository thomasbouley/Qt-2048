#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QKeyEvent>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <vector>

#include "board.h"
#include "losewindow.h"

class boardwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit boardwindow(int size=4,QWidget *parent = nullptr);
    explicit boardwindow(QString gamestatestring,QWidget *parent = nullptr);
    ~boardwindow();

    QString getgamestring();

public slots:
    void reset_highscore();
    void anim_finished(QAbstractAnimation::State , QAbstractAnimation::State );

signals:
    void game_end();

private:
    int boardsize;

    int tileshstart,tilesvstart,tilesize,tilespacing;
    int labelvstart;
    int winwidth,winhight;

    void setsizes();
    void setsizes(int newwidth);



    QLabel *scorelabel;
    QPushButton * newgamebutton;
    QLabel *highscorelabel;


    QLabel *boardlabel;
    std::vector<std::vector<QLabel *>> tiles;

    void initializewidgets();

    void initializelabels();
    void initializetiles();

    void resizelabels();
    void resizetiles();


    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *resetAct;
    QAction *newAct;


    void updatetiles();
    static const char styles[2][100];
    static const char styletable[13][70];

    int tilesfontsize(int value);

    void updatescore();

    void keyPressEvent(QKeyEvent *);

    void closeEvent(QCloseEvent *);

    void resizeEvent(QResizeEvent *);
    bool breakrize;

    board b;
    bool haswon;
    long highscore;

    loseWindow *lw;
    winWindow *ww;

    void initalizeanimations();
    void startanimations();
    std::vector<std::vector<QPropertyAnimation *>> animationtable;
    QParallelAnimationGroup *group;

};

#endif // BOARDWINDOW_H
