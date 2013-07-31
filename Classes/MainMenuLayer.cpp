#include "MainMenuLayer.h"
//#include "CCArray.h"
#include "BeadBoardLayer.h"
#include "BoardKeeper.h"
#include "SettingLayer.h"
#include "GameRulerLayer.h"
#include "StringRes.h"

static const char s_pPathClose[] = "Images/close.png";


void MainMenuLayer::runInSceen()
{
    CCScene * pScene = CCScene::create();
    CCLayer * pLayer = MainMenuLayer::create();
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
}


MainMenuLayer::MainMenuLayer()
{
    m_closeMenu=false;
    m_backMenu=false;
}

CCArray* MainMenuLayer::prepareMenuList()
{
    CCArray* pArrayOfItems = CCArray::create();
    const char* newgame_str = StringRes::sharedStringRes()->getString(ID_STRING_NEW_GAME);
    addMenuItem(pArrayOfItems,newgame_str,menu_selector(MainMenuLayer::newGame));
    bool hasValidStore = BoardStore::hasValidStore();
    const char* continues_str = StringRes::sharedStringRes()->getString(ID_STRING_CONTINUE);
    addMenuItem(pArrayOfItems,continues_str,menu_selector(MainMenuLayer::continueGame),hasValidStore);
    const char* setting_str = StringRes::sharedStringRes()->getString(ID_STRING_SETTING);
    addMenuItem(pArrayOfItems,setting_str,menu_selector(MainMenuLayer::setting));
    
    const char* game_ruler = StringRes::sharedStringRes()->getString(ID_STRING_GAME_RULER);
    addMenuItem(pArrayOfItems,game_ruler,menu_selector(MainMenuLayer::gameRuler));
    const char* quit_str = StringRes::sharedStringRes()->getString(ID_STRING_QUIT);
    addMenuItem(pArrayOfItems,quit_str,menu_selector(MainMenuLayer::close));
    return pArrayOfItems;

}
MainMenuLayer::~MainMenuLayer()
{

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void MainMenuLayer::keyBackClicked()
{
    //close(NULL);
    closeCallback(NULL);
}
#endif


void MainMenuLayer::newGame(CCObject *pSender)
{
    BeadBoardLayer::runInSceen(true);
}

void MainMenuLayer::continueGame(CCObject *pSender)
{
    BeadBoardLayer::runInSceen(false);
}
void MainMenuLayer::setting(CCObject *pSender)
{
    SettingLayer::runInSceen();
}
void MainMenuLayer::gameRuler(CCObject *pSender)
{
    GameRulerLayer::runInSceen();
}

void MainMenuLayer::close(CCObject * pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
