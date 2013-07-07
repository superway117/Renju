#include "User.h"
int User::m_lucky=-1;
int User::m_withdraw=-1;

void User::lazyInit()
{
    if(m_lucky <0)
    {
        m_lucky =  CCUserDefault::sharedUserDefault()->getIntegerForKey("luckycard", 10);
        if(m_lucky==0)m_lucky = 10;
    }
    if(m_withdraw <0)
    {
        m_withdraw =  CCUserDefault::sharedUserDefault()->getIntegerForKey("withdraw", 10);
        if(m_withdraw==0)m_withdraw = 10;
    }

}
int User::addLuckyCard(int num)
{
    lazyInit();
    m_lucky+=num;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("luckycard", m_lucky);
    return m_lucky;
}
int User::deleteLuckyCard(int num)
{
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
    lazyInit();
    m_withdraw+=num;
    CCUserDefault::sharedUserDefault()->setIntegerForKey("withdraw", m_withdraw);
    return m_withdraw;
}
int User::deleteWithdrawCard(int num)
{
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