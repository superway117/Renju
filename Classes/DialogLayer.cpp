
#include "DialogLayer.h"
#include "BoardConfig.h"

DialogLayer::DialogLayer():m_bTouchedMenu(false)
{
}

DialogLayer::~DialogLayer()
{
}

DialogLayer* DialogLayer::create(const char* str)
{
    DialogLayer *pRet = new DialogLayer();
    if (pRet && pRet->init(str))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool DialogLayer::init(const char* str)
{
    bool bRet = false;

    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100)));
        //CCTimer::timerWithTarget(this,schedule_selector( DialogLayer::timerCB), 2);
        //setContentSize(CCSize(320,100));

        //bg->setContentSize(CCSize(320,100));
        //CCSize bg_size = CCDirector::sharedDirector()->getWinSize();
        //bg->setPosition(ccp(160,50));

        schedule(schedule_selector( DialogLayer::timerCB), 2, 0, 0);
        this->initDialog(str);
        //setContentSize(CCSize(320,100));
        bRet = true;
    } while (0);

    return bRet;
}
void DialogLayer::timerCB(float dt)
{
    this->removeFromParentAndCleanup(true);
}

void DialogLayer::initDialog(const char* str)
{

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* bg = CCSprite::create("Images/dialog_bg.png");
    bg->setPosition(ccp(size.width / 2, size.height / 2));


    CCLabelTTF *label = CCLabelTTF::create(str, "Marker Felt", NORMAL_FONT_SIZE);
    label->setPosition(ccp(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
    bg->addChild(label);

    //setPosition(ccp(0, 44 ));

    addChild(bg);
#if 0
    CCMenuItemFont *okMenuItem = CCMenuItemFont::create("OK", this, menu_selector(DialogLayer::okMenuItemCallback));
    okMenuItem->setPosition(ccp(winSize.width / 2 - 50, winSize.height / 2 - 50));

    CCMenuItemFont *cancelMenuItem = CCMenuItemFont::create("Cancel", this, menu_selector(DialogLayer::cancelMenuItemCallback));
    cancelMenuItem->setPosition(ccp(winSize.width / 2 + 50, winSize.height / 2 - 50));

    m_pMenu = CCMenu::create(okMenuItem, cancelMenuItem, NULL);
    m_pMenu->setPosition(CCPointZero);
    this->addChild(m_pMenu);
#endif
}

void DialogLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);

}

void DialogLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool DialogLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //m_bTouchedMenu = m_pMenu->ccTouchBegan(pTouch, pEvent);

    return true;
}

void DialogLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

    if (m_bTouchedMenu) {
        m_pMenu->ccTouchMoved(pTouch, pEvent);
    }
}

void DialogLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_bTouchedMenu) {
        m_pMenu->ccTouchEnded(pTouch, pEvent);
        m_bTouchedMenu = false;
    }
}

void DialogLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_bTouchedMenu) {
        m_pMenu->ccTouchEnded(pTouch, pEvent);
        m_bTouchedMenu = false;
    }
}

void DialogLayer::okMenuItemCallback(cocos2d::CCObject *pSender)
{

}

void DialogLayer::cancelMenuItemCallback(cocos2d::CCObject *pSender)
{
    this->removeFromParentAndCleanup(true);
}
