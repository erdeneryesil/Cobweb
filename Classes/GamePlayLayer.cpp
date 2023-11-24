#include "GamePlayLayer.h"
#include "Boundries.h"
#include "GameOverScene.h"
#include"GameLoadScene.h"
#include "Database.h"
#include "Bumblebee.h"
#include "Crane.h"
#include "Dragonfly.h"
#include "Gadfly.h"
#include "Ladybird.h"
#include "Mosquito.h"
#include "Moth.h"
#include "Robberfly.h"
#include "BonusBug.h"


#define SPIDER_CLIMB_SPRITESHEET "spider/climb"

using namespace cocos2d;
GamePlayLayer::~GamePlayLayer(){
    this->removeChild(this->_gameInfoLayer);
    delete this->_gameInfoLayer;

    this->_spider->release();

    while(!this->_repositoryBugs->empty()){
        this->_repositoryBugs->popBack();
    }
    delete this->_repositoryBugs;

    while(!this->_flyingBugs->empty()){
        this->_flyingBugs->popBack();
    }
    delete this->_flyingBugs;

    while(!this->_catchedBugs->empty()){
        this->_catchedBugs->popBack();
    }
    delete this->_catchedBugs;

    while(!this->_sameLineBugsY->empty()){
        this->_sameLineBugsY->pop_back();
    }
    delete this->_sameLineBugsY;


     if(this->_bonusMighty)
        this->_bonusBugMighty->release();

    if(this->_bonusWeb)
        this->_bonusBugWeb->release();
}
GamePlayLayer::GamePlayLayer(){
    CurrentLevel* level=new CurrentLevel();//bu fonksiyon içinde delete

    int webCount=level->webCount();
    int bonusWebCount=level->bonusWebCount();
    this->_levelId=level->levelId();
    this->_playCount=level->playCount();
    this->_score=level->score();
    this->_scoreLevel=0;
    this->_highScore=level->highScore();
    this->_minRateWebBug=level->minRateWebBug();
    if(level->oddsBonusMighty()==0)this->_bonusMighty=false;
    else this->_bonusMighty=true;
    if(level->oddsBonusWeb()==0)this->_bonusWeb=false;
    else this->_bonusWeb=true;
    this->_oddsBonusMightyTop=ODDS_BONUS_BUG_MIGHTY_TOP;
    this->_oddsBonusMighty=level->oddsBonusMighty()*this->_playCount;
    this->_oddsBonusWebTop=ODDS_BONUS_BUG_WEB_TOP;
    this->_oddsBonusWeb=level->oddsBonusWeb()*this->_playCount;
    this->_maximumBugCountOnStage=level->maxBugCountOnStage();
    this->_bugCount=0;
    this->_bugCount+=this->_mosquitoCount=level->mosquitoCount();
    this->_bugCount+=this->_mothCount=level->mothCount();
    this->_bugCount+=this->_craneCount=level->craneCount();
    this->_bugCount+=this->_ladybirdCount=level->ladybirdCount();
    this->_bugCount+=this->_dragonflyCount=level->dragonflyCount();
    this->_bugCount+=this->_gadflyCount=level->gadflyCount();
    this->_bugCount+=this->_bumblebeeCount=level->bumblebeeCount();
    this->_bugCount+=this->_robberflyCount=level->robberflyCount();
    this->_pause=false;
    this->_sameLineBuffer=SAME_LINE_BUFFER_AND_WEB_R_RATION*Web::getActiveR();
    this->_maxBugCountOnSameLine=MAX_BUG_COUNT_ON_SAME_LINE;
    this->_bugTypeCount=BUG_TYPE_COUNT;

    delete level;

    EventListenerTouchOneByOne* touchListener=EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan=CC_CALLBACK_2(GamePlayLayer::touchBegan,this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,this);

    this->_gameInfoLayer=new GameInfoLayer();
    this->addChild(this->_gameInfoLayer,GAME_INFO_LAYER_ZORDER);

    float sceneWidth=Boundries::getPointRight()-Boundries::getPointLeft();
    this->_bugVelocity=sceneWidth/BUG_VELOCITY_DIVIDING;
    if(this->_bugVelocity<BUG_VELOCITY_MIN)this->_bugVelocity=BUG_VELOCITY_MIN;//this->_bugVelocity nin en az 2 olması lazım, çok önemli

    this->_repositoryBugs=new Vector<Bug*>();
    this->_initRepositoryBugs();
    this->_initBonusBugs();

    this->_flyingBugs=new Vector<Bug*>();
    this->_catchedBugs=new Vector<Bug*>();
    this->_sameLineBugsY=new std::vector<float>();

    if(this->_maximumBugCountOnStage==1)Web::setOneBug(true);//sahneden sadece 1 bug olabiliyorsa, Web de sadece tek bir sinek yakalayabilsin
    else Web::setOneBug(false);

    this->_spider=Spider::createSpider(this,webCount,bonusWebCount);
    this->_spider->retain();
    this->_webActiveR=Web::getActiveR();
    this->_webOriginalR=Web::getOriginalR();
    this->addChild(this->_spider,SPIDER_ZORDER);this->_spider->addWebStringToLayer();

    this->setWebCountLabel();
    this->setBugCountLabel();
    this->_gameInfoLayer->levelLabelSetNumber(this->_levelId);
    this->_gameInfoLayer->scoreLabelSetNumber(this->_score);

    this->scheduleUpdate();
    this->schedule(schedule_selector(GamePlayLayer::_sparseOperationsUpdate),SPARSE_OPERATIONS_UPDATE_INTERVAL);
    this->schedule(schedule_selector(GamePlayLayer::_bugTakeToStageUpdate),BUG_TAKE_TO_STAGE_UPDATE_INTERVAL);
}

