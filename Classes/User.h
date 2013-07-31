#ifndef __User_H__
#define __User_H__

#include "cocos2d.h"
USING_NS_CC;

class User
{
public:

//cards
    static int addLuckyCard(int num);
    static int deleteLuckyCard(int num);
    static int getLuckyCardNum();
    static int addWithdrawCard(int num);
    static int deleteWithdrawCard(int num);
    static int getWithdrawCardNum();

//audio
    static void enableAudioOn(bool on);
    static bool isAudioOn();

//bead num
    static void setBeadNum(int num);
    static int getBeadNum();

//high
    static void setHighScore(int high);
    static int getHighScore();
    static int getHighScoreForce();

private:
    static void lazyInit();
    static bool m_init;

    static int m_lucky;
    static int m_withdraw;

    static bool m_audio;

    static int m_beadNum;
    static int m_high;

};

#endif /* __User_H__ */
