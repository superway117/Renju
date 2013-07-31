#ifndef __BoardKeeper_H__
#define __BoardKeeper_H__

#include "cocos2d.h"
#include "Bead.h"
#include "BoardConfig.h"

USING_NS_CC;

#define MAX_STORE_BOARD_NUM 10

class BoardData
{
public:
    BoardData();
    BoardData(const BoardData& other );
    void reset();
    void resetBoard();
    bool isValid();

    int m_score;
    int m_beads[BEADBOARD_SIZE*BEADBOARD_SIZE];

};

class BoardStore
{
public:
    static int getHighScore();

    static void storeHighScore(int score);

    static int getIndex();

    static void storeIndex(int index);

    static bool hasValidStore();

    static bool store(int index, BoardData& data);

    static bool restore(int index,BoardData& data);

    static void invalidAll();

    static void invalid(int index);

};

class BoardKeeper
{
public:
    BoardKeeper();

    void init(bool is_new);

    bool push(Bead** board,int score);
    bool push(int score);
    bool setBoardData(Bead** board);

    bool pop();
    int  getCurScore();
    const BoardData&  getCurBoard();

    int getHighScore();
    void storeHighScore(int score);
    void reset();

    bool store(int index);
    bool restore(int index);

    bool storeAll();
    bool restoreAll();

    void invalidAll();

    bool hasValidStore();

private:
    void nextIndex();
    void prevIndex();

    int getPrevIndex();

    bool isPrevValid();
    bool isCurValid();

    void reset(int idx);

    void storeCurIndex();

    void restoreCurIndex();

    void saveContext();

    void invalidCurrent();



private:
    int m_hightScore;

    int m_index;
    BoardData m_data[MAX_STORE_BOARD_NUM];

};

#endif /* BoardKeeper */
