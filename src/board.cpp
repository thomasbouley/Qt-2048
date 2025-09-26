#include "board.h"

#include <utility>
#include <stdexcept>

#include <cstdint>

board::board(unsigned int size):
    bsize{size},
    boardstate(size,std::vector<unsigned int>(size,0)),
    animationinfo(size,std::vector<int>(size,0)){
    score=0;
    haswonb=false;
    numempty=size*size;
    gen.seed((uintptr_t) this);
    addrandom();
    addrandom();

    lastmove=direction::Up;//Ensure that getlastmove returns valid direction
}

board::board(unsigned int size, unsigned long nscore,std::vector<std::vector<unsigned int>> bs):
    bsize{size},
    boardstate(bs),
    animationinfo(size,std::vector<int>(size,0)){
    score=nscore;
    if(bsize!=bs.size())
        throw(std::runtime_error("Inconsistent Board"));
    for(int i=0;i<bsize;i++){
        if(bsize!=bs[i].size())
            throw (std::runtime_error("Inconsistent Board"));
    }

    unsigned long maxscore=0;
    unsigned long minscore=0;

    haswonb=false;
    numempty=0;

    unsigned int val;
    for(int i=0;i<bsize;i++){
        for(int j=0;j<bsize;j++){
            val=boardstate[i][j];
            if(val==0)
                numempty++;
            else if(val>1){
                maxscore+= (1L << val)*(val-1);
                minscore+= (1L << val)*(val-2);
            }
            if(val>=11)
                haswonb=true;

        }
    }

    if(score>maxscore || score<minscore)
        throw (std::runtime_error("Inconsistent Board"));//Ensure that the score is from a possible game

    lastmove=direction::Up;//Ensure that getlastmove returns valid direction

}

board::~board(){}





bool board::updateboard(direction d){

    _zeroanimationinfo();

    lastmove=d;
    int lastpos,lastvalue,curvalue,ai,aj,lai,laj;
    std::pair<int,int> ap,lap;

    bool didmove=false;

    for(int j=0;j<bsize;j++){
        lastvalue=0;
        lastpos=-1;
        for(int i=0;i<bsize;i++){
            ap=_intexrotate(std::make_pair(i,j),d);
            ai=ap.first;
            aj=ap.second;
            curvalue=boardstate[ai][aj];
            if(curvalue!=0 && lastvalue!=curvalue){
                lastpos++;
                lap=_intexrotate(std::make_pair(lastpos,j),d);
                lai=lap.first;
                laj=lap.second;
                boardstate[ai][aj]=0;
                boardstate[lai][laj]=curvalue;
                lastvalue=curvalue;
                animationinfo[ai][aj]= i-lastpos;
                if(lastpos!=i)
                    didmove=true;
            }
            else if(curvalue!=0 && lastvalue==curvalue){
                boardstate[lai][laj]++;
                boardstate[ai][aj]=0;
                lastvalue=0;
                numempty++;
                animationinfo[ai][aj]= i-lastpos;
                didmove=true;
                score+= 1L << (curvalue+1);
                if(curvalue==10)
                    haswonb=true;
            }
        }
    }
    if(didmove)
        addrandom();

    return didmove;
}

bool board::haslost(){
    int prev,curr;
    if(numempty==0){
        for(int i=0;i<bsize;i++){
            prev=boardstate[i][0];
            for(int j=1;j<bsize;j++){
                curr=boardstate[i][j];
                if(prev==curr){
                    return false;
                }
                prev=curr;
            }
        }

        for(int j=0;j<bsize;j++){
            prev=boardstate[0][j];
            for(int i=1;i<bsize;i++){
                curr=boardstate[i][j];
                if(prev==curr){
                    return false;
                }
                prev=curr;
            }
        }

        return true;
    }
    return false;
}

bool board::haswon(){
    return haswonb;
}


void board::addrandom(){
    if(numempty==0) return;
    std::uniform_int_distribution<int> dis1(1,numempty);
    int wempty=dis1(gen);

    int newvalue=1;
    std::uniform_int_distribution<int> dis2(1,10);
    if(dis2(gen)==1)
        newvalue=2;

    int cempty=0;
    bool break_outer=false;
    for(int i=0;(i<bsize)&&(!break_outer);i++){
        for(int j=0;j<bsize;j++){
            if(boardstate[i][j]==0)
                cempty++;
            if(cempty==wempty){
                boardstate[i][j]=newvalue;
                numempty--;
                break_outer=true;
                break;
            }
        }
    }

}



unsigned long board::getscore(){
    return score;
}

std::vector<std::vector<unsigned int>> board::getboardstate(){
    return boardstate;
}

unsigned int board::getsize(){
    return bsize;
}

board::direction board::getlastmove(){
    return lastmove;
}

std::vector<std::vector<int>> board::getanimationinfo(){
    return animationinfo;
}

std::pair<int,int> board::_intexrotate(std::pair<int,int> p,direction d){
    int i=p.first;
    int j=p.second;
    switch (d) {
    case board::direction::Up :
        return std::make_pair(i,j);
        break;
    case board::direction::Right :
        return std::make_pair(j,bsize-i-1);
        break;
    case board::direction::Down :
        return std::make_pair(bsize-i-1,bsize-j-1);
        break;
    case board::direction::Left :
        return std::make_pair(bsize-j-1,i);
        break;
    }
    return std::make_pair(0,0);//Should never happen
}

void board::_zeroanimationinfo(){
    for(int i=0;i<bsize;i++){
        for(int j=0;j<bsize;j++){
            animationinfo[i][j]=0;
        }
    }
}

