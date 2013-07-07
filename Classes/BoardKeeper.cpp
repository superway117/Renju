#include "BoardKeeper.h"

using namespace std;

BoardData::BoardData()
{
    reset();
}

BoardData::BoardData(const BoardData& other )
{
    m_score = other.m_score;
    memcpy(m_beads, other.m_beads,sizeof(m_beads));
}
void BoardData::reset()
{
    m_score = -1;
    memset(m_beads , -1, sizeof(m_beads));
}
void BoardData::resetBoard()
{
    memset(m_beads , -1, sizeof(m_beads));
}

bool BoardData::isValid()
{
    return (m_score >=0 );

}

BoardKeeper::BoardKeeper():m_index(-1)
{
    m_hightScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("highscore", 0);
    reset();
}

void BoardKeeper::nextIndex()
{
    if(m_index+1>=MAX_STORE_BOARD_NUM)
        m_index = 0;
    else
        m_index++;
}
int BoardKeeper::getPrevIndex()
{
    if(m_index==0)
        return MAX_STORE_BOARD_NUM-1;
    else
        return m_index-1;
}
void BoardKeeper::prevIndex()
{
    m_index = getPrevIndex();
}
bool BoardKeeper::isPrevValid()
{
    int idx = getPrevIndex();
    return m_data[idx].isValid();
}
bool BoardKeeper::isCurValid()
{
    return m_data[m_index].isValid();
}

void BoardKeeper::reset()
{
    for(int i=0;i<MAX_STORE_BOARD_NUM;i++)
        m_data[i].reset();
    m_index = -1;

}
void BoardKeeper::reset(int idx)
{
    m_data[idx].reset();
}
bool BoardKeeper::push(Bead** board,int score)
{
    //CCAssert(m_index>=0&&m_index<MAX_STORE_BOARD_NUM,"wrong index");
    nextIndex();
    CCLOGERROR("BoardKeeper::push m_index=%d\n",m_index);
    m_data[m_index].reset();
    m_data[m_index].m_score = score;
    setBoardData(board);
    if(score>m_hightScore)
    {
        storeHighScore(score);
    }
    return true;
}

bool BoardKeeper::push(int score)
{
    return push(NULL,score);
}
bool BoardKeeper::setBoardData(Bead** board)
{
    CCAssert(isCurValid(),"cur is not valid");
    int* array= m_data[m_index].m_beads;
    if(board)
    {
        m_data[m_index].resetBoard();
        for(int i=0; i<BEADBOARD_SIZE*BEADBOARD_SIZE ; i++)
        {
            if(board[i])
            {
                array[i]=(int)board[i]->getBeadColor();
            }

        }
    }
    return true;
}
bool BoardKeeper::pop()
{

    //TODO: need think about how to restore the highscore
    if(isPrevValid())
    {
        m_data[m_index].reset();
        prevIndex();
        CCLOGERROR("BoardKeeper::pop m_index=%d",m_index);
        return true;
    }
    return false;

}
int BoardKeeper::getHighScore()
{
    return m_hightScore;
}
const BoardData& BoardKeeper::getCurBoard()
{
    CCAssert(isCurValid(),"cur is not valid");
    CCLOGERROR("BoardKeeper::getCurBoard m_index=%d",m_index);
    return m_data[m_index];

}

void BoardKeeper::storeHighScore(int score)
{
    m_hightScore=score;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("highscore", m_hightScore);
}


int  BoardKeeper::getCurScore()
{
    CCAssert(isCurValid(),"cur is not valid");
    return m_data[m_index].m_score;
}

void BoardKeeper::storeCurIndex()
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("index", m_index);
}

void BoardKeeper::restoreCurIndex()
{
    CCUserDefault::sharedUserDefault()->getIntegerForKey("index", -1);
}

bool BoardKeeper::store(int index)
{
    char key[10];
    std::string value;
    sprintf(key,"board%d",index);
    if(!m_data[index].isValid())
    {
        value = "";
    }
    else
    {
        stringstream ss;
        ss<<m_data[index].m_score;
        for(int i=0;i<BEADBOARD_SIZE*BEADBOARD_SIZE;i++)
            ss<<m_data[index].m_beads[i];
        value = ss.str();
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(key, value);
    return true;

}

bool BoardKeeper::restore(int index)
{
    char key[10];
    sprintf(key,"board%d",index);
    std::string value = CCUserDefault::sharedUserDefault()->getStringForKey(key);
    if(value == "")
    {
        m_data[index].reset();
    }
    else
    {
        const char* str = value.c_str();
        size_t len = value.length();
        char tmp[4];
        memset(tmp,0, sizeof(tmp));
        CCAssert(len==BEADBOARD_SIZE*BEADBOARD_SIZE+1,"wrong length");
        tmp[0]=str[0];
        m_data[index].m_score=atoi(tmp);
        for(int i=1;i<BEADBOARD_SIZE*BEADBOARD_SIZE+1;i++)
        {
            tmp[0]=str[i];
            m_data[index].m_beads[i]=atoi(tmp);
        }
    }

    return true;
}

bool BoardKeeper::storeAll()
{
    storeCurIndex();
    for(int i=0;i<MAX_STORE_BOARD_NUM;i++)
    {
        if(m_data[i].isValid())
            store(i);
    }
    return true;
}
bool BoardKeeper::restoreAll()
{
    restoreCurIndex();
    for(int i=0;i<MAX_STORE_BOARD_NUM;i++)
    {
        restore(i);
    }
    return true;
}

