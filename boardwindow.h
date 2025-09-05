#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QKeyEvent>


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

signals:
    void game_end();

private:
    int boardsize;

    int tileshstart,tilesvstart,tilesize,tilespacing;
    int winwidth,winhight;

    void setsizes();



    QLabel *scorelabel;
    QPushButton * newgamebutton;
    QLabel *highscorelabel;


    QLabel *boardlabel;
    std::vector<std::vector<QLabel *>> tiles;

    void initializelables();
    void initializetiles();


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

    board b;
    bool haswon;
    long highscore;

    loseWindow *lw;
    winWindow *ww;
};

#endif // BOARDWINDOW_H
