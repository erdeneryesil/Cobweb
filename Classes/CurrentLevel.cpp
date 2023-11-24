#include "CurrentLevel.h"
#include "Database.h"

CurrentLevel::CurrentLevel(){
    Database::CurrentLevelDetailed* currentLevelDetailed=Database::getInstance()->getCurrentLevelDetailed();

    this->_levelId=currentLevelDetailed->levelId;
    this->_playCount=currentLevelDetailed->playCount;
    this->_score=currentLevelDetailed->score;
    this->_highScore=currentLevelDetailed->highScore;
    this->_mosquitoCount=currentLevelDetailed->mosquitoCount;
    this->_craneCount=currentLevelDetailed->craneCount;
    this->_mothCount=currentLevelDetailed->mothCount;
    this->_ladybirdCount=currentLevelDetailed->ladybirdCount;
    this->_dragonflyCount=currentLevelDetailed->dragonflyCount;
    this->_gadflyCount=currentLevelDetailed->gadflyCount;
    this->_bumblebeeCount=currentLevelDetailed->bumblebeeCount;
    this->_robberflyCount=currentLevelDetailed->robberflyCount;
    this->_webCount=currentLevelDetailed->webCount;
    this->_bonusWebCount=currentLevelDetailed->bonusWebCount;
    this->_minRateWebBug=currentLevelDetailed->minRateWebBug;
    this->_oddsBonusMighty=currentLevelDetailed->oddsBonusMighty;
    this->_oddsBonusWeb=currentLevelDetailed->oddsBonusWeb;
    this->_maxBugCountOnStage=currentLevelDetailed->maxBugCountOnStage;

    delete currentLevelDetailed;
}
CurrentLevel::~CurrentLevel(){}
int CurrentLevel::levelId(){return this->_levelId;}
int CurrentLevel::playCount(){return this->_playCount;}
int CurrentLevel::score(){return this->_score;}
int CurrentLevel::highScore(){return this->_highScore;}
int CurrentLevel::mosquitoCount(){return this->_mosquitoCount;}
int CurrentLevel::craneCount(){return this->_craneCount;}
int CurrentLevel::mothCount(){return this->_mothCount;}
int CurrentLevel::ladybirdCount(){return this->_ladybirdCount;}
int CurrentLevel::dragonflyCount(){return this->_dragonflyCount;}
int CurrentLevel::gadflyCount(){return this->_gadflyCount;}
int CurrentLevel::bumblebeeCount(){return this->_bumblebeeCount;}
int CurrentLevel::robberflyCount(){return this->_robberflyCount;}
int CurrentLevel::webCount(){return this->_webCount;}
int CurrentLevel::bonusWebCount(){return this->_bonusWebCount;}
double CurrentLevel::minRateWebBug(){return this->_minRateWebBug;}
int CurrentLevel::oddsBonusMighty(){return this->_oddsBonusMighty;}
int CurrentLevel::oddsBonusWeb(){return this->_oddsBonusWeb;}
int CurrentLevel::maxBugCountOnStage(){return this->_maxBugCountOnStage;}
