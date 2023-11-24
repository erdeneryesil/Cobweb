#include "Web.h"
#include "Spider.h"
#include "GamePlayLayer.h"
USING_NS_CC;
Spider* Web::_ownerSpider;
bool Web::_oneBug;

void Web::setOneBug(bool oneBug){
    Web::_oneBug=oneBug;
}
float Web::getActiveR(){
    Image * image = new Image();
    image->initWithImageFile(WEB_PNG_FILE);
    unsigned short width=image->getWidth()*ACTIVE_SCALE_RATE;
    delete image;

    return ((float)width)*0.5f;
}
float Web::getOriginalR(){
    Image * image = new Image();
    image->initWithImageFile(WEB_PNG_FILE);
    unsigned short width=image->getWidth();
    delete image;

    return ((float)width)*0.5f;
}
void Web::setOwnerSpider(Spider* spider){
    Web::_ownerSpider=spider;
}
Spider* Web::getOwnerSpider(){
    return Web::_ownerSpider;
}
Web::Web(){}
Web::~Web(){
    this->_riseSequence->release();
    this->_activeRepeat->release();
    this->_deathSequence->release();
    this->_deathSuddenSequence->release();

    delete this->_location;

    this->_bugs->clear();
    delete this->_bugs;
}
Web* Web::createWeb(const Vec2*const location){
    Web* web=new Web();
    if(web && web->initWeb(location)){
        web->autorelease();
        return web;
    }
    CC_SAFE_DELETE(web);
    return NULL;
}
bool Web::initWeb(const Vec2*const location){
    this->initWithFile(WEB_PNG_FILE);

    this->_location=new Vec2(location->x,location->y);
    this->setVisible(false);
    this->_activeDuration=ACTIVE_DURATION;
    this->_activeTimerDefault=this->_activeDuration/Director::getInstance()->getAnimationInterval();
    this->_activeTimer=this->_activeTimerDefault;
    this->_riseDuration=RISE_DURATION;//prerise+rotate süresi
    this->_activeScaleRate=ACTIVE_SCALE_RATE;//büyüme efekti de olduğu için png dosyası orijinal boyuttan büyük, orijinal boyuta ulaşmak için scale et
    this->_minScaleRate=MIN_SCALE_RATE;//en küçük olabileceği ebat
    this->_maxScaleRate=MAX_SCALE_RATE;//en büyük olabileceği ebat
    this->_deathDuration=DEATH_DURATION;
    this->_deathSuddenDuration=DEATH_DURATION*DEATH_SUDDEN_DURATION_RATE;//normal death den daha hızlı olmalı
    this->_status=idle;
    this->_bugs=new Vector<Bug*>();
    this->_innerBoundaryTop=this->_location->y+this->_activeScaleRate*this->getContentSize().height*INNER_BOUNDARY_RATE;
    this->_innerBoundaryBottom=this->_location->y-this->_activeScaleRate*this->getContentSize().height*INNER_BOUNDARY_RATE;
    this->_innerBoundaryLeft=this->_location->x-this->_activeScaleRate*this->getContentSize().width*INNER_BOUNDARY_RATE;
    this->_innerBoundaryRight=this->_location->x+this->_activeScaleRate*this->getContentSize().width*INNER_BOUNDARY_RATE;

    this->_initRiseAction();
    this->_initActiveAction();
    this->_initDeathAction();
    this->_initDeathSuddenAction();

    this->scheduleUpdate();

    return true;
}
void Web::_initActiveAction(){
    ScaleTo* scaleDec=ScaleTo::create(ACTIVE_INTERVAL,_activeScaleRate*ACTIVE_SCALE_DEC_RATE);
    ScaleTo* scaleInc=ScaleTo::create(ACTIVE_INTERVAL,_activeScaleRate*ACTIVE_SCALE_INC_RATE);
    RotateTo* rotateMinus=RotateTo::create(ACTIVE_INTERVAL,-ACTIVE_ROTATE_ANGLE);
    RotateTo* rotatePlus=RotateTo::create(ACTIVE_INTERVAL,ACTIVE_ROTATE_ANGLE);

    Sequence* sequenceScale=Sequence::create(scaleDec,scaleInc,nullptr);
    Sequence* sequenceRotate=Sequence::create(rotateMinus,rotatePlus,nullptr);
    Spawn* spawn=Spawn::create(sequenceScale,sequenceRotate,nullptr);

    this->_activeRepeat=Repeat::create(spawn,-1);
    this->_activeRepeat->retain();
}

void Web::update(float dt){
    switch(this->_status){
    case active:
        this->_activeTimer--;
        if(this->_activeTimer<=0){//ağın aktifliği bittiyse
            this->_activeTimer=this->_activeTimerDefault;
            this->stopAction(this->_activeRepeat);
            this->_status=dying;
            this->_preStartDeathSettings();
            this->runAction(this->_deathSequence);
        }
        break;
    case dying:
        if(this->_deathSequence->isDone()){
            this->_status=dead;
            this->_finishedDeathSettings();
        }
        break;
    case triggerRise:
        this->_status=rising;
        this->_preStartRiseSettings();
        this->runAction(this->_riseSequence);
        break;
    case rising:
        if(this->_riseSequence->isDone()){
            this->_status=active;
            this->_finishedRiseSettings();
            this->_preStartActiveSettings();
            this->runAction(this->_activeRepeat);
        }
        break;
    case risingInterrupt:
        this->_status=dyingSudden;
        this->stopAction(this->_riseSequence);
        this->runAction(this->_deathSuddenSequence);
        break;
    case dyingSudden:
        if(this->_deathSuddenSequence->isDone()){
            this->_status=dead;//*
            this->_finishedDeathSuddenSettings();
        }
        break;
    case dead:
        break;
    case idle:
        break;
    }
}