void GamePlayLayer::_initBonusBugs(){
    if(this->_bonusMighty){
        this->_bonusBugMighty=(BonusBug*)BonusBug::createBonusBug(BonusBug::BonusType::disableMighty,this->_bugVelocity);
        this->_bonusBugMighty->retain();
        this->_bonusBugMightyTakeToStage=false;
    }

    if(this->_bonusWeb){
        this->_bonusBugWeb=(BonusBug*)BonusBug::createBonusBug(BonusBug::BonusType::increaseWeb,this->_bugVelocity);
        this->_bonusBugWeb->retain();
        this->_bonusBugWebTakeToStage=false;
    }
}
void GamePlayLayer::_initRepositoryBugs(){
    int bugCounts[this->_bugTypeCount]={this->_mosquitoCount,this->_mothCount,this->_craneCount,this->_ladybirdCount,this->_dragonflyCount,this->_gadflyCount,this->_bumblebeeCount,this->_robberflyCount};


    int bugTypeIndex=-1;
    while(this->_repositoryBugs->size()<this->_bugCount){
        bugTypeIndex=(bugTypeIndex+1)%this->_bugTypeCount;

        if(bugTypeIndex<0)
            bugTypeIndex=this->_bugTypeCount-1;

        if(bugCounts[bugTypeIndex]>0)
            bugCounts[bugTypeIndex]--;
        else
            continue;

        Bug* bug;
        switch(bugTypeIndex){
        case 0://bugCounts[0], yani mosquito...
            bug=(Mosquito*)Mosquito::createWeakestBug(Bug::BugType::mosquito,this->_bugVelocity*FACTOR_WEAKEST_BUG_VELOCITY);
            break;
        case 1:
            bug=(Moth*)Moth::createFeebleBug(Bug::BugType::moth,this->_bugVelocity);
            break;
        case 2:
            bug=(Crane*)Crane::createWeakestBug(Bug::BugType::crane,this->_bugVelocity*FACTOR_WEAKEST_BUG_VELOCITY);
            break;
        case 3:
            bug=(Ladybird*)Ladybird::createFeebleBug(Bug::BugType::ladybird,this->_bugVelocity);
            break;
        case 4:
            bug=(Dragonfly*)Dragonfly::createPowerfulBug(Bug::BugType::dragonfly,this->_bugVelocity);
            break;
        case 5:
            bug=(Gadfly*)Gadfly::createStrongestBug(Bug::BugType::gadfly,this->_bugVelocity);
            break;
        case 6:
            bug=(Bumblebee*)Bumblebee::createPowerfulBug(Bug::BugType::bumblebee,this->_bugVelocity);
            break;
        case 7:
            bug=(Robberfly*)Robberfly::createStrongestBug(Bug::BugType::robberfly,this->_bugVelocity);
            break;
        }
        this->_repositoryBugs->pushBack(bug);
    }
    this->_repositoryBugs->reverse();
}

