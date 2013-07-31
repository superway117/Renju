#ifndef __GameRulerLayer_H__
#define __GameRulerLayer_H__

#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;


class GameRulerLayer : public BaseLayer//cocos2d::CCLayer
{
public:
    GameRulerLayer();

    static void runInSceen();

    virtual bool init();

    static GameRulerLayer * create();

private:
    //void goBack(CCObject *pSender);
    void addRuler();

};

#endif /* __GameRulerLayer_H__ */
