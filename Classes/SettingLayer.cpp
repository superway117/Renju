#include "SettingLayer.h"
#include "BoardKeeper.h"
#include "MainMenuLayer.h"
#include "User.h"
#include "DialogLayer.h"
#include "StringRes.h"


static const char s_pPathClose[] = "Images/close.png";
static const char s_pPathBack[] = "Images/back.png";


void SettingLayer::runInSceen()
{
    CCScene * pScene = CCScene::create();
    CCLayer * pLayer = SettingLayer::create();
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
}


CCArray* SettingLayer::prepareMenuList()
{
    CCArray* pArrayOfItems = CCArray::create();
    int select = static_cast<int>(User::isAudioOn());
    const char* audioon_str = StringRes::sharedStringRes()->getString(ID_STRING_AUDIO_ON);
    const char* audiooff_str = StringRes::sharedStringRes()->getString(ID_STRING_AUDIO_OFF);
    addToggleMenuItem(pArrayOfItems,menu_selector(SettingLayer::toggleAudio),select,audioon_str,audiooff_str,NULL);

    select = User::getBeadNum()-5;
    const char* bead5_str = StringRes::sharedStringRes()->getString(ID_STRING_5_BEAD_MODE);
    const char* bead6_str = StringRes::sharedStringRes()->getString(ID_STRING_6_BEAD_MODE);
    const char* bead7_str = StringRes::sharedStringRes()->getString(ID_STRING_7_BEAD_MODE);
 
    addToggleMenuItem(pArrayOfItems,menu_selector(SettingLayer::toggleBeadNum),select,bead5_str,bead6_str,bead7_str,NULL);
    const char* reset_high_str = StringRes::sharedStringRes()->getString(ID_STRING_RESET_HIGH_SCORE);
    addMenuItem(pArrayOfItems,reset_high_str,menu_selector(SettingLayer::resetHigh));
    //addMenuItem(pArrayOfItems,"Back",menu_selector(SettingLayer::goBack));
    return pArrayOfItems;

}


void SettingLayer::goBack(CCObject *pSender)
{
    MainMenuLayer::runInSceen();
}
void SettingLayer::resetHigh(CCObject *pSender)
{
    User::setHighScore(0);
    const char* str = StringRes::sharedStringRes()->getString(ID_STRING_OPERATOR_SUCC_HINT);
    DialogLayer *d = DialogLayer::create(str);
    addChild(d);
}

void SettingLayer::toggleAudio(CCObject *pSender)
{
    CCMenuItemToggle* item = (CCMenuItemToggle*)pSender;
    int index = item->getSelectedIndex();
    CCLOG("SettingLayer::toggleAudio index=%d",index);
    User::enableAudioOn(static_cast<bool>(index));
}


void SettingLayer::toggleBeadNum(CCObject *pSender)
{
    CCMenuItemToggle* item = (CCMenuItemToggle*)pSender;
    int index = item->getSelectedIndex();
    CCLOG("SettingLayer::toggleBeadNum index=%d",index);
    User::setBeadNum(index+5);
}

#if 0
void SettingLayer::close(CCObject * pSender)
{
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
#endif