void GamePlayLayer::setWebCountLabel(){
    this->_gameInfoLayer->webLabelSetNumber(this->_spider->getWebCount());
}
void GamePlayLayer::setBugCountLabel(){
    this->_gameInfoLayer->flyLabelSetNumber(this->_bugCount);
}
void GamePlayLayer::setScoreLabel(){
    this->_gameInfoLayer->scoreLabelSetNumber(this->_score+this->_scoreLevel);
}
void GamePlayLayer::increaseBugCount(){
    this->_bugCount++;
}
void GamePlayLayer::disableMightyByBonus(){
    for(Vector<Bug*>::iterator itrBug=this->_flyingBugs->begin();itrBug!=this->_flyingBugs->end();itrBug++){//sadece _flyingBugs içindekilerin gücü pasif edilecek
        Bug* bug=*itrBug;
        switch(bug->getBugType()){
        case Bug::BugType::dragonfly:
        case Bug::BugType::bumblebee:
            ((PowerfulBug*)bug)->disableMighty();
            break;
        case Bug::BugType::gadfly:
        case Bug::BugType::robberfly:
            ((StrongestBug*)bug)->disableMighty();
            break;
        }
    }
}
bool GamePlayLayer::isGameOver(){

     if(this->_maximumBugCountOnStage==1)
        if(this->_bugCount>(this->_spider->getLiveWebs()->size()+this->_spider->getWebCount()))
            return true; //sahnede tek sineğin olabildiği seviyelerde eğer, tek bir sinek bile yakalanamamışsa

    if(this->_spider->getWebCount()==0)
        if(this->_spider->getLiveWebs()->size()==0)
            if(this->_bugCount>0)
                return true;
    return false;
}
void GamePlayLayer::gameOverOperations(){
    Database::getInstance()->updatePlayCountInCurrentLevel(this->_playCount+1);
    this->scheduleOnce(schedule_selector(GamePlayLayer::_transitionGameOverScene), SCENE_TRANSITION_TIME);
}
void GamePlayLayer::scoreAnimationGameOver(){
    this->unscheduleUpdate();//update iptal et
    this->getEventDispatcher()->removeAllEventListeners();//dokunma olaylarını iptal et
    this->_gameInfoLayer->setScoreLabelForGameOver(this->_score,this->_scoreLevel);
}
void GamePlayLayer::_transitionGameOverScene(float dt){
    Scene* gameOverScene=GameOverScene::createScene();
    ((GameOverScene*)gameOverScene)->setScene(this->_score);
    Director::getInstance()->replaceScene(gameOverScene);
}
void GamePlayLayer::_transitionCongratulationsScene(float dt){
    Scene* congratulationsScene=GameOverScene::createScene();
    ((GameOverScene*)congratulationsScene)->setScene(this->_score,false);
    Director::getInstance()->replaceScene(congratulationsScene);
}
bool GamePlayLayer::isWin(){
    if(this->_bugCount==0)return true;
    return false;
}
void GamePlayLayer::winOperations(){
    this->_score+=this->_scoreLevel;
    if(this->_score>this->_highScore)
            Database::getInstance()->updateHighScoreInCurrentLevel(this->_score);

    if(this->_levelId!=Database::getLastLevelId()){
        Database::getInstance()->updateAllInCurrentLevel(this->_levelId+1,1,this->_score);
        this->scheduleOnce(schedule_selector(GamePlayLayer::_transitionGameLoadScene), SCENE_TRANSITION_TIME);
    }else{//oyun kazanılarak bittiyse
        Database::getInstance()->updateAllInCurrentLevel(1,1,0);
        this->scheduleOnce(schedule_selector(GamePlayLayer::_transitionCongratulationsScene), SCENE_TRANSITION_TIME);
    }
}
void GamePlayLayer::scoreAnimationWin(){
    this->unscheduleUpdate();//update iptal et
    this->getEventDispatcher()->removeAllEventListeners();//dokunma olaylarını iptal et
    int scoreFactor;
    if(this->_maximumBugCountOnStage==1){
        scoreFactor=SCORE_FACTOR_ONE_BUG;
    }else{
        scoreFactor=this->_spider->getWebCount();
    }

    this->_gameInfoLayer->setScoreLabelForWin(this->_score,this->_scoreLevel,scoreFactor);

    this->_score+=scoreFactor*this->_scoreLevel;
}
void GamePlayLayer::_transitionGameLoadScene(float dt){
    Scene* gameLoadScene=GameLoadScene::createScene();
    ((GameLoadScene*)gameLoadScene)->setCurrentLevel(this->_levelId+1);
    Director::getInstance()->replaceScene(gameLoadScene);
}
bool GamePlayLayer::_isBugOnWeb(const Bug*const bug, Web*const web,float webR){
    float distance=std::sqrt(std::pow(web->getPosition().x-bug->getPosition().x,2)+std::pow(web->getPosition().y-bug->getPosition().y,2));
    return distance<=webR;
}
void GamePlayLayer::update(float dt){
    for(Vector<Bug*>::iterator itrBug=this->_flyingBugs->begin();itrBug!=this->_flyingBugs->end();itrBug++){
        Bug* bug=*itrBug;
        Vector<Web*>* liveWebs=this->_spider->getLiveWebs();

        for(Vector<Web*>::iterator itrWeb=liveWebs->begin();itrWeb!=liveWebs->end();itrWeb++){//spider'ın web vectorü içinde dolaşılacak bu testWebVector
            Web* web=*itrWeb;

            if(web->getStatus()==Web::active){
                if(this->_isBugOnWeb(bug,web,this->_webActiveR)){
                    web->bugPutIn(bug);
                    web->bugPushBack(bug);
                    bug->catchStart(web);

                    this->_scoreLevel+=bug->getPoint();
                    this->setScoreLabel();

                    this->_bugCount--;
                    this->setBugCountLabel();

                    this->_flyingBugs->eraseObject(bug);
                    this->_catchedBugs->pushBack(bug);
                    itrBug--;

                    if(bug->getBugType()!=Bug::BugType::bonusbug)//son yakalanan Bonus Bug'ın ehemmiyeti yok
                        if(this->isWin())
                            this->scoreAnimationWin();//önce GameInfoLayer içinden skor animasyonu çalışacak, animasyon bittikten sonra winOperations() fonksiyonu GameInfoLayer içinden çağırılacak

                    break;
                }
            }else if(web->getStatus()==Web::rising){
                if(bug->isMighty()){
                    if(this->_isBugOnWeb(bug,web,this->_webOriginalR*web->getScale())){
                        this->_spider->knitInterrupt();
                        return;

                    }
                }/*else{
                    if(this->_isBugOnWeb(bug,web,this->_webOriginalR*web->getScale())){
                        //log("yakalayamazki");
                    }
                }*/

            }
        }
    }
}
void GamePlayLayer::_sparseOperationsUpdate(float dt){

    this->_removeDeadBugs();
    this->_refreshSpiderCurrentSide();
    this->_bonusBugOperations();
}
void GamePlayLayer::_bonusBugOperations(){

    if(this->_bonusMighty){//eğer o levelde mighty Bug varsa
        if(!this->_bonusBugMightyTakeToStage){//sadece 1 kere çalışacak
            if(this->_areBugsOnSameLine()){
                int random=std::rand()%this->_oddsBonusMightyTop;
                this->_oddsBonusMighty++;

                if(random<=this->_oddsBonusMighty){
                    this->_bonusBugMightyTakeToStage=true;

                    this->_flyingBugs->pushBack(this->_bonusBugMighty);
                    this->addChild(this->_bonusBugMighty,BUG_ZORDER);
                    this->_bonusBugMighty->flyStart();
                }
            }
        }
   }

    if(this->_bonusWeb){//eğer o levelde web Bug varsa
        if(!this->_bonusBugWebTakeToStage){//sadece 1 kere çalışacak
            if(this->_rateWebBug()){

                int random=std::rand()%this->_oddsBonusWebTop;
                    this->_oddsBonusWeb++;

                    if(random<=this->_oddsBonusWeb){
                        this->_bonusBugWebTakeToStage=true;

                        this->_flyingBugs->pushBack(this->_bonusBugWeb);
                        this->addChild(this->_bonusBugWeb,BUG_ZORDER);
                        this->_bonusBugWeb->flyStart();
                    }
            }
        }
    }
}
void GamePlayLayer::_removeDeadBugs(){
    for(Vector<Bug*>::iterator itr=this->_catchedBugs->begin();itr!=this->_catchedBugs->end();itr++){
        Bug* bug=*itr;
        if(bug->getStatus()==Bug::dead){
            this->removeChild(bug);
            this->_catchedBugs->eraseObject(bug);
            itr--;
        }
    }
}
void GamePlayLayer::_refreshSpiderCurrentSide(){
    if(this->_spider->getCurrentSide()==this->_calculateBugsHorizontalWeight())this->_spider->sameSide();
    else this->_spider->walk();
}
Spider::Side GamePlayLayer::_calculateBugsHorizontalWeight(){
    int bugCount=0;
    float horizontalWeight=0.0f;

    for(Vector<Bug*>::iterator itrBug=this->_flyingBugs->begin();itrBug!=this->_flyingBugs->end();itrBug++){
        Bug* bug=*itrBug;
        horizontalWeight+=bug->getPositionX();
        bugCount++;
    }

    horizontalWeight/=bugCount;

    float distanceL=std::abs(Boundries::getPointSpiderLeftX()-horizontalWeight);
    float distanceR=std::abs(Boundries::getPointSpiderRightX()-horizontalWeight);

    Spider::Side side;
    if(distanceL<distanceR)side=Spider::Side::left;
    else side=Spider::Side::right;

    return side;
}

