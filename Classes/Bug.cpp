#include "Bug.h"
#include "Boundries.h"
#include "Web.h"
#include "math.h"

USING_NS_CC;

Bug::Bug(){}
Bug::~Bug(){
    this->_flyAnimate->release();
    this->_deathSequence->release();
    this->_catchSpawn->release();

    this->_explosionSprite->release();
    this->_explosionSequence->release();

    delete this->_vector;
}

bool Bug::initBug(std::string pngFile,std::string plistFile,float velocity){
    if(!this->init()) return false;

    SpriteBatchNode* spriteBatchNode=SpriteBatchNode::create(pngFile);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* spriteCache=SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile(plistFile);

    SpriteFrame* spriteFrame=spriteCache->getSpriteFrameByName("fly1.png");
    this->setSpriteFrame(spriteFrame);

    this->_boundaryTop=Boundries::getPointBridgeY()-this->getBoundingBox().size.height/2;
    this->_boundaryBottom=this->getBoundingBox().size.height/2;
    this->_boundaryLeft=Boundries::getPointLeft()+this->getBoundingBox().size.width/2;
    this->_boundaryRight=Boundries::getPointRight()-this->getBoundingBox().size.width/2;

    this->_overflowTop=false;
    this->_overflowBottom=false;
    this->_overflowLeft=false;
    this->_overflowRight=false;
    this->_overflowCounterTop=0;
    this->_overflowCounterBottom=0;
    this->_overflowCounterLeft=0;
    this->_overflowCounterRight=0;

    this->_velocity=velocity;
    this->_initActions(pngFile,plistFile);
    this->_initExplosionSprite(BUG_EXPLOSION_SPRITE_PNG_FILE,BUG_EXPLOSION_SPRITE_PLIST_FILE);
    this->_status=idle;

    return true;
}
void Bug::flyStart(){
    this->_status=triggerFly;
    this->scheduleUpdate();
}
void Bug::_initFlip(){
    if(this->_vector->x>0)this->setFlippedX(true);
}
void Bug::_initEntrance(){
    this->_initialY=this->_generateEntranceYRandom();

    if(this->_vector->x<0)
        this->_initialX=this->_boundaryRight;
    else
        this->_initialX=this->_boundaryLeft;

    this->setPosition(this->_initialX,this->_initialY);

}
float Bug::_generateEntranceYRandom(){
    return std::rand()%((int)(this->_boundaryTop-this->_boundaryBottom))+this->_boundaryBottom;
}
void Bug::_initActions(std::string pngFile,std::string plistFile){
    SpriteBatchNode* spriteBatch=SpriteBatchNode::create(pngFile);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCache=SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(plistFile);

    //bu değerler cihaza göre değişebilir
    this->_flyInterval=BUG_FLY_INTERVAL_NUMERATOR/BUG_FLY_INTERVAL_DENOMINATOR;
    this->_overflowCounterMax=BUG_FLY_INTERVAL_DENOMINATOR;//_flyInterval'in paydası kadar, çünkü taşma süresi max 1sn olmalı
    this->_flyFrameCount=BUG_FLY_FRAME_COUNT;
    this->_deathFrameCount=BUG_DEATH_FRAME_COUNT;
    this->_catchFrameCount=BUG_CATCH_FRAME_COUNT;
    this->_flyDurationPerFrame=BUG_FLY_DURATION_PER_FRAME;
    this->_deathDuration=BUG_DEATH_DURATION;
    this->_catchDurationPerFrame=BUG_CATCH_DURATION_PER_FRAME;
    this->_catchRotateDuration=BUG_CATCH_ROTATE_DURATION;

    //fly action init
    Vector<SpriteFrame*>framesFly;
    for(int i=1;i<=this->_flyFrameCount;i++){
        std::string frameNameFly=StringUtils::format("fly%d.png",i);
        SpriteFrame* frameFly=frameCache->getSpriteFrameByName(frameNameFly.c_str());
        framesFly.pushBack(frameFly);
    }
    Animation* animationFly=Animation::createWithSpriteFrames(framesFly,this->_flyDurationPerFrame);
    animationFly->setLoops(-1);
    this->_flyAnimate=Animate::create(animationFly);
    this->_flyAnimate->retain();

    //dead action init
    Vector<SpriteFrame*>framesDeath;
    for(int i=1;i<=this->_deathFrameCount;i++){
        std::string frameNameDeath=StringUtils::format("death%d.png",i);
        SpriteFrame* frameDeath=frameCache->getSpriteFrameByName(frameNameDeath.c_str());
        framesDeath.pushBack(frameDeath);
    }
    Animation* animationDeath=Animation::createWithSpriteFrames(framesDeath,this->_deathDuration/this->_deathFrameCount);
    Animate* deathAnimate=Animate::create(animationDeath);
    this->_deathSequence=Sequence::create(deathAnimate,nullptr);
    this->_deathSequence->retain();

    //catch action init
    Vector<SpriteFrame*>framesCatch;
    for(int i=1;i<=this->_catchFrameCount;i++){
        std::string frameNameCatch=StringUtils::format("catch%d.png",i);
        SpriteFrame* frameCatch=frameCache->getSpriteFrameByName(frameNameCatch.c_str());
        framesCatch.pushBack(frameCatch);
    }
    Animation* animationCatch=Animation::createWithSpriteFrames(framesCatch,this->_catchDurationPerFrame);
    animationCatch->setLoops(-1);
    Animate* animateCatch=Animate::create(animationCatch);
    float randomRotateAngle=std::rand()%180+1;
    RotateTo* rotateCatch=RotateTo::create(this->_catchRotateDuration,randomRotateAngle);
    this->_catchSpawn=Spawn::create(rotateCatch,animateCatch,nullptr);
    this->_catchSpawn->retain();
}
void Bug::_initExplosionSprite(std::string pngFile,std::string plistFile){
    int frameCount=BUG_EXPLOSION_SPRITE_FRAME_COUNT;

    this->_explosionSprite=Sprite::create();
    SpriteBatchNode* spriteBatch=SpriteBatchNode::create(pngFile);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCache=SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(plistFile);

    Vector<SpriteFrame*>frames;
    for(int i=1;i<=frameCount;i++){
        std::string frameNameDeath=StringUtils::format("%d.png",i);
        SpriteFrame* frameDeath=frameCache->getSpriteFrameByName(frameNameDeath.c_str());
        frames.pushBack(frameDeath);
    }

    Animation* animation=Animation::createWithSpriteFrames(frames,BUG_EXPLOSION_SPRITE_DURATION_PER_FRAME);
    animation->setRestoreOriginalFrame(true);
    Animate* animate=Animate::create(animation);
    this->_explosionSequence=Sequence::create(animate,nullptr);
    this->_explosionSequence->retain();

    this->_explosionSprite->setPosition(this->getBoundingBox().size/2);
    this->_explosionSprite->retain();
    this->addChild(this->_explosionSprite);
}
void Bug::death(){
    this->_status=triggerDying;
}
void Bug::_finishedDeathSettings(){
   this->setVisible(false);
}
void Bug::catchStart(Web* web){
    this->_status=triggerCatch;
    this->_web=web;
}
void Bug::_startCatchSettings(){}

