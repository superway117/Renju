#include "BaseLayer.h"
#include "SmartRes.h"
#include "MainMenuLayer.h"
#include "BoardConfig.h"

#define BACKGROUND_NUM 2
static const char s_pPathBack[] = "Images/back.png";
static const char s_pPathClose[] = "Images/close.png";
BaseLayer::BaseLayer(): m_closeMenu(true),m_backMenu(true)
{

}

bool BaseLayer::init()
{
    CCLog("BaseLayer::init");
    CCLayer::init();
    //init background
    int bg_num = (rand()%BACKGROUND_NUM)+1;
    char bg_path[20];
    sprintf(bg_path,"Images/bg%d.png",bg_num);
    CCSprite* bg = CCSprite::create(bg_path);

    //CCSize bg_size = CCDirector::sharedDirector()->getWinSize();
    bg->setPosition(ccp(SMART_CENTER.x,SMART_CENTER.y));
    addChild(bg);

    // add close menu
    if(m_closeMenu)
    {
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(s_pPathClose, s_pPathClose, this, menu_selector(BaseLayer::closeCallback) );
        CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);

        pMenu->setPosition( CCPointZero );
        pCloseItem->setPosition(ccp( SMART_RIGHT - 50, SMART_TOP - 30));
        addChild(pMenu, 1);
    }

    //init back  menu
    if(m_backMenu)
    {
        CCMenuItemImage *pBackItem = CCMenuItemImage::create(s_pPathBack, s_pPathBack, this, menu_selector(BaseLayer::goBack) );
        CCMenu* pBackMenu =CCMenu::create(pBackItem, NULL);

        pBackMenu->setPosition( CCPointZero );
        pBackItem->setPosition(ccp( SMART_RIGHT - 50, SMART_BOTTOM + 160));
        addChild(pBackMenu, 1);
    }

    setTouchEnabled(true);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    setKeypadEnabled(true);
    #endif

    return true;

}

void BaseLayer::goBack(CCObject *pSender)
{
    MainMenuLayer::runInSceen();
}

void BaseLayer::closeCallback(CCObject * pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void BaseLayer::keyBackClicked()
{
    goBack(NULL);
}
#endif

void BaseMenuLayer::addMenuItem(CCArray* pArrayOfItems,const char* str,SEL_MenuHandler selector,bool enable)
{
    CCLabelTTF* label = CCLabelTTF::create(str, "Arial", LARGE_FONT_SIZE);
    
    CCSprite* s = CCSprite::create("Images/button1.png");
    label->setPosition(ccp(s->getContentSize().width/2,s->getContentSize().height/2));
    s->addChild(label);

    CCMenuItemSprite* pMenuItem = CCMenuItemSprite::create(s, s,s,this, selector);
    pArrayOfItems->addObject(pMenuItem);
}
void BaseMenuLayer::addToggleMenuItem(CCArray* pArrayOfItems,SEL_MenuHandler selector,int select,...)
{
    va_list args;
    va_start(args, select);
    CCMenuItemToggle *item = CCMenuItemToggle::createWithTarget(this, selector,  NULL);
    do{
        const char* str = va_arg(args, const char*);
        if(!str) break;
        //CCLabelTTF *label = CCLabelTTF::create(str, "Arial", 24);
        //CCMenuItemLabel *menuitem = CCMenuItemLabel::create(label);
        CCLabelTTF* label = CCLabelTTF::create(str, "Arial", LARGE_FONT_SIZE);
        CCSprite* s = CCSprite::create("Images/button1.png");
        label->setPosition(ccp(s->getContentSize().width/2,s->getContentSize().height/2));
        s->addChild(label);

        CCMenuItemSprite* menuitem = CCMenuItemSprite::create(s, s,s,this, selector);
        item->addSubItem(menuitem);

    }while(true);


    item->setSelectedIndex(select);
    pArrayOfItems->addObject(item);
    va_end(args);
}

void BaseMenuLayer::initMenuList(CCArray* pArrayOfItems)
{
    CC_RETURN_IF_FAIL(!m_pItemMenu);
    m_pItemMenu = CCMenu::createWithArray(pArrayOfItems);
    m_pItemMenu->alignItemsVertically();

    CCObject* pObject = NULL;
    int i=0;
    CCNode* child = NULL;
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCARRAY_FOREACH(pArrayOfItems, pObject)
    {
        if(pObject == NULL)
            break;

        child = (CCNode*)pObject;

        CCPoint dstPoint = child->getPosition();
        int offset = (int) (s.width/2 + 50);
        if( i % 2 == 0)
            offset = -offset;

        child->setPosition( ccp( dstPoint.x + offset, dstPoint.y) );
        child->runAction(
                          CCEaseElasticOut::create(CCMoveBy::create(2, ccp(dstPoint.x - offset,0)), 0.35f)
                        );
        i++;
    }

    addChild(m_pItemMenu);
    m_pItemMenu->setPosition(ccp(s.width/2, s.height/2));
    CC_SAFE_RELEASE(pArrayOfItems);

}
BaseMenuLayer::BaseMenuLayer(): m_pItemMenu(NULL)
{

}
bool BaseMenuLayer::init()
{
    CCLog("BaseMenuLayer::init");
    BaseLayer::init();

    // add menu items
    CCArray* pArrayOfItems=prepareMenuList();
    initMenuList(pArrayOfItems);

    return true;

}
#if 0

void BaseMenuLayer::onExit()
{
    if(m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* child;
        CCARRAY_FOREACH(m_pChildren, child)
        {
            CCNode* pChild = (CCNode*) child;
            pChild->stopAllActions();
      
        }
    }
    CCLayer::onExit();
}
#endif