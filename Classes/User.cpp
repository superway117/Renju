#include "User.h"
#include "BoardKeeper.h"

bool User::m_init=false;

int User::m_lucky=-1;
int User::m_withdraw=-1;
bool User::m_audio=false;
int User::m_beadNum=-1;
int User::m_high=-1;

void User::lazyInit()
{
    if(m_init) return;
    m_init = true;


    m_lucky =  CCUserDefault::sharedUserDefault()->getIntegerForKey("luckycard", 10);
    m_withdraw =  CCUserDefault::sharedUserDefault()->getIntegerForKey("withdraw", 10);

    m_audio=CCUserDefault::sharedUserDefault()->getBoolForKey("audio", false);
    m_beadNum=CCUserDefault::sharedUserDefault()->getIntegerForKey("beadnum", 6);
    if(m_beadNum>7) m_beadNum=7;
    if(m_beadNum<5) m_beadNum=5;
    getHighScoreForce();
}
int User::addLuckyCard(int num)
{
    if(num < 1) return m_lucky;
    lazyInit();
    m_lucky+=num;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("luckycard", m_lucky);
    return m_lucky;
}
int User::deleteLuckyCard(int num)
{
    if(num < 1) return m_lucky;
    lazyInit();
    m_lucky-=num;
    if(m_lucky<0) m_lucky=0;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("luckycard", m_lucky);
    return m_lucky;
}
int User::getLuckyCardNum()
{

    lazyInit();
    return m_lucky;
}
int User::addWithdrawCard(int num)
{
    if(num < 1) return m_withdraw;
    lazyInit();
    m_withdraw+=num;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("withdraw", m_withdraw);
    return m_withdraw;
}
int User::deleteWithdrawCard(int num)
{
    if(num < 1) return m_withdraw;
    lazyInit();
    m_withdraw-=num;
    if(m_withdraw<0) m_withdraw=0;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("withdraw", m_withdraw);
    return m_withdraw;

}
int User::getWithdrawCardNum()
{
    lazyInit();
    return m_withdraw;
}

void User::enableAudioOn(bool on)
{
    lazyInit();
    if(on==m_audio) return;
    m_audio = on;
    CCUserDefault::sharedUserDefault()->setBoolForKey("audio", on);
}

bool User::isAudioOn()
{
    lazyInit();
    return m_audio;
}


void User::setBeadNum(int num)
{
    if(num < 5) return;
    lazyInit();
    if(num==m_beadNum) return;
    m_beadNum = num;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("beadnum", num);
    User::getHighScoreForce();
    BoardStore::invalidAll();
}
int User::getBeadNum()
{
    lazyInit();
    return m_beadNum;
}


void User::setHighScore(int high)
{
    if(high <0 || m_high == high) return;
    lazyInit();
    m_high = high;

    char str[10];
    sprintf(str,"high%d",m_beadNum);
    CCUserDefault::sharedUserDefault()->setIntegerForKey(str, m_high);
}
int User::getHighScore()
{
    lazyInit();
    return m_high;
}

int User::getHighScoreForce()
{
    char str[10];
    sprintf(str,"high%d",m_beadNum);
    m_high = CCUserDefault::sharedUserDefault()->getIntegerForKey(str, 0);
    return m_high;
}