#include "GameLoadScene.h"
#include "GamePlayScene.h"
#include "LanguageManager.h"

using namespace cocos2d;
GameLoadScene::~GameLoadScene(){
    this->_labelLevel->release();
}
GameLoadScene::GameLoadScene(){}

Scene* GameLoadScene::createScene(){
    return GameLoadScene::create();
}
bool GameLoadScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* background=Sprite::create(GAME_LOAD_BG_PNG_FILE);
    background->setPosition(visibleSize/2);
    this->addChild(background);

    this->_currentLevel=1;//eğer setCurrentLevel ile set edilmezse varsayılan olarak 1
    std::stringstream ss;
    ss<<this->_currentLevel;
    std::string currentLevelStr=ss.str();

    float rateWidtHeight=0;
    float theoricRateWidtHeight=Boundries::getTheoricWidth()/Boundries::getTheoricHeight();
    float textScale;
    if(Boundries::getPointRight()<Boundries::getTheoricWidth())
        rateWidtHeight=Boundries::getPointRight()/Boundries::getPointTop();
    else
        rateWidtHeight=Boundries::getTheoricWidth()/Boundries::getTheoricHeight();

    textScale=rateWidtHeight/theoricRateWidtHeight;

    this->_labelLevel=Label::create();
    this->_labelLevel->retain();
    this->_labelLevel->setContentSize(this->_labelLevel->getContentSize()*textScale);
    this->_labelLevel->setScale(textScale);
    this->_labelLevel->setPosition(visibleSize/2);
    this->_labelLevel->setBMFontFilePath(GAME_LOAD_FONT_FILE);
    this->_labelLevel->setString((std::string(LanguageManager::getInstance()->getStringForKey("labelLevel"))).append(currentLevelStr));
    this->addChild(this->_labelLevel);

    return true;
}
void GameLoadScene::setCurrentLevel(int currentLevel){
    this->_currentLevel=currentLevel;
    std::stringstream ss;
    ss<<this->_currentLevel;
    std::string currentLevelStr=ss.str();
    this->_labelLevel->setString((std::string(LanguageManager::getInstance()->getStringForKey("labelLevel"))).append(currentLevelStr));
}
void GameLoadScene::onEnter(){
    Node::onEnter();
    this->scheduleOnce(schedule_selector(GameLoadScene::finishSplash), 1.0f);
}
void GameLoadScene::finishSplash(float dt) {
    Director::getInstance()->replaceScene(GamePlayScene::createScene());
}