bool GamePlayLayer::_areBugsOnSameLine(){
    this->_sameLineBugsY->clear();

    for(Vector<Bug*>::iterator itrBug=this->_flyingBugs->begin();itrBug!=this->_flyingBugs->end();itrBug++){
        Bug* bug=*itrBug;
        if(bug->getBugType()==Bug::BugType::gadfly){
            this->_sameLineBugsY->push_back(((Gadfly*)bug)->getInitialY());
        }else if(bug->getBugType()==Bug::BugType::dragonfly){
            this->_sameLineBugsY->push_back(((Dragonfly*)bug)->getInitialY());
        }
    }

    if(this->_sameLineBugsY->empty())return false;

    std::sort(this->_sameLineBugsY->begin(),this->_sameLineBugsY->end());

    int sameLineYCountMax=0;
    for(std::vector<float>::iterator outerItrY=this->_sameLineBugsY->begin();outerItrY!=this->_sameLineBugsY->end();outerItrY++){

        float firstY=this->_sameLineBugsY->back();
        this->_sameLineBugsY->pop_back();
        outerItrY--;

        int sameLineYCount=0;
        for(std::vector<float>::iterator innerItrY=this->_sameLineBugsY->begin();innerItrY!=this->_sameLineBugsY->end();innerItrY++){
            float y=*innerItrY;
            float distance=std::abs(firstY-y);
            if(distance<(this->_sameLineBuffer)){
                sameLineYCount++;
                this->_sameLineBugsY->erase(innerItrY);
                innerItrY--;
            }
        }
        if(sameLineYCount>sameLineYCountMax)sameLineYCountMax=sameLineYCount;
    }

    if(sameLineYCountMax>0)sameLineYCountMax++;//sınanan sinek de ekleniyor
    return (sameLineYCountMax>=this->_maxBugCountOnSameLine);
}
bool GamePlayLayer::_rateWebBug(){
    float currentRateWebBug=(float)this->_spider->getWebCount()/(float)this->_bugCount;

    if(currentRateWebBug<=this->_minRateWebBug)return true;
    return false;
}
bool GamePlayLayer::touchBegan(cocos2d::Touch*touch,cocos2d::Event*event){
        Vec2 location=touch->getLocation();

        if(!this->_pause){
            if(this->_gameInfoLayer->tapPause(location)){
                this->_pauseGame();
            }else{
                this->_spider->knit(location);
            }
        }else{
            if(this->_gameInfoLayer->tapResume(location)){
                this->_resumeGame();
            }
        }

    return true;
}

