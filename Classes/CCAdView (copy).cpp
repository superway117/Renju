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

#include "CCAdView.h"

#include "cocos2d.h"
#include "User.h"

using namespace cocos2d;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "BeadBoardLayer.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_rastergrid_AdMobHelper_initJNI(JNIEnv*, jobject, jobject);
JNIEXPORT void JNICALL Java_com_rastergrid_AdMobHelper_onTouchAd(JNIEnv* env, jobject , jobject );

#ifdef __cplusplus
}
#endif

static jmethodID mid;
static jclass mClass;
static jobject mObject;

int s_adTouchNum = 0;
bool s_adLastTouchChecked = true;

JNIEXPORT void JNICALL Java_com_rastergrid_AdMobHelper_initJNI(JNIEnv* env, jobject thiz, jobject weak_this)
{
	jclass clazz = env->GetObjectClass(thiz);
	mClass = (jclass)env->NewGlobalRef(clazz);
	mObject = env->NewGlobalRef(weak_this);
}


JNIEXPORT void JNICALL Java_com_rastergrid_AdMobHelper_onTouchAd(JNIEnv* env, jobject thiz, jobject weak_this)
{

    s_adTouchNum++;
    s_adLastTouchChecked = false;
}



class AdMobHelperEnv
{
public:
    AdMobHelperEnv()
    {
        m_isAttached = false;
	    int status = JniHelper::getJavaVM()->GetEnv((void**)&m_env, JNI_VERSION_1_6);
	    if (status < 0)
	    {
		    CCLOG("AdMobHelperEnv: JavaVM::GetEnv failed, assuming native thread");
		    status = JniHelper::getJavaVM()->AttachCurrentThread(&m_env, NULL);
		    if (status < 0)
		    {
			    CCLOG("AdMobHelperEnv: failed to attach current thread");
			    return;
		    }
		    m_isAttached = true;
	    }
        return;
    }

    ~AdMobHelperEnv()
    {
        if (m_isAttached) JniHelper::getJavaVM()->DetachCurrentThread();
    }

    JNIEnv* operator-> () { return m_env; }

protected:
    bool m_isAttached;
    JNIEnv* m_env;
};

static void AdMobHelper_init_Android(int adSize, const char* adUnitId)
{
    AdMobHelperEnv env;
	mid = env->GetStaticMethodID(mClass, "nativeInit", "(ILjava/lang/String;)V");
    jstring jAdUnitId = env->NewStringUTF(adUnitId);
	if (mid != NULL) env->CallStaticVoidMethod(mClass, mid, adSize, jAdUnitId);
}

static void AdMobHelper_init1_Android()
{
    AdMobHelperEnv env;
    mid = env->GetStaticMethodID(mClass, "nativeInit1", "(ILjava/lang/String;)V");

    if (mid != NULL) env->CallStaticVoidMethod(mClass, mid);
}

static void AdMobHelper_delete_Android()
{
    AdMobHelperEnv env;
    CCLOG("CCAdView AdMobHelper_delete_Android");
	mid = env->GetStaticMethodID(mClass, "nativeDelete", "()V");
	if (mid != NULL) env->CallStaticVoidMethod(mClass, mid);
}

static void AdMobHelper_setParent_Android(int parent)
{
    AdMobHelperEnv env;
	mid = env->GetStaticMethodID(mClass, "nativeSetParent", "(I)V");
	if (mid != NULL) env->CallStaticVoidMethod(mClass, mid, parent);
}

static void AdMobHelper_setVisible_Android(bool visible)
{
    AdMobHelperEnv env;
	mid = env->GetStaticMethodID(mClass, "nativeSetVisible", "(Z)V");
	if (mid != NULL) env->CallStaticVoidMethod(mClass, mid, visible ? 1 : 0);
}

static void AdMobHelper_setAlignment_Android(int horizontal, int vertical)
{
    AdMobHelperEnv env;
	mid = env->GetStaticMethodID(mClass, "nativeSetAlignment", "(II)V");
	if (mid != NULL) env->CallStaticVoidMethod(mClass, mid, horizontal, vertical);
}

static void AdMobHelper_loadAd_Android()
{
    AdMobHelperEnv env;
	mid = env->GetStaticMethodID(mClass, "nativeLoadAd", "()V");
	if (mid != NULL) env->CallStaticVoidMethod(mClass, mid);
}

static void AdMobHelper_useLocation_Android(int location)
{
    AdMobHelperEnv env;
	mid = env->GetStaticMethodID(mClass, "nativeUseLocation", "(I)V");
	if (mid != NULL) env->CallStaticVoidMethod(mClass, mid, location);
}

#if 0
static void AdMobHelper_autoHit_Android()
{
    AdMobHelperEnv env;
    mid = env->GetStaticMethodID(mClass, "nativeAutoHit", "()V");
    if (mid != NULL) env->CallStaticVoidMethod(mClass, mid);
}
#endif

