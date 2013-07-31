#include "BoardKeeper.h"
#include "User.h"


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

int BoardStore::getHighScore()
{
    return User::getHighScore();
    //return CCUserDefault::sharedUserDefault()->getIntegerForKey("highscore", 0);

}

void BoardStore::storeHighScore(int score)
{
    CCAssert(score>=0,"highscore should >= 0");
    #if 0
    CCUserDefault::sharedUserDefault()->setIntegerForKey("highscore", score);
    #endif

    User::setHighScore(score);
}


int BoardStore::getIndex()
{
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("index", -1);
}

void BoardStore::storeIndex(int index)
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("index", index);
}

bool BoardStore::hasValidStore()
{
    CCLOG("BoardStore::hasValidStore index=%d",BoardStore::getIndex());
    return (BoardStore::getIndex()>=0);
}
bool BoardStore::store(int index, BoardData& data)
{
    char key[10];
    static char value[200];
    //std::string value;
    sprintf(key,"board%d",index);
    if(!data.isValid())
    {
        strcpy(value,"");
    }
    else
    {
        char tmp[4];
        memset(tmp,0, sizeof(tmp));
        memset(value,0, sizeof(value));
        int offset = 0;

        for(int i=0;i<BEADBOARD_SIZE*BEADBOARD_SIZE;i++)
        {
            int color = (int)data.m_beads[i];
            sprintf(value+offset,"%d",color);
            if(color==-1)
                offset+=2;
            else
                offset++;
        }
        sprintf(value+offset,"%d",data.m_score);
        CCLOG("BoardStore::store value=%s",value);
    }
    CCUserDefault::sharedUserDefault()->setStringForKey(key, value);
    return true;

}

bool BoardStore::restore(int index,BoardData& data)
{
    char key[10];
    sprintf(key,"board%d",index);
    std::string value = CCUserDefault::sharedUserDefault()->getStringForKey(key);
    if(value == "")
    {
        data.reset();
    }
    else
    {
        const char* str = value.c_str();
        size_t len = value.length();
        char tmp[4];
        memset(tmp,0, sizeof(tmp));
        CCLOG("BoardStore::restore idx=%d str=%s",index,str);
        CCAssert(len>BEADBOARD_SIZE*BEADBOARD_SIZE,"wrong length");

        int offset=0;
        for(int i=0;i<BEADBOARD_SIZE*BEADBOARD_SIZE;i++)
        {
            if(str[offset]=='-')
            {
                tmp[0]=str[offset++];
                tmp[1]=str[offset++];
            }
            else
            {
                tmp[0]=str[offset++];
                tmp[1]='\0';
            }
            data.m_beads[i]=atoi(tmp);
            CCLOG("BoardStore::restore i=%d color=%d",i,atoi(tmp));
        }
        data.m_score=atoi(str+offset);

    }

    return true;
}

void BoardStore::invalid(int index)
{
    char key[10];
    sprintf(key,"board%d",index);
    CCUserDefault::sharedUserDefault()->setStringForKey(key, "");
}
void BoardStore::invalidAll()
{
    BoardStore::storeIndex(-1);
    char key[10];
    for(int i=0;i<MAX_STORE_BOARD_NUM;i++)
    {
        sprintf(key,"board%d",i);
        CCUserDefault::sharedUserDefault()->setStringForKey(key, "");
    }
}

BoardKeeper::BoardKeeper()
{
    //m_hightScore = BoardStore::getHighScore();
    reset();
    
}

void BoardKeeper::init(bool is_new)
{
    if(is_new || !hasValidStore())
    {
        CCLOG("BoardKeeper::init new");
        BoardStore::invalidAll();
    }
    else
    {
        CCLOG("BoardKeeper::init continues");
        restoreAll();  
    }
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
    CCLOG("BoardKeeper::push m_index=%d\n",m_index);
    m_data[m_index].reset();
    m_data[m_index].m_score = score;
    setBoardData(board);
    if(score>getHighScore())
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
    saveContext();
    return true;
}
bool BoardKeeper::pop()
{

    //TODO: need think about how to restore the highscore
    if(isPrevValid())
    {
        invalidCurrent();
        prevIndex();
        CCLOG("BoardKeeper::pop m_index=%d",m_index);
        storeCurIndex();
        return true;
    }
    return false;

}
int BoardKeeper::getHighScore()
{

    return BoardStore::getHighScore();

}
const BoardData& BoardKeeper::getCurBoard()
{
    CCAssert(isCurValid(),"cur is not valid");
    CCLOGERROR("BoardKeeper::getCurBoard m_index=%d",m_index);
    return m_data[m_index];

}

void BoardKeeper::storeHighScore(int score)
{
    //m_hightScore=score;
    BoardStore::storeHighScore(score);
}


int  BoardKeeper::getCurScore()
{
    CCAssert(isCurValid(),"cur is not valid");
    return m_data[m_index].m_score;
}

void BoardKeeper::saveContext()
{
    if(isCurValid())
    {
        storeCurIndex();
        BoardStore::store(m_index,m_data[m_index]);
    }
}
void BoardKeeper::invalidCurrent()
{
    m_data[m_index].reset();
    BoardStore::invalid(m_index);
}

void BoardKeeper::storeCurIndex()
{
    CCLOG("BoardKeeper::storeCurIndex m_index=%d",m_index);
    BoardStore::storeIndex( m_index);
}

void BoardKeeper::restoreCurIndex()
{
    m_index=BoardStore::getIndex();
    CCLOG("BoardKeeper::restoreCurIndex m_index=%d",m_index);
}

bool BoardKeeper::hasValidStore()
{
    return BoardStore::hasValidStore();
}

bool BoardKeeper::store(int index)
{
    return BoardStore::store(index,m_data[index]);
}

bool BoardKeeper::restore(int index)
{
    return BoardStore::restore(index,m_data[index]);
}

bool BoardKeeper::storeAll()
{
    storeCurIndex();
    for(int i=0;i<MAX_STORE_BOARD_NUM;i++)
    {
        if(m_data[i].isValid())
            BoardStore::store(i,m_data[i]);
    }
    return true;
}

bool BoardKeeper::restoreAll()
{
    restoreCurIndex();
    CC_RETURN_VAL_IF_FAIL(m_index>=0,false);
    for(int i=0;i<MAX_STORE_BOARD_NUM;i++)
    {
        BoardStore::restore(i,m_data[i]);
    }
    return true;
}


void BoardKeeper::invalidAll()
{
    BoardStore::invalidAll();

}
