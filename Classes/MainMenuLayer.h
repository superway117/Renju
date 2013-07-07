#ifndef _MainMenuLayer_H_
#define _MainMenuLayer_H_

#include "cocos2d.h"

USING_NS_CC;

class MainMenuLayer : public CCLayer
{
public:
    static void runInSceen();

    MainMenuLayer();
    ~MainMenuLayer();

    virtual bool init();

    CREATE_FUNC(MainMenuLayer);

    //void menuCallback(CCObject * pSender);
    //virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    //virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);

private:
    void initMenuList();
    void addMenuItem(CCArray* pArrayOfItems,std::string name,SEL_MenuHandler selector);

    void newGame(CCObject *pSender);
    void continueGame(CCObject *pSender);

    void closeCallback(CCObject * pSender);


private:

    CCMenu* m_pItemMenu;
};

#endif
