#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "Bead.h"

USING_NS_CC;

GameLayer::GameLayer()
{

}

GameLayer * GameLayer::create(bool is_new)
{
    GameLayer * pRet = new GameLayer();
    if (pRet && pRet->init(is_new))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

void GameLayer::runInSceen(bool is_new)
{
    CCScene * pScene = CCScene::create();
    CCLayer * pLayer = GameLayer::create(is_new);
    pScene->addChild(pLayer);
    CCDirector::sharedDirector()->replaceScene(pScene);
}


bool GameLayer::init(bool is_new)
{

//init color layer
    CC_RETURN_VAL_IF_FAIL(CCLayerColor::initWithColor( ccc4(255,255,255,255) ),false);

//add bead board
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();

    BeadBoardLayer* board = BeadBoardLayer::create(is_new);
    //board->setAnchorPoint(ccp(0,0));
    CCSize board_size = board->getContentSize();
    int board_y = win_size.height  - board_size.height;
    board->setPosition(ccp(0,board_y));
    addChild(board);
    setTouchEnabled(true);
    //test
    CCLog("GameLayer::init board size:(%f,%f)",board_size.width,board_size.height);
    return true;

}

void GameLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // Choose one of the touches to work with
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    CCPoint location = touch->getLocation();

    CCLog("GameLayer::ccTouchesEnded touch x:%f, y:%f", location.x, location.y);

}
