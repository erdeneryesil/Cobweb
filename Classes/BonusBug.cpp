#include "BonusBug.h"
#include "Web.h"
#include "Spider.h"
#include "GamePlayLayer.h"

USING_NS_CC;
int BonusBug::_bonusBugPoint=BONUSBUG_POINT;

BonusBug::BonusBug(){}
BonusBug::~BonusBug(){
    this->_flySpriteSequence->release();
    this->_flySprite->release();
}
BonusBug* BonusBug::createBonusBug(BonusType bonusType,float velocity){
    BonusBug* bonusBug=new BonusBug();
    std::string pngFile=BONUSBUG_PNG_FILE;
    std::string plistFile=BONUSBUG_PLIST_FILE;

    if(bonusBug&& bonusBug->initBonusBug(pngFile,plistFile,bonusType,velocity)){
        bonusBug->autorelease();
        return bonusBug;
    }
    CC_SAFE_DELETE(bonusBug);

    return NULL;
}
void BonusBug::_initVectorRandom(){
    this->_point=BONUSBUG_POINT;

    float x=this->_velocity;
    float y=0;

    int signX=std::rand()%2;
    if(!signX)x*=-1;

    this->_vector=new Vec2(x,y);

    this->_vectorYUpdateDefault=this->_visibleSize.height/BONUSBUG_CONSTANT_1;
    this->_setRunTimerRandom();
    this->_vectorYUpdate=this->_vectorYUpdateDefault;
    this->_vectorYUpdateDecrease=this->_vectorYUpdateDefault/this->_flyStatusTimer;
}
bool BonusBug::initBonusBug(std::string pngFile,std::string plistFile,BonusType bonusType,float velocity){
    if(!this->initBug(pngFile,plistFile,velocity))return false;

    this->_bugType=BugType::bonusbug;
    this->_bonusType=bonusType;
    this->_mighty=false;

    this->_visibleSize=Director::getInstance()->getVisibleSize();
    this->_initVectorRandom();//this->_vector...
    this->_initFlip();//hareket edeceği yöne göre döndür
    this->_initEntrance();
    this->_initFlySprite();

    this->_flayStatusArray[0]=run;
    this->_flayStatusArray[1]=descent;
    this->_flayStatusArray[2]=run;
    this->_flayStatusArray[3]=rise;
    this->_flyStatusIndex=0;

    this->_setFlyStatus(this->_flayStatusArray[this->_flyStatusIndex]);
    this->_setFlyStatusFunctionPointer();

    return true;
}

void BonusBug::catchStart(Web* web){
    Bug::catchStart(web);

    this->_flySprite->setVisible(false);
    this->_flySprite->stopAction(this->_flySpriteSequence);

    if(this->_bonusType==disableMighty){
        Web::getOwnerSpider()->getParentLayer()->disableMightyByBonus();
    }else if(this->_bonusType==increaseWeb){
        Web::getOwnerSpider()->increaseWebCountByBonus();
        Web::getOwnerSpider()->getParentLayer()->setWebCountLabel();
    }
    Web::getOwnerSpider()->getParentLayer()->increaseBugCount();
}
void BonusBug::_setRunTimerRandom(){
    int random=this->_visibleSize.height+std::rand()%(int)this->_visibleSize.height;//
    this->_flyStatusTimer=random/(this->_visibleSize.height/BONUSBUG_CONSTANT_2);
}
void BonusBug::_setDescenRiseTimerRandom(){
    int random=this->_visibleSize.height+std::rand()%(int)this->_visibleSize.height;
    this->_flyStatusTimer=BONUSBUG_CONSTANT_4*random/(this->_visibleSize.height/BONUSBUG_CONSTANT_2);
}
void BonusBug::_changeDescentRiseParametersRandom(){
    this->_setDescenRiseTimerRandom();
    this->_vector->y=0.0f;
    this->_vectorYUpdate=this->_vectorYUpdateDefault;
    this->_vectorYUpdateDecrease=this->_vectorYUpdateDefault/this->_flyStatusTimer;
}
void BonusBug::_changeRunParametersRandom(){
    this->_setRunTimerRandom();
    this->_vector->y=0.0f;
}
void BonusBug::_flyUpdate(){
    if(this->_flyStatusTimer<=0){
        this->_setFlyStatus(this->_flayStatusArray[(++this->_flyStatusIndex)%BONUSBUG_FLY_STATUS_COUNT]);
        this->_setFlyStatusFunctionPointer();
    }

    (this->*_flyStatusFunctionPointer)();
    this->_flyStatusTimer--;
}

