#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>


class board
{
public:
    board(unsigned int size);
    ~board();

    enum class direction;
    void updateboard(direction d);
    unsigned long getscore();
    unsigned int getsize();

    bool haswon();
    bool haslost();

    std::vector<std::vector<int>> getboardstate();

    direction getlastmove();
    std::vector<std::vector<int>> getanimationinfo();



private:
    const unsigned int bsize;
    unsigned long score;
    bool haswonb;
    std::vector<std::vector<int>> boardstate;

    direction lastmove;
    std::vector<std::vector<int>> animationinfo;


    unsigned int numempty;
    void addrandom();
    std::mt19937 gen;

    std::pair<int,int> _intexrotate(std::pair<int,int> p,direction d);
    void _zeroanimationinfo();


};


enum class board::direction{
    Up,
    Down,
    Left,
    Right
};


#endif // BOARD_H