#endif /* CC_PLATFORM_ANDROID */

#if (CC_PLATFORM_WIN32)

#include "draw_nodes/CCDrawingPrimitives.h"

static CCSize size;

static void AdMobHelper_init_Win32(CCAdSize adSize)
{
    switch (adSize)
    {
    case kCCAdSizeSmartBanner:
    case kCCAdSizeBanner:
        size = CCSize(320, 50);
        break;
    case kCCAdSizeMediumRectangle:
        size = CCSize(300, 250);
        break;
    case kCCAdSizeFullBanner:
        size = CCSize(468, 60);
        break;
    case kCCAdSizeLeaderboard:
        size = CCSize(728, 90);
        break;
    case kCCAdSizeWideSkyscraper:
        size = CCSize(160, 600);
        break;
    }
}

static void AdMobHelper_draw_Win32(CCHorizontalAlignment horizontal, CCVerticalAlignment vertical)
{
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();

    CCPoint origin, destination;

    switch (horizontal)
    {
    case kCCHorizontalAlignmentLeft:
        origin.x      = 0;
        destination.x = size.width;
        break;
    case kCCHorizontalAlignmentCenter:
        origin.x      = (screenSize.width - size.width) * 0.5f;
        destination.x = (screenSize.width + size.width) * 0.5f;
        break;
    case kCCHorizontalAlignmentRight:
        origin.x      = screenSize.width - size.width;
        destination.x = screenSize.width;
        break;
    }

    switch (vertical)
    {
    case kCCVerticalAlignmentBottom:
        origin.y      = 0;
        destination.y = size.height;
        break;
    case kCCVerticalAlignmentCenter:
        origin.y      = (screenSize.height - size.height) * 0.5f;
        destination.y = (screenSize.height + size.height) * 0.5f;
        break;
    case kCCVerticalAlignmentTop:
        origin.y      = screenSize.height - size.height;
        destination.y = screenSize.height;
        break;
    }

    ccColor4F white = { 1.f, 1.f, 1.f, 1.f };
    ccDrawSolidRect(origin, destination, white);
        
    origin.x      += 1.f;    
    origin.y      += 1.f;
    destination.x -= 1.f;    
    destination.y -= 1.f;

    ccColor4F black = { 0.f, 0.f, 0.f, 1.f };
    ccDrawSolidRect(origin, destination, black);
}

#endif /* CC_PLATFORM_WIN32 */

CCAdView* CCAdView::create(CCAdSize adSize, const char* adUnitId)
{
    CCAdView *adView = new CCAdView();
    if (adView && adView->init(adSize, adUnitId))
    {
        adView->autorelease();
        adView->retain();
        return adView;
    }
    CCLOG("CCAdView::create delete");
    CC_SAFE_DELETE(adView);
    return NULL;
}


bool CCAdView::init(CCAdSize adSize, const char* adUnitId)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdMobHelper_init_Android((int)adSize, adUnitId);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    AdMobHelper_init_Win32(adSize);
#endif

    return true;
}
bool CCAdView::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AdMobHelper_init1_Android();
#endif

    return true;
}

CCAdView::CCAdView()
  : m_horizontalAlignment(kCCHorizontalAlignmentLeft),
    m_verticalAlignment(kCCVerticalAlignmentTop),
    m_usedLocation(kCCLocationNone)
{
    CCLOG("CCAdView constructor ");
}

CCAdView::~CCAdView()
{
    CCLOG("CCAdView delete ");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdMobHelper_delete_Android();
#endif
}

void CCAdView::draw()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (isVisible())
    {
        AdMobHelper_draw_Win32(m_horizontalAlignment, m_verticalAlignment);
    }
#endif
    //CCNode::draw();
}

void CCAdView::setParent(CCNode* parent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdMobHelper_setParent_Android((int)parent);
#endif
    //CCNode::setParent(parent);
}

void CCAdView::setVisible(bool visible)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdMobHelper_setVisible_Android(visible);
#endif
    //CCNode::setVisible(visible);
}

void CCAdView::setAlignment(CCHorizontalAlignment horizontal, CCVerticalAlignment vertical)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdMobHelper_setAlignment_Android((int)horizontal, (int)vertical);
#endif

    m_horizontalAlignment = horizontal;
    m_verticalAlignment   = vertical;
}

void CCAdView::loadAd()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdMobHelper_loadAd_Android();
#endif
}

void CCAdView::useLocation(CCLocation location)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	AdMobHelper_useLocation_Android((int)location);
#endif

    m_usedLocation = location;
}

#if 0
void CCAdView::autoHit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AdMobHelper_autoHit_Android();
#endif

}
#endif
