#include "AppDelegate.h"
#include "Boundries.h"
#include "GameMenuScene.h"
#include "LanguageManager.h"
#include "Database.h"


#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1422,800);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif

    LanguageManager::deleteInstance();//Burada yok ediliyor sadece
    Database::deleteInstance();//Burada yok ediliyor sadece
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}


static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Cobweb", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Cobweb");
#endif
        director->setOpenGLView(glview);
    }

    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

    Size frameSize=glview->getFrameSize();
    Size designSizeTheoric;
    Size designSizePratic;


    std::srand(time(NULL)); //rasgele sayı üretebilmek için

    auto fileUtils=FileUtils::getInstance();
    std::vector<std::string>searchPaths;

    if (frameSize.height<=360 ){
        searchPaths.push_back("640x360");
        designSizeTheoric=Size(640,360);
        glview->setDesignResolutionSize(640,360,ResolutionPolicy::FIXED_HEIGHT);
    }else if (frameSize.height<=480){
        searchPaths.push_back("854x480");
        designSizeTheoric=Size(854,480);
        glview->setDesignResolutionSize(854,480,ResolutionPolicy::FIXED_HEIGHT);
    }else if(frameSize.height<=720){
        searchPaths.push_back("1280x720");
        designSizeTheoric=Size(1280,720);
        glview->setDesignResolutionSize(1280,720,ResolutionPolicy::FIXED_HEIGHT);
    }else if(frameSize.height<=800){
        searchPaths.push_back("1422x800");
        designSizeTheoric=Size(1422,800);
        glview->setDesignResolutionSize(1422,800,ResolutionPolicy::FIXED_HEIGHT);
    }else if(frameSize.height<=1080){
        searchPaths.push_back("1920x1080");
        designSizeTheoric=Size(1920,1080);
        glview->setDesignResolutionSize(1920,1080,ResolutionPolicy::FIXED_HEIGHT);
    }else if(frameSize.height<=1200){
        searchPaths.push_back("2133x1200");
        designSizeTheoric=Size(2133,1200);
        glview->setDesignResolutionSize(2133,1200,ResolutionPolicy::FIXED_HEIGHT);
    }else if(frameSize.height<=1536){
        searchPaths.push_back("2730x1536");
        designSizeTheoric=Size(2730,1536);
        glview->setDesignResolutionSize(2730,1536,ResolutionPolicy::FIXED_HEIGHT);
    }else{
        searchPaths.push_back("3840x2160");
        designSizeTheoric=Size(3840,2160);
        glview->setDesignResolutionSize(3840,2160,ResolutionPolicy::FIXED_HEIGHT);
    }
    fileUtils->setSearchPaths(searchPaths);
    director->setContentScaleFactor(1.0f);

    designSizePratic=glview->getDesignResolutionSize();

    Boundries::init(frameSize,designSizePratic,designSizeTheoric);

    register_all_packages();

    cocos2d::Device::setKeepScreenOn(true);//boş kalan ekranın kararmaması için android de

    auto scene = GameMenuScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
