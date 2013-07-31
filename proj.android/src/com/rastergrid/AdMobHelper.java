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

package com.rastergrid;

import java.lang.ref.WeakReference;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.google.ads.AdRequest;
import com.google.ads.AdSize;
import com.google.ads.AdView;
import com.google.ads.AdListener;
import com.google.ads.*;

import android.content.Context;
import android.location.Location;
import android.location.LocationManager;
import android.os.Handler;
import android.os.Message;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

import android.util.Log;

import android.os.SystemClock;
import android.view.MotionEvent;

public class AdMobHelper implements AdListener
{
    protected static final int ADMOB_HELPER_INIT            = 1;
    protected static final int ADMOB_HELPER_DELETE          = 2;
    protected static final int ADMOB_HELPER_SET_PARENT      = 3;
    protected static final int ADMOB_HELPER_SET_VISIBLE     = 4;
    protected static final int ADMOB_HELPER_SET_ALIGNMENT   = 5;
    protected static final int ADMOB_HELPER_LOAD_AD         = 6;
    protected static final int ADMOB_HELPER_USE_LOCATION    = 7;
    
    protected static final int ADMOB_HELPER_AUTO_HIT        = 8;

    protected static final int ADMOB_HELPER_INIT1            = 9;

    protected static final int USE_LOCATION_NONE            = 0;
    protected static final int USE_LOCATION_COARSE          = 1;
    protected static final int USE_LOCATION_FINE            = 2;


    protected int mParent;
    protected Cocos2dxActivity mActivity;
    protected AdView mAdView;
    protected RelativeLayout mLayout;
    protected Location mLocation;
    protected static Handler mHandler;

    public AdMobHelper(Cocos2dxActivity activity)
    {
        mActivity = activity;

        //
        mAdView = null;
        // Parent is zero by default
        mParent = 0;

        // Create wrapper relative layout
        mLayout = new RelativeLayout(mActivity);

        // Add layout to content view
        activity.addContentView(mLayout, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));

        // Init JNI handlers
        initJNI(new WeakReference<AdMobHelper>(this));
        mHandler = new Handler()
        {
            public void handleMessage(Message msg)
            {
                switch (msg.what)
                {
                case ADMOB_HELPER_INIT:
                    {

                        AdMobHelper.this.init();
                    }
                    break;
     

                case ADMOB_HELPER_DELETE:
                    {
                        Log.d("AdMobHelper","delete");
                        if (mAdView != null)
                        {
                            mLayout.removeView(mAdView);
                            mAdView = null;
                        }
                    }
                    break;
                }
              
                super.handleMessage(msg);
            }
        };
    }



    public void init()
    {

        // If AdView already exists then remove it (two ads cannot be used at the same time)
        if (mAdView != null)
        {
            Log.d("AdMobHelper","init 1");
            mLayout.removeView(mAdView);
            mAdView = null;
        }

        Log.d("AdMobHelper","init");
        // This array maps integer ad size to the ones accepted by AdMob
        AdSize mapAdSize[] =
        {
            AdSize.SMART_BANNER,
            AdSize.BANNER,
            AdSize.IAB_MRECT,
            AdSize.IAB_BANNER,
            AdSize.IAB_LEADERBOARD,
            AdSize.IAB_WIDE_SKYSCRAPER
        };

        // Create AdView
        mAdView = new AdView(mActivity, AdSize.SMART_BANNER, "a151f7d321e46ec");
        mAdView.setLayoutParams(
                new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                                                ViewGroup.LayoutParams.WRAP_CONTENT));
        setAlignment(0,-1);
        mLayout.addView(mAdView);
        mAdView.setAdListener(this);
        loadAd();
    }

    public void onReceiveAd(Ad ad)
    {

    }
    public void onFailedToReceiveAd(Ad ad, AdRequest.ErrorCode error)
    {

    }
    public void onPresentScreen(Ad ad)
    {
        onTouchAd(new WeakReference<AdMobHelper>(this)); 
    }

    public void onDismissScreen(Ad ad)
    {

    }
    public void onLeaveApplication(Ad ad)
    {

    }

    public void setParent(int parent)
    {
        if (parent == 0 && mParent != 0)
        {
            // Remove from layout
            mLayout.removeView(mAdView);
        }
        else
        if (parent != 0 && mParent == 0)
        {
            // Add to layout
            mLayout.addView(mAdView);
        }
        mParent = parent;
    }

    public void setVisible(boolean visible)
    {
        if (mAdView != null)
        {
            mAdView.setVisibility(visible ? AdView.VISIBLE : AdView.INVISIBLE);
        }
    }

    public void setAlignment(int horizontal, int vertical)
    {
        RelativeLayout.LayoutParams layoutParams =
            new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                                            ViewGroup.LayoutParams.WRAP_CONTENT);

        layoutParams.addRule(horizontal > 0 ? RelativeLayout.ALIGN_PARENT_RIGHT
                           : horizontal < 0 ? RelativeLayout.ALIGN_PARENT_LEFT
                                            : RelativeLayout.CENTER_HORIZONTAL);
        layoutParams.addRule(vertical > 0 ? RelativeLayout.ALIGN_PARENT_TOP
                           : vertical < 0 ? RelativeLayout.ALIGN_PARENT_BOTTOM
                                          : RelativeLayout.CENTER_VERTICAL);

        mAdView.setLayoutParams(layoutParams);
    }

    public void loadAd()
    {
        // Create ad request and set location
        AdRequest request = new AdRequest();
        //request.setLocation(mLocation);
        //request.setLocation();

        //request.addTestDevice(AdRequest.TEST_EMULATOR);
        //request.addTestDevice("DED815D250FD0304C64EEF13B6077D43");    // 我的 T-Mobile G1 测试手机


        // Request ad
        mAdView.loadAd(request);
    }



    private native void initJNI(Object wadk_this);
    private native void onTouchAd(Object wadk_this);


    private static void nativeInit()
    {
        Message msg = new Message();
        msg.what = ADMOB_HELPER_INIT;
        
        mHandler.sendMessage(msg);
    }

    private static void nativeDelete()
    {
        Message msg = new Message();
        msg.what = ADMOB_HELPER_DELETE;
        mHandler.sendMessage(msg);
    }

}