void BonusBug::_runFunction(){
    this->setPosition(this->getPosition()+*(this->_vector));

    this->_overflowXControl();
    this->_overflowYControl();
}
void BonusBug::_descentFunction(){
    this->setPosition(this->getPosition()+*(this->_vector));

    this->_vector->y-=this->_vectorYUpdate;
    this->_vectorYUpdate-=this->_vectorYUpdateDecrease;

    this->_overflowXControl();
    this->_overflowYControl();
    this->_descentOverflowControl();
}
void BonusBug::_riseFunction(){
    this->setPosition(this->getPosition()+*(this->_vector));

    this->_vector->y+=this->_vectorYUpdate;
    this->_vectorYUpdate-=this->_vectorYUpdateDecrease;

    this->_overflowXControl();
    this->_overflowYControl();
    this->_riseOverflowControl();
}
void BonusBug::_setFlyStatus(FlyStatus flyStatus){
    this->_flyStatus=flyStatus;
}
void BonusBug::_setFlyStatusFunctionPointer(){
    if(this->_flyStatus==descent){
        this->_flyStatusFunctionPointer=&BonusBug::_descentFunction;
        this->_changeDescentRiseParametersRandom();
    }else if(this->_flyStatus==rise){
        this->_flyStatusFunctionPointer=&BonusBug::_riseFunction;
        this->_changeDescentRiseParametersRandom();
    }else if(this->_flyStatus==run){
        this->_flyStatusFunctionPointer=&BonusBug::_runFunction;
        this->_changeRunParametersRandom();
    }
}
void BonusBug::_descentOverflowControl(){
    if(this->getPositionY()<this->_boundaryBottom){
        this->_flyStatusTimer=0;
    }
}
void BonusBug::_riseOverflowControl(){
    if(this->getPositionY()>this->_boundaryTop){
        this->_flyStatusTimer=0;
    }
}
BonusBug::BonusType BonusBug::getBonusType(){
    return this->_bonusType;
}
int BonusBug::getPoint(){
    return BonusBug::_bonusBugPoint;
}
void BonusBug::_initFlySprite(){
    int frameCount=BONUSBUG_FLY_SPRITE_FRAME_COUNT;

    this->_flySprite=Sprite::create();
    SpriteBatchNode* spriteBatch=SpriteBatchNode::create(BONUSBUG_FLY_SPRITE_PNG_FILE);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCache=SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(BONUSBUG_FLY_SPRITE_PLIST_FILE);

    Vector<SpriteFrame*>frames;
    for(int i=1;i<=frameCount;i++){
        std::string frameName=StringUtils::format("%d.png",i);
        SpriteFrame* frame=frameCache->getSpriteFrameByName(frameName.c_str());
        frames.pushBack(frame);
    }
    Animation* animation=Animation::createWithSpriteFrames(frames,BONUSBUG_FLY_SPRITE_DURATION_PER_FRAME);
    animation->setLoops(-1);
    Animate* animate=Animate::create(animation);
    this->_flySpriteSequence=Sequence::create(animate,nullptr);
    this->_flySpriteSequence->retain();

    this->_flySprite->setPosition(Vec2(this->getBoundingBox().size.width/BONUSBUG_FLY_SPRITE_POS_V_RATE,this->getBoundingBox().size.height/BONUSBUG_FLY_SPRITE_POS_H_RATE));
    this->_flySprite->retain();
    this->addChild(this->_flySprite,-1);
    this->_flySprite->runAction(this->_flySpriteSequence);
}


