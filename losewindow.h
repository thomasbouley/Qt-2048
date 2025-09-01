#ifndef LOSEWINDOW_H
#define LOSEWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

class loseWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit loseWindow( QWidget *parent = nullptr);
    ~loseWindow();

    QPushButton *newgamebutton;


public slots:
    void close_pressed();

signals:

private:
    QLabel *message;
    QPushButton *closebutton;
};


class winWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit winWindow( QWidget *parent = nullptr);
    ~winWindow();

    QPushButton *newgamebutton;


public slots:
    void close_pressed();
    void go_pressed();

signals:

private:
    QLabel *message;
    QPushButton *closebutton;
    QPushButton *contbutton;

};


#endif // LOSEWINDOW_H
