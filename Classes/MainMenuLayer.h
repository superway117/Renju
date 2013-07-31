#ifndef _MainMenuLayer_H_
#define _MainMenuLayer_H_
#include "BaseLayer.h"
#include "cocos2d.h"

USING_NS_CC;

class MainMenuLayer : public BaseMenuLayer
{
public:
    static void runInSceen();

    MainMenuLayer();
    ~MainMenuLayer();

    CREATE_FUNC(MainMenuLayer);
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void keyBackClicked();
    #endif

protected:
    CCArray* prepareMenuList();

private:
    void newGame(CCObject *pSender);
    void continueGame(CCObject *pSender);
    void setting(CCObject *pSender);
    void gameRuler(CCObject *pSender);
    void close(CCObject * pSender);

};

#endif
