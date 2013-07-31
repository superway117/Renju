
#ifndef __SMART_RES_H_
#define __SMART_RES_H_


#include "cocos2d.h"

USING_NS_CC;

#define SMART_SCREEN     SmartRes::sharedRes()->getScreenSize()
#define SMART_CENTER     SmartRes::sharedRes()->getCenter()
#define SMART_TOP        SmartRes::sharedRes()->getTop()
#define SMART_RIGHT      SmartRes::sharedRes()->getRight()
#define SMART_BOTTOM     SmartRes::sharedRes()->getBottom()
#define SMART_LEFT       SmartRes::sharedRes()->getLeft()
#define SMART_SCALE      SmartRes::sharedRes()->getScale()
#define SMART_XY(v)      SmartRes::sharedRes()->xy(v)

class SmartRes
{
public:
    static SmartRes* sharedRes(void);

    void setVirtualScreenWidth(float width);

    const CCSize getScreenSize(void) {
        return m_screenSize;
    }

    float getScale(void) {
        return m_scale;
    }

    const CCPoint getCenter(void) {
        return m_center;
    }

    float getTop(void) {
        return m_top;
    }

    float getRight(void) {
        return m_right;
    }

    float getBottom(void) {
        return m_bottom;
    }

    float getLeft(void) {
        return m_left;
    }

    float xy(float v) {
        return m_scale * v;
    }

private:
    SmartRes(void)
    : m_scale(1.0f)
    , m_top(0)
    , m_right(0)
    , m_bottom(0)
    , m_left(0)
    {
    }
    bool init(void);

    static SmartRes* s_sharedRes;

    CCSize m_screenSize;
    CCPoint m_center;
    float m_scale;
    float m_top;
    float m_right;
    float m_bottom;
    float m_left;
};
#endif 