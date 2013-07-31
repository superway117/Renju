#ifndef _SettingLayer_H_
#define _SettingLayer_H_
#include "BaseLayer.h"
#include "cocos2d.h"

USING_NS_CC;

class SettingLayer : public BaseMenuLayer
{
public:
    static void runInSceen();

    CREATE_FUNC(SettingLayer);


protected:
    CCArray* prepareMenuList();

private:
    void resetHigh(CCObject *pSender);
    void toggleAudio(CCObject *pSender);
    void goBack(CCObject *pSender);
    void toggleBeadNum(CCObject *pSender);

};

#endif
