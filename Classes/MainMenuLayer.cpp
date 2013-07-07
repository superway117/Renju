#include "MainMenuLayer.h"
#include "CCArray.h"
#include "VisibleRect.h"
#include "GameLayer.h"

//#define LINE_SPACE          40
static const char s_pPathClose[] = "Images/close.png";


void MainMenuLayer::runInSceen()
{
    CCScene * pScene = CCScene::create();
    CCLayer * pLayer = MainMenuLayer::create();
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
}


void MainMenuLayer::addMenuItem(CCArray* pArrayOfItems,std::string name,SEL_MenuHandler selector)
{
    CCLabelTTF* label = CCLabelTTF::create(name.c_str(), "Arial", 24);
    CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, selector);
    pArrayOfItems->addObject(pMenuItem);
}

void MainMenuLayer::initMenuList()
{
    CC_RETURN_IF_FAIL(!m_pItemMenu);

    CCArray* pArrayOfItems = CCArray::create();
    addMenuItem(pArrayOfItems,"New Game",menu_selector(MainMenuLayer::newGame));
    addMenuItem(pArrayOfItems,"Continue",menu_selector(MainMenuLayer::continueGame));
    addMenuItem(pArrayOfItems,"Setting",menu_selector(MainMenuLayer::newGame));
    addMenuItem(pArrayOfItems,"High Score",menu_selector(MainMenuLayer::newGame));
    addMenuItem(pArrayOfItems,"Game Rules",menu_selector(MainMenuLayer::newGame));
    addMenuItem(pArrayOfItems,"Quit",menu_selector(MainMenuLayer::closeCallback));
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
MainMenuLayer::MainMenuLayer(): m_pItemMenu(NULL)
{

}
bool MainMenuLayer::init()
{
    CCLog("MainMenuLayer::init");
    // add close menu
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(s_pPathClose, s_pPathClose, this, menu_selector(MainMenuLayer::closeCallback) );
    CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);

    pMenu->setPosition( CCPointZero );
    pCloseItem->setPosition(ccp( VisibleRect::right().x - 30, VisibleRect::top().y - 30));

    // add menu items
    initMenuList();
    setTouchEnabled(true);
    addChild(pMenu, 1);
    return true;

}

MainMenuLayer::~MainMenuLayer()
{

}

void MainMenuLayer::newGame(CCObject *pSender)
{
    GameLayer::runInSceen(true);
}

void MainMenuLayer::continueGame(CCObject *pSender)
{
    GameLayer::runInSceen(false);
}


void MainMenuLayer::closeCallback(CCObject * pSender)
{
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
