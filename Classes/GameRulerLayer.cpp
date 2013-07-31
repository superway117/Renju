#include "GameRulerLayer.h"
#include "SimpleAudioEngine.h"
#include "Bead.h"
#include "MainMenuLayer.h"
#include "SmartRes.h"
#include "BoardConfig.h"
#include "StringRes.h"

GameRulerLayer::GameRulerLayer()
{

}

GameRulerLayer * GameRulerLayer::create()
{
    GameRulerLayer * pRet = new GameRulerLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void GameRulerLayer::runInSceen()
{
    CCScene * pScene = CCScene::create();
    CCLayer * pLayer = GameRulerLayer::create();
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
}


bool GameRulerLayer::init()
{

//base
    BaseLayer::init();


    addRuler();
    return true;

}

void GameRulerLayer::addRuler()
{
    const char* str = StringRes::sharedStringRes()->getString(ID_STRING_GAME_DESCRIPTION);
    CCLabelTTF *label1 = CCLabelTTF::create(str, "Arial", NORMAL_FONT_SIZE);
    label1->setHorizontalAlignment(kCCTextAlignmentLeft);
    label1->setPosition(ccp(SMART_CENTER.x, SMART_CENTER.y));
    label1->setColor(ccBLACK);
    addChild(label1);
}