void Web::_initRiseAction(){
    float preriseDurationRate=PRERISE_DURATION_RATE;//prerise süresi _riseDuration % kaçı 0-1 arası
    float rotateDurationRate=ROTATE_DURATIONRATE;//rotate süresi _riseDuration % kaçı 0-1 arası
    float preriseDuration=this->_riseDuration*preriseDurationRate;
    float rotateDuration=this->_riseDuration*rotateDurationRate;

    ScaleTo* scalePrerise=ScaleTo::create(preriseDuration,this->_activeScaleRate);
    RotateBy* rotatePrerise=RotateBy::create(preriseDuration,RISE_ROTATE_ANGLE);
    Spawn* spawnPrerise=Spawn::create(scalePrerise,rotatePrerise,nullptr);

    RotateBy* rotateRise=RotateBy::create(rotateDuration,RISE_ROTATE_ANGLE);

    this->_riseSequence=Sequence::create(spawnPrerise,rotateRise,nullptr);
    this->_riseSequence->retain();
}
void Web::rise(){
    this->_status=triggerRise;
}
void Web::riseInterrupt(){
    this->_status=risingInterrupt;
}
void Web::_preStartRiseSettings(){
    this->setVisible(true);
    this->setScale(this->_minScaleRate);
    this->setPosition(*(this->_location));
    this->setRotation(0.0f);
}
void Web::_finishedRiseSettings(){
    this->setScale(this->_activeScaleRate);
}
void Web::_preStartActiveSettings(){
    Web::_ownerSpider->decreaseWebCount();//önce Web sayısını azalt
    Web::_ownerSpider->getParentLayer()->setWebCountLabel();//sonra azaltılan Web sayısını görüntüle
}
void Web::_initDeathAction(){
    float growDurationRate=GROW_DURATION_RATE;//başlangıç olarak büyüme süresi _deathDuration % kaçı 0-1 arası
    float shrinkDurationRate=SHRINK_DURATION_RATE;//küçülme süresi _deathDuration % kaçı 0-1 arası
    float growDuration=this->_deathDuration*growDurationRate;
    float shrinkDuration=this->_deathDuration*shrinkDurationRate;

    ScaleTo* scaleGrow=ScaleTo::create(growDuration,this->_maxScaleRate);//resmin orijinal boyutlarına kadar büyütülüyor

    ScaleTo* scaleShrink=ScaleTo::create(shrinkDuration,this->_minScaleRate);
    FadeOut* fadeOutShrink=FadeOut::create(shrinkDuration);
    RotateBy* rotateShrink=RotateBy::create(shrinkDuration,DEATH_ROTATE_ANGLE);
    Spawn* spawnShrink=Spawn::create(scaleShrink,fadeOutShrink,rotateShrink,nullptr);

    this->_deathSequence=Sequence::create(scaleGrow,spawnShrink,nullptr);
    this->_deathSequence->retain();
}
void Web::_preStartDeathSettings(){
    while(!this->_bugs->empty()){
        this->_bugs->back()->death();
        this->_bugs->popBack();
    }

    this->setVisible(true);
    this->setScale(this->_activeScaleRate);
    this->setPosition(*(this->_location));
}
void Web::_finishedDeathSettings(){
    this->setOpacity(255);
    this->setVisible(false);
    this->setRotation(0.0f);

    Web::_ownerSpider->moveWebFromLiveWebsToDeadWebs(this);

    if(Web::_ownerSpider->getParentLayer()->isGameOver())
        Web::_ownerSpider->getParentLayer()->scoreAnimationGameOver();
}

 void Web::_initDeathSuddenAction(){
    ScaleTo* scale=ScaleTo::create(this->_deathSuddenDuration,this->_minScaleRate);
    FadeOut* fadeOut=FadeOut::create(this->_deathSuddenDuration);
    RotateBy* rotateBy=RotateBy::create(this->_deathSuddenDuration,DEATH_ROTATE_ANGLE);
    Spawn* spawn=Spawn::create(scale,fadeOut,rotateBy,nullptr);

    this->_deathSuddenSequence=Sequence::create(spawn,nullptr);
    this->_deathSuddenSequence->retain();
 }
 void Web::_finishedDeathSuddenSettings(){
    this->setOpacity(255);
    this->setVisible(false);
    this->setRotation(0.0f);

    Web::_ownerSpider->moveWebFromLiveWebsToDeadWebs(this);
 }
Web::Status Web::getStatus(){
    return this->_status;
 }
 void Web::bugPutIn(Bug* bug){
    if(bug->getPositionX()<this->_innerBoundaryLeft){
        bug->setPositionX(this->_innerBoundaryLeft);

    }else if(bug->getPositionX()>this->_innerBoundaryRight){
        bug->setPositionX(this->_innerBoundaryRight);
    }

    if(bug->getPositionY()>this->_innerBoundaryTop){
        bug->setPositionY(this->_innerBoundaryTop);
    }else if(bug->getPositionY()<this->_innerBoundaryBottom){
        bug->setPositionY(this->_innerBoundaryBottom);
    }
    if(Web::_oneBug)this->_activeTimer=0;
 }
 void Web::bugPushBack(Bug* bug){
    this->_bugs->pushBack(bug);
 }
