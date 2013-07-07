#ifndef __User_H__
#define __User_H__

#include "cocos2d.h"
USING_NS_CC;

class User
{
public:
    static int addLuckyCard(int num);
    static int deleteLuckyCard(int num);
    static int getLuckyCardNum();
    static int addWithdrawCard(int num);
    static int deleteWithdrawCard(int num);
    static int getWithdrawCardNum();

public:
    static void lazyInit();
    static int m_lucky;
    static int m_withdraw;
};

#endif /* __User_H__ */
