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



static void AdMobHelper_init_Android()
{
    AdMobHelperEnv env;
    mid = env->GetStaticMethodID(mClass, "nativeInit", "()V");

    if (mid != NULL) env->CallStaticVoidMethod(mClass, mid);
}

static void AdMobHelper_delete_Android()
{
    AdMobHelperEnv env;
    CCLOG("CCAdView AdMobHelper_delete_Android");
	mid = env->GetStaticMethodID(mClass, "nativeDelete", "()V");
	if (mid != NULL) env->CallStaticVoidMethod(mClass, mid);
}


CCAdView* CCAdView::create(CCAdSize adSize, const char* adUnitId)
{
    CCAdView *adView = new CCAdView();
    if (adView && adView->init())
    {
        adView->autorelease();
        adView->retain();
        return adView;
    }
    CCLOG("CCAdView::create delete");
    CC_SAFE_DELETE(adView);
    return NULL;
}

bool CCAdView::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AdMobHelper_init_Android();
#endif

    return true;
}

CCAdView::CCAdView()
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

#endif