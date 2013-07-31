LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := app/main.cpp \
                ../../Classes/MainMenuLayer.cpp \
                ../../Classes/AppDelegate.cpp \
                ../../Classes/SmartRes.cpp \
                ../../Classes/BaseLayer.cpp \
                ../../Classes/Bead.cpp \
                ../../Classes/BeadBoardLayer.cpp \
                ../../Classes/Arithmetic.cpp \
                ../../Classes/BoardKeeper.cpp \
                ../../Classes/User.cpp \
                ../../Classes/DialogLayer.cpp \
                ../../Classes/SettingLayer.cpp \
                ../../Classes/GameRulerLayer.cpp \
                ../../Classes/StringRes.cpp \
                ../../Classes/CCAdView.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static

#LOCAL_WHOLE_STATIC_LIBRARIES += jsonc_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
#$(call import-module,external/json-c)