#ifndef __GameLayer_H__
#define __GameLayer_H__

#include "cocos2d.h"
#include "Bead.h"
#include "BeadBoardLayer.h"
USING_NS_CC;


class GameLayer : public cocos2d::CCLayerColor
{
public:
    GameLayer();

    static void runInSceen(bool is_new);

    virtual bool init(bool is_new);

    static GameLayer * create(bool is_new);
    //CREATE_FUNC(GameLayer);

    void registerWithTouchDispatcher();

    void ccTouchesEnded(CCSet* touches, CCEvent* event);


private:
    void goBack(CCObject *pSender);

};

#endif /* __GameLayer_H__ */
