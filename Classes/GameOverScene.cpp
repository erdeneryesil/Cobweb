#include "GameOverScene.h"
#include "GameMenuScene.h"
#include "LanguageManager.h"
#include "Boundries.h"

using namespace cocos2d;
GameOverScene::~GameOverScene(){
    this->_labelResult->release();
    this->_labelScoreTitle->release();
    this->_labelScore->release();
}

GameOverScene::GameOverScene(){}

Scene* GameOverScene::createScene(){
    return GameOverScene::create();
}
bool GameOverScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* background=Sprite::create(GAME_OVER_BG_PNG_FILE);
    background->setPosition(visibleSize/2);
    this->addChild(background);


    float rateWidtHeight=0;
    float theoricRateWidtHeight=Boundries::getTheoricWidth()/Boundries::getTheoricHeight();
    float textScale;
    if(Boundries::getPointRight()<Boundries::getTheoricWidth())
        rateWidtHeight=Boundries::getPointRight()/Boundries::getPointTop();
    else
        rateWidtHeight=Boundries::getTheoricWidth()/Boundries::getTheoricHeight();

    textScale=rateWidtHeight/theoricRateWidtHeight;

    this->_labelResult=Label::create();
    this->_labelResult->retain();
    this->_labelResult->setContentSize(this->_labelResult->getContentSize()*textScale);
    this->_labelResult->setScale(textScale);
    this->_labelResult->setPosition(Vec2(visibleSize.width/2,visibleSize.height*2/3));
    this->_labelResult->setBMFontFilePath(GAME_OVER_FONT_FILE);
    this->_labelResult->setString("-");
    this->addChild(this->_labelResult);

    this->_labelScoreTitle=Label::create();
    this->_labelScoreTitle->retain();
    this->_labelScoreTitle->setContentSize(this->_labelScoreTitle->getContentSize()*textScale);
    this->_labelScoreTitle->setScale(textScale);
    this->_labelScoreTitle->setPosition(Vec2(visibleSize.width/2,visibleSize.height*2/3-1.5f*this->_labelResult->getBoundingBox().size.height));
    this->_labelScoreTitle->setBMFontFilePath(GAME_OVER_FONT_FILE);
    this->_labelScoreTitle->setString(LanguageManager::getInstance()->getStringForKey("labelScore"));
    this->addChild(this->_labelScoreTitle);

    this->_labelScore=Label::create();
    this->_labelScore->retain();
    this->_labelScore->setContentSize(this->_labelScore->getContentSize()*textScale);
    this->_labelScore->setScale(textScale);
    this->_labelScore->setPosition(Vec2(visibleSize.width/2,visibleSize.height*2/3-1.5f*this->_labelResult->getBoundingBox().size.height-1.0f*this->_labelScoreTitle->getBoundingBox().size.height));
    this->_labelScore->setBMFontFilePath(GAME_OVER_FONT_FILE);
    this->_labelScore->setString("-");
    this->addChild(this->_labelScore);

    return true;
}

void GameOverScene::onEnter(){
    Node::onEnter();
    this->scheduleOnce(schedule_selector(GameOverScene::finishSplash), 2.0f);
}
void GameOverScene::finishSplash(float dt) {
    Director::getInstance()->replaceScene(GameMenuScene::createScene());
}
void GameOverScene::setScene(int score,bool gameOver){
    if(gameOver)
        this->_labelResult->setString(LanguageManager::getInstance()->getStringForKey("labelGameOver"));
    else
        this->_labelResult->setString(LanguageManager::getInstance()->getStringForKey("labelCongratulations"));

    this->_labelScore->setString(StringUtils::toString(score));
}
