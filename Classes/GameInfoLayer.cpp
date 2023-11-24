#include "GameInfoLayer.h"
#include "GamePlayLayer.h"

using namespace cocos2d;
GameInfoLayer::~GameInfoLayer(){
    this->_flyIcon->release();
    this->_flyLabel->release();
    this->_webIcon->release();
    this->_webLabel->release();

    this->_levelLabel->release();
    this->_scoreLabel->release();
    this->_scoreFactorLabel->release();

    this->_pauseIcon->release();
    this->_resumeIcon->release();
}

GameInfoLayer::GameInfoLayer(){
    this->_webIcon=Sprite::create(WEB_ICON_PNG_FILE);
    this->_webIcon->retain();
    this->_webIcon->setPosition(Vec2(Boundries::getPointRight()-this->_webIcon->getBoundingBox().size.width,Boundries::getPointTop()-this->_webIcon->getBoundingBox().size.height/2));
    this->addChild(this->_webIcon);

    this->_webLabel=Label::createWithBMFont(GAME_INFO_FONT_FILE,"-");
    this->_webLabel->retain();
    this->_webLabel->setPosition(Vec2(Boundries::getPointRight()-this->_webIcon->getBoundingBox().size.width,Boundries::getPointTop()-this->_webLabel->getBoundingBox().size.height/2-this->_webIcon->getBoundingBox().size.height));
    this->addChild(this->_webLabel);

    this->_levelLabel=Label::createWithBMFont(GAME_INFO_FONT_FILE,"-");
    this->_levelLabel->retain();
    this->_levelLabel->setPosition(Vec2(Boundries::getPointRight()-this->_levelLabel->getBoundingBox().size.width,Boundries::getPointBottom()));
    this->addChild(this->_levelLabel);

    this->_scoreLabel=Label::createWithBMFont(GAME_INFO_FONT_FILE,"-");
    this->_scoreLabel->retain();
    this->_scoreLabel->setPosition(Vec2(Boundries::getPointLeft()+this->_scoreLabel->getBoundingBox().size.width,Boundries::getPointBottom()));
    this->addChild(this->_scoreLabel);

    this->_scoreFactorLabel=Label::createWithBMFont(GAME_INFO_FONT_FILE_BIG,"-");
    this->_scoreFactorLabel->retain();
    this->_scoreFactorLabel->setVisible(false);
    this->addChild(this->_scoreFactorLabel);


    this->_flyIcon=Sprite::create(FLY_ICON_PNG_FILE);
    this->_flyIcon->retain();
    this->_flyIcon->setPosition(Vec2(Boundries::getPointRight()-this->_flyIcon->getBoundingBox().size.width*2.5f,Boundries::getPointTop()-this->_flyIcon->getBoundingBox().size.height/2));
    this->addChild(this->_flyIcon);

    this->_flyLabel=Label::createWithBMFont(GAME_INFO_FONT_FILE,"-");
    this->_flyLabel->retain();
    this->_flyLabel->setPosition(Vec2(Boundries::getPointRight()-this->_flyIcon->getBoundingBox().size.width*2.5f,Boundries::getPointTop()-this->_flyLabel->getBoundingBox().size.height/2-this->_flyIcon->getBoundingBox().size.height));
    this->addChild(this->_flyLabel);

    this->_pauseIcon=Sprite::create(PAUSE_ICON_PNG_FILE);
    this->_pauseIcon->retain();
    this->_pauseIcon->setPosition(Vec2(Boundries::getPointLeft()+this->_pauseIcon->getBoundingBox().size.width,Boundries::getPointTop()-this->_pauseIcon->getBoundingBox().size.height*0.75f));
    this->addChild(this->_pauseIcon);

    this->_resumeIcon=Sprite::create(RESUME_ICON_PNG_FILE);
    this->_resumeIcon->retain();
    this->_resumeIcon->setPosition(Vec2(Boundries::getPointRight()/2,Boundries::getPointBridgeY()/2));
    this->_resumeIcon->setVisible(false);
    this->addChild(this->_resumeIcon);
}
void GameInfoLayer::flyLabelSetNumber(int number){
    this->_flyLabel->setString(StringUtils::toString(number));
}
void GameInfoLayer::webLabelSetNumber(int number){
    this->_webLabel->setString(StringUtils::toString(number));
}
void GameInfoLayer::levelLabelSetNumber(int number){
    this->_levelLabel->setString(StringUtils::toString(number));
}
void GameInfoLayer::scoreLabelSetNumber(int number){
    this->_scoreLabel->setString(StringUtils::toString(number));
    this->_scoreLabel->setPosition(Vec2(Boundries::getPointLeft()+this->_scoreLabel->getBoundingBox().size.width,Boundries::getPointBottom()));
}
void GameInfoLayer::setScoreLabelForGameOver(int score,int levelScore){
    this->_animScore=score;
    this->_animLevelScore=levelScore;
    this->_animIndex=levelScore;

    this->schedule(schedule_selector(GameInfoLayer::_scoreAnimationGameOver), SCORE_ANIMATION_INTERVAL);
}
void GameInfoLayer::_scoreAnimationGameOver(float dt){
    if(this->_animIndex>=0){
        std::string scoreStr=StringUtils::toString(this->_animScore+this->_animIndex);
        this->_scoreLabel->setString(scoreStr);
        this->_scoreLabel->setPosition(Vec2(Boundries::getPointLeft()+this->_scoreLabel->getBoundingBox().size.width,Boundries::getPointBottom()));
        this->_animIndex--;
    }else{
        this->unschedule(schedule_selector(GameInfoLayer::_scoreAnimationGameOver));
        ((GamePlayLayer*)(this->_parent))->gameOverOperations();
    }
}
void GameInfoLayer::setScoreLabelForWin(int score,int levelScore,int factor){
    this->_animScore=score;
    this->_animLevelScore=levelScore;
    this->_animFactor=factor;
    this->_animIndex=1;

    std::string scoreFactStr="+";
    scoreFactStr.append(StringUtils::toString(factor));
    scoreFactStr.append("x");
    this->_scoreFactorLabel->setString(scoreFactStr);
    this->_scoreFactorLabel->setVisible(true);
    this->_scoreFactorLabel->setPosition(Vec2(Boundries::getPointLeft()+this->_scoreFactorLabel->getBoundingBox().size.width/2,Boundries::getPointBottom()+this->_scoreLabel->getBoundingBox().size.height));

    this->schedule(schedule_selector(GameInfoLayer::_scoreAnimationWin), SCORE_ANIMATION_INTERVAL);
}
void GameInfoLayer::_scoreAnimationWin(float dt){
    if(this->_animIndex<=_animLevelScore*this->_animFactor){
        std::string scoreStr=StringUtils::toString(this->_animScore+this->_animLevelScore+this->_animIndex);
        this->_scoreLabel->setString(scoreStr);
        this->_scoreLabel->setPosition(Vec2(Boundries::getPointLeft()+this->_scoreLabel->getBoundingBox().size.width,Boundries::getPointBottom()));
        this->_animIndex++;
    }else{
        this->unschedule(schedule_selector(GameInfoLayer::_scoreAnimationWin));
        ((GamePlayLayer*)(this->_parent))->winOperations();
    }
}
void GameInfoLayer::showPauseIcon(){
    this->_pauseIcon->setVisible(true);
}
void GameInfoLayer::hidePauseIcon(){
    this->_pauseIcon->setVisible(false);
}
void GameInfoLayer::showResumeIcon(){
    this->_resumeIcon->setVisible(true);
}
void GameInfoLayer::hideResumeIcon(){
    this->_resumeIcon->setVisible(false);
}

bool GameInfoLayer::tapPause(Vec2& tapPos){
    if(this->_pauseIcon->getBoundingBox().containsPoint(tapPos))return true;
    return false;
}
bool GameInfoLayer::tapResume(Vec2& tapPos){
    if(this->_resumeIcon->getBoundingBox().containsPoint(tapPos))return true;
    return false;
}
