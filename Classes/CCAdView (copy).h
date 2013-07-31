/*
 * Copyright (c) 2013 Daniel Rakos
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __CCADVIEW_H__
#define __CCADVIEW_H__

#include "cocos2d.h"

NS_CC_BEGIN

typedef enum _CCAdSize
{
    kCCAdSizeSmartBanner,
    kCCAdSizeBanner,
    kCCAdSizeMediumRectangle,
    kCCAdSizeFullBanner,
    kCCAdSizeLeaderboard,
    kCCAdSizeWideSkyscraper
} CCAdSize;

typedef enum _CCHorizontalAlignment
{
    kCCHorizontalAlignmentLeft = -1,
    kCCHorizontalAlignmentCenter = 0,
    kCCHorizontalAlignmentRight = 1
} CCHorizontalAlignment;

typedef enum _CCVerticalAlignment
{
    kCCVerticalAlignmentBottom = -1,
    kCCVerticalAlignmentCenter = 0,
    kCCVerticalAlignmentTop = 1
} CCVerticalAlignment;

typedef enum _CCLocation
{
    kCCLocationNone,
    kCCLocationCoarse,
    kCCLocationFine
} CCLocation;

typedef void (CCObject::*AdViewOnTap_HANDLE)();

class CCAdView : public CCObject//, public CCTargetedTouchDelegate
{
public:
    static CCAdView* create(CCAdSize adSize, const char* adUnitId);
    virtual bool init(CCAdSize adSize, const char* adUnitId);
    virtual bool init();

    CCAdView(void);
    virtual ~CCAdView(void);

	virtual void draw(void);

    virtual void setParent(CCNode* parent);
	virtual void setVisible(bool visible);

	virtual void setAlignment(CCHorizontalAlignment horizontal, CCVerticalAlignment vertical);
    virtual CCHorizontalAlignment getHorizontalAlignment() const { return m_horizontalAlignment; }
    virtual CCVerticalAlignment getVerticalAlignment() const { return m_verticalAlignment; }

    virtual void loadAd();
    virtual void useLocation(CCLocation location);
    virtual CCLocation getUsedLocation() const { return m_usedLocation; }

    //void autoHit();

protected:
    CCHorizontalAlignment   m_horizontalAlignment;
    CCVerticalAlignment     m_verticalAlignment;
    CCLocation              m_usedLocation;

};

NS_CC_END

#endif /* __CCADVIEW_H__ */
