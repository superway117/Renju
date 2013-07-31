#ifndef _BaseLayer_H_
#define _BaseLayer_H_

#include "cocos2d.h"

USING_NS_CC;


class BaseLayer : public CCLayer
{
public:
    BaseLayer();
    virtual bool init();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void keyBackClicked();
    #endif
    

protected:
    void closeCallback(CCObject * pSender);
    void goBack(CCObject *pSender);

    bool m_closeMenu;
    bool m_backMenu;

};

class BaseMenuLayer : public BaseLayer
{
public:

    BaseMenuLayer();
    //~BaseMenuLayer();

    virtual bool init();

    //void onExit();

protected:
    void initMenuList(CCArray* pArrayOfItems);

    virtual CCArray* prepareMenuList()=0;

    void addMenuItem(CCArray* pArrayOfItems,const char* str,SEL_MenuHandler selector,bool enable=true);
    void addToggleMenuItem(CCArray* pArrayOfItems,
        SEL_MenuHandler selector,int select,...);



private:

    CCMenu* m_pItemMenu;
};

#endif
