LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
		   ../../Classes/Spider.cpp \
  		   ../../Classes/Bug.cpp \
  		   ../../Classes/WeakestBug.cpp \
  		   ../../Classes/Mosquito.cpp \
  		   ../../Classes/Crane.cpp \
  		   ../../Classes/FeebleBug.cpp \
    		   ../../Classes/Moth.cpp \
  		   ../../Classes/Ladybird.cpp \
  		   ../../Classes/PowerfulBug.cpp \
  		   ../../Classes/Dragonfly.cpp \
  		   ../../Classes/Bumblebee.cpp \
  		   ../../Classes/StrongestBug.cpp \
 		   ../../Classes/Gadfly.cpp \
  		   ../../Classes/Robberfly.cpp \
  		   ../../Classes/BonusBug.cpp \
  		   ../../Classes/Web.cpp \
  		   ../../Classes/WebString.cpp \
  		   ../../Classes/Boundries.cpp \
  		   ../../Classes/GameMenuScene.cpp \
  		   ../../Classes/GameSureScene.cpp \
  		   ../../Classes/GamePlayScene.cpp \
  		   ../../Classes/GamePlayLayer.cpp \
  		   ../../Classes/GameInfoLayer.cpp \
  		   ../../Classes/GameLoadScene.cpp \
  		   ../../Classes/GameOverScene.cpp \
  		   ../../Classes/LanguageManager.cpp \
  		   ../../Classes/Database.cpp \
  		   ../../Classes/CurrentLevel.cpp \
  		   ../../Classes/sqlite3.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