void GamePlayLayer::_pauseGame(){
    this->_pause=true;
    this->_gameInfoLayer->hidePauseIcon();
    this->_gameInfoLayer->showResumeIcon();

    Director::getInstance()->pause();
}
void GamePlayLayer::_resumeGame(){
    this->_pause=false;
    this->_gameInfoLayer->hideResumeIcon();
    this->_gameInfoLayer->showPauseIcon();

    Director::getInstance()->resume();
}
void GamePlayLayer::_bugTakeToStageUpdate(float dt){
    if(this->_repositoryBugs->size()==0){
        this->unschedule(schedule_selector(GamePlayLayer::_bugTakeToStageUpdate));
        return;
    }
    int bugCountOnStage=this->_flyingBugs->size();

    if(this->_flyingBugs->contains(this->_bonusBugWeb))bugCountOnStage--;//ekranda bonusBug varsa onu sayma
    if(this->_flyingBugs->contains(this->_bonusBugMighty))bugCountOnStage--;//ekranda bonusBug varsa onu sayma

    if(bugCountOnStage<this->_maximumBugCountOnStage){
        Bug* bug=this->_repositoryBugs->back();
        this->_flyingBugs->pushBack(bug);
        this->_repositoryBugs->popBack();

        this->addChild(bug,BUG_ZORDER);
        bug->flyStart();
    }
}
