
#ifndef ModalDialogDemo_DialogLayer_h
#define ModalDialogDemo_DialogLayer_h

#include "cocos2d.h"

USING_NS_CC;

class DialogLayer: public CCLayerColor
{

    CCMenu *m_pMenu;

    bool m_bTouchedMenu;

public:
    DialogLayer();
    ~DialogLayer();

    virtual bool init(const char* str);
    void initDialog(const char* str);

    static DialogLayer* create(const char* str);

    void onEnter();
    void onExit();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    void okMenuItemCallback(CCObject *pSender);
    void cancelMenuItemCallback(CCObject *pSender);
private:
    void timerCB(float dt);
};

#endif