void Bug::update(float dt){

    switch(this->_status){
    //case idle:
        //break;
    case triggerFly:
        this->_status=fly;
        this->runAction(this->_flyAnimate);
        break;
    case fly:
        this->_flyUpdate();
        break;
    case triggerCatch:
        this->_status=catched;
        this->stopAction(this->_flyAnimate);
        this->_startCatchSettings();
        this->runAction(this->_catchSpawn);
        this->_explosionSprite->runAction(this->_explosionSequence);
        break;
    //case catched:
        //break;
    case triggerDying:
        this->_status=dying;
        this->stopAction(this->_catchSpawn);
        this->runAction(this->_deathSequence);
        break;
    case dying:
        if(this->_deathSequence->isDone()){
            this->_status=dead;
            this->_finishedDeathSettings();
        }
        break;
    //case dead:
        //break;
    }

}

Bug::Status Bug::getStatus(){
    return this->_status;
}
bool Bug::isMighty(){
    return this->_mighty;
}
Bug::BugType Bug::getBugType(){
    return this->_bugType;
}

void Bug::_overflowXControl(){
    if(!this->_overflowLeft && !this->_overflowRight)
        if(this->getPositionX()>=this->_boundaryLeft && this->getPositionX()<=this->_boundaryRight)return;

    if(!this->_overflowLeft){
       if(this->getPositionX()<this->_boundaryLeft){
            if(this->_vector->x<0){
                this->_vector->x*=-1;
                this->setFlippedX(true);
            }
            this->_overflowLeft=true;
            return;
        }
    }else{
        if(this->getPositionX()>=this->_boundaryLeft){
            this->_overflowLeft=false;
            this->_overflowCounterLeft=0;
        }else{
            this->setPositionX(this->_boundaryLeft);
            this->_overflowCounterLeft++;
            if(this->_overflowCounterLeft>this->_overflowCounterMax){
                this->_vector->x*=-1;
                this->setFlippedX(true);
            }
        }
        return;
    }

    if(!this->_overflowRight){
       if(this->getPositionX()>this->_boundaryRight){
            if(this->_vector->x>0){
                this->_vector->x*=-1;
                this->setFlippedX(false);
            }
            this->_overflowRight=true;
            return;
        }
    }else{
        if(this->getPositionX()<=this->_boundaryRight){
            this->_overflowRight=false;
            this->_overflowCounterRight=0;
        }else{
            this->setPositionX(this->_boundaryRight);
            this->_overflowCounterRight++;
            if(this->_overflowCounterRight>this->_overflowCounterMax){
                this->_vector->x*=-1;
                this->setFlippedX(false);
            }
        }
        return;
    }
}
void Bug::_overflowYControl(){
    if(!this->_overflowBottom && !this->_overflowTop)
        if(this->getPositionY()>=this->_boundaryBottom && this->getPositionY()<=this->_boundaryTop)return;

    if(!this->_overflowBottom){
       if(this->getPositionY()<this->_boundaryBottom){
            if(this->_vector->y<0)this->_vector->y*=-1;
            this->_overflowBottom=true;
            return;
        }
    }else{
        if(this->getPositionY()>=this->_boundaryBottom){
            this->_overflowBottom=false;
            this->_overflowCounterBottom=0;
        }else{
            this->setPositionY(this->_boundaryBottom);
            this->_overflowCounterBottom++;
            if(this->_overflowCounterBottom>this->_overflowCounterMax)this->_vector->y*=-1;
        }
        return;
    }

    if(!this->_overflowTop){
       if(this->getPositionY()>this->_boundaryTop){
            if(this->_vector->y>0)this->_vector->y*=-1;
            this->_overflowTop=true;
            return;
        }
    }else{
        if(this->getPositionY()<=this->_boundaryTop){
            this->_overflowTop=false;
            this->_overflowCounterTop=0;
        }else{
            this->setPositionY(this->_boundaryTop);
            this->_overflowCounterTop++;
            if(this->_overflowCounterTop>this->_overflowCounterMax)this->_vector->y*=-1;
        }
        return;
    }
}
