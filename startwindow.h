#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

#include <QMainWindow>
#include <QPushButton>
#include <QMenu>
#include <QKeyEvent>
#include <QLabel>
#include <QApplication>
#include <QLineEdit>
#include <QIntValidator>

#include "boardwindow.h"


class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();


public slots:
    void start_pressed();
    void cancel_pressed();
    void new_game();
    void save_state();

private:
    QLabel *infotext;
    QLineEdit *number;
    QValidator *validator;
    QPushButton *start;
    QPushButton *cancel;

    boardwindow *bw;
};
#endif // STARTWINDOW_H
