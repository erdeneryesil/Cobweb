#include "Spider.h"
#include "Boundries.h"
#include "GamePlayLayer.h"

USING_NS_CC;
Spider::Spider(){}
Spider::~Spider(){
    while(!this->_liveWebs->empty()){
        this->_liveWebs->popBack();
    }
    delete this->_liveWebs;


    while(!this->_deadWebs->empty()){
        this->_deadWebs->popBack();
    }
    delete this->_deadWebs;

    delete this->_attackLocationLeft;
    delete this->_attackLocationRight;
    delete this->_knitPos;

    this->_walkLSpawn->release();
    this->_walkRSpawn->release();
    this->_sameSideAnimateL->release();
    this->_sameSideAnimateR->release();

    this->_jumpBezier->release();
    this->_jumpSequence->release();

    this->_innerKnitAnimate->release();

    this->_climbMoveBy->release();
    this->_climbSpawn->release();

    this->_goBackSpawnL->release();
    this->_goBackSpawnR->release();
}
Spider* Spider::createSpider(GamePlayLayer* parentLayer,int webCount,int bonusWebCount){
    Spider* spider=new Spider();
    if(spider && spider->initSpider(parentLayer,webCount,bonusWebCount)){
        spider->autorelease();
        return spider;
    }
    CC_SAFE_DELETE(spider);

    return NULL;
}
bool Spider::initSpider(GamePlayLayer* parentLayer,int webCount,int bonusWebCount){
    this->init();

    this->_parentLayer=parentLayer;
    Web::setOwnerSpider(this);

    this->_initSprite(SPIDER_SPRITE_SPRITESHEET_PNG_FILE,SPIDER_SPRITE_SPRITESHEET_PLIST_FILE);
    this->setSpriteFrame(this->_spriteFrameRight);

    this->_attackLocationY=Boundries::getPointBridgeY()+this->getBoundingBox().size.height/LOCATION_Y_BODY_HEIGHT;
    this->_attackLocationLeft=new Vec2(Boundries::getPointSpiderLeftX(),this->_attackLocationY);
    this->_attackLocationRight=new Vec2(Boundries::getPointSpiderRightX(),this->_attackLocationY);
    this->_knitPos=new Vec2(0,0);
    this->_webActiveR=Web::getActiveR();

    this->_boundaryTop=Boundries::getPointBridgeY()-this->_webActiveR;
    this->_boundaryBottom=this->_webActiveR;
    this->_boundaryRight=Boundries::getPointRight()-this->_webActiveR;
    this->_boundaryLeft=Boundries::getPointLeft()+this->_webActiveR;

    this->_status=waiting;
    this->_currentSide=right;//değiştirilebilir right değeri
    this->setPosition(*(this->_attackLocationRight)); //değiştirilebilir right değeri


    this->_webCount=webCount;
    this->_bonusWebCount=bonusWebCount;
    this->_liveWebs=new Vector<Web*>();
    this->_deadWebs=new Vector<Web*>();

    this->_webString=WebString::createWebString();


    this->_initSameSideActions(SPIDER_SAMESIDELEFT_SPRITESHEET_PNG_FILE,SPIDER_SAMESIDELEFT_SPRITESHEET_PLIST_FILE,SPIDER_SAMESIDERIGHT_SPRITESHEET_PNG_FILE,SPIDER_SAMESIDERIGHT_SPRITESHEET_PLIST_FILE);
    this->_initWalkActions(SPIDER_SLOWWALK_SPRITESHEET_PNG_FILE,SPIDER_SLOWWALK_SPRITESHEET_PLIST_FILE);

    this->_initGoBackActions(SPIDER_SLOWWALK_SPRITESHEET_PNG_FILE,SPIDER_SLOWWALK_SPRITESHEET_PLIST_FILE);
    this->_initJumpAction(SPIDER_JUMP_SPRITESHEET_PNG_FILE,SPIDER_JUMP_SPRITESHEET_PLIST_FILE);
    this->_initInnerKnitAction(SPIDER_KNIT_SPRITESHEET_PNG_FILE,SPIDER_KNIT_SPRITESHEET_PLIST_FILE);
    this->_initClimbAction(SPIDER_CLIMB_SPRITESHEET_PNG_FILE,SPIDER_CLIMB_SPRITESHEET_PLIST_FILE);

    this->schedule(schedule_selector(Spider::_removeWebFromLayerUpdate),REMOVE_WEB_FROM_LAYER_UPDATE_INTERVAL);
    this->scheduleUpdate();

    return true;
}
void Spider::_initSprite(std::string pngFile,std::string plistFile){
    SpriteBatchNode* spriteBatchNode=SpriteBatchNode::create(pngFile);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* spriteCache=SpriteFrameCache::getInstance();
    spriteCache->addSpriteFramesWithFile(plistFile);

    this->_spriteFrameLeft=spriteCache->getSpriteFrameByName("left.png");
    this->_spriteFrameRight=spriteCache->getSpriteFrameByName("right.png");
    this->_spriteFrameCenter=spriteCache->getSpriteFrameByName("center.png");
}

void Spider::_adaptKnitPos(Vec2& knitPos){
    if(knitPos.x<this->_boundaryLeft)this->_knitPos->x=this->_boundaryLeft;
    else if(knitPos.x>this->_boundaryRight)this->_knitPos->x=this->_boundaryRight;
    else this->_knitPos->x=knitPos.x;

    if(knitPos.y<this->_boundaryBottom)this->_knitPos->y=this->_boundaryBottom;
    else if(knitPos.y>this->_boundaryTop)this->_knitPos->y=this->_boundaryTop;
    else this->_knitPos->y=knitPos.y;
}

void Spider::walk(){
    if(this->_status!=waiting)return;//sadece waiting durumunda iken walking olabilir

    if(this->_currentSide==left)this->_walkSpawn=this->_walkRSpawn;
    else this->_walkSpawn=this->_walkLSpawn;

    this->_status=triggerWalk;
}
void Spider::_initWalkActions(std::string pngFile,std::string plistFile){
    SpriteBatchNode* spriteBatch=SpriteBatchNode::create(pngFile);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCache=SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(plistFile);

    //bu değerler cihaza göre değişebilir
    this->_walkDuration=WALK_DURATION;
    this->_walkFrameCount=WALK_FRAME_COUNT;

    Vector<SpriteFrame*>frames;
    for(int i=1;i<=_walkFrameCount;i++){
        std::string frameName=StringUtils::format("%d.png",i);
        SpriteFrame* frame=frameCache->getSpriteFrameByName(frameName.c_str());
        frames.pushBack(frame);
    }

    MoveTo* moveL=MoveTo::create(this->_walkDuration,*(this->_attackLocationLeft));
    EaseSineInOut* easeL=EaseSineInOut::create(moveL);
    Animation* animationL=Animation::createWithSpriteFrames(frames,this->_walkDuration/this->_walkFrameCount);
    animationL->setLoops(1);
    Animate* animateL=Animate::create(animationL);
    this->_walkLSpawn=Spawn::create(animateL,easeL,nullptr);
    this->_walkLSpawn->retain();

    frames.reverse();
    MoveTo* moveR=MoveTo::create(this->_walkDuration,*(this->_attackLocationRight));
    EaseSineInOut* easeR=EaseSineInOut::create(moveR);
    Animation* animationR=Animation::createWithSpriteFrames(frames,this->_walkDuration/this->_walkFrameCount);
    animationR->setLoops(1);
    Animate* animateR=Animate::create(animationR);
    this->_walkRSpawn=Spawn::create(animateR,easeR,nullptr);
    this->_walkRSpawn->retain();
}
void Spider::_finishedWalkSettings(){
    this->_currentSide=(Side)((this->_currentSide+1)%2);
}

void Spider::sameSide(){
    if(this->_status!=waiting)return;//sadece waiting durumunda iken sameSiding olabilir

    if(this->_currentSide==left)this->_sameSideAnimate=this->_sameSideAnimateL;
    else if(this->_currentSide==right)this->_sameSideAnimate=this->_sameSideAnimateR;

    this->_status=triggerSameSide;
}
void Spider::_initSameSideActions(std::string pngFileL,std::string plistFileL,std::string pngFileR,std::string plistFileR){
    SpriteBatchNode* spriteBatchL=SpriteBatchNode::create(pngFileL);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCacheL=SpriteFrameCache::getInstance();
    frameCacheL->addSpriteFramesWithFile(plistFileL);

    //bu değerler cihaza göre değişebilir
    this->_sameSideFrameCount=SAME_SIDE_FRAME_COUNT;
    this->_sameSideDuration=SAME_SIDE_DURATION;

    Vector<SpriteFrame*>framesL;
    for(int i=1;i<=this->_sameSideFrameCount;i++){
        std::string frameNameL=StringUtils::format("%d.png",i);
        SpriteFrame* frameL=frameCacheL->getSpriteFrameByName(frameNameL.c_str());
        framesL.pushBack(frameL);
    }

    Animation* animationL=Animation::createWithSpriteFrames(framesL,this->_sameSideDuration/this->_sameSideFrameCount);
    animationL->setLoops(1);
    this->_sameSideAnimateL=Animate::create(animationL);
    this->_sameSideAnimateL->retain();

    SpriteBatchNode* spriteBatchR=SpriteBatchNode::create(pngFileR);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCacheR=SpriteFrameCache::getInstance();
    frameCacheR->addSpriteFramesWithFile(plistFileR);

    Vector<SpriteFrame*>framesR;
    for(int i=1;i<=this->_sameSideFrameCount;i++){
        std::string frameNameR=StringUtils::format("%d.png",i);
        SpriteFrame* frameR=frameCacheR->getSpriteFrameByName(frameNameR.c_str());
        framesR.pushBack(frameR);
    }
    Animation* animationR=Animation::createWithSpriteFrames(framesR,this->_sameSideDuration/this->_sameSideFrameCount);
    animationR->setLoops(1);
    this->_sameSideAnimateR=Animate::create(animationR);
    this->_sameSideAnimateR->retain();
}

void Spider::knit(cocos2d::Vec2& knitPos){
    //Eğer _webCount 0 ise çık
    if(this->_webCount==0)return;

//sadece waiting, walking,sameSiding durumlarında ise knit olabilecek
    switch(this->_status){
    case waiting:
    case walking:
    case sameSiding:
    case triggerWalk:
    case triggerSameSide:
        break;
    default:
        return;
    }

    this->_adaptKnitPos(knitPos);

    this->_knitDistanceL=std::abs(this->_knitPos->x-this->_attackLocationLeft->x);
    this->_knitDistanceR=std::abs(this->_knitPos->x-this->_attackLocationRight->x);

    if(this->_knitDistanceL<this->_knitDistanceR){
        this->_goBackSpawn=this->_goBackSpawnL;
        this->_currentSide=left;
    }else{
        this->_goBackSpawn=this->_goBackSpawnR;
        this->_currentSide=right;
    }
    this->_status=triggerKnit;
}
void Spider::_initJumpAction(std::string pngFile,std::string plistFile){
    SpriteBatchNode* jumpSpriteBatch=SpriteBatchNode::create(pngFile);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* jumpFrameCache=SpriteFrameCache::getInstance();
    jumpFrameCache->addSpriteFramesWithFile(plistFile);

    //bu değerler cihaza göre değişebilir
    this->_jumpShrinkRate=this->getBoundingBox().size.height/JUMP_SHRINK_RATE;
    this->_jumpDuration=JUMP_DURATION;
    this->_jumpFlyFrameCount=JUMP_FLY_FRAME_COUNT;
    this->_jumpShrinkFrameCount=JUMP_SHRINK_FRAME_COUNT;
    std::string jumpShrinkFrameFileName="bzl.png";
    this->_jumpDurationPerFrame=this->_jumpDuration/(this->_jumpFlyFrameCount+this->_jumpShrinkFrameCount);

    Vector<SpriteFrame*> jumpFramesShrink1;
    std::string frameNameShrink1=StringUtils::format("%d.png",1);
    SpriteFrame* frameShrink1=jumpFrameCache->getSpriteFrameByName(frameNameShrink1.c_str());
    jumpFramesShrink1.pushBack(frameShrink1);

    frameNameShrink1=jumpShrinkFrameFileName;
    frameShrink1=jumpFrameCache->getSpriteFrameByName(frameNameShrink1.c_str());
    jumpFramesShrink1.pushBack(frameShrink1);

    Animation* jumpAnimationShrink1=Animation::createWithSpriteFrames(jumpFramesShrink1,this->_jumpDurationPerFrame);
    jumpAnimationShrink1->setLoops(1);
    Animate* jumpAnimateShrink1=Animate::create(jumpAnimationShrink1);
    MoveBy* jumpMoveByShrink1=MoveBy::create(this->_jumpDurationPerFrame*2,Vec2(0,-this->_jumpShrinkRate));
    Spawn* jumpSpawnShrink1=Spawn::create(jumpAnimateShrink1,jumpMoveByShrink1,nullptr);
    Sequence* jumpSequenceShrink1=Sequence::create(jumpSpawnShrink1,jumpSpawnShrink1->reverse(),nullptr);

    Vector<SpriteFrame*> jumpFrames;
    for(int i=1;i<=this->_jumpFlyFrameCount-1;i++){
        std::string frameName=StringUtils::format("%d.png",i);
        SpriteFrame* frame=jumpFrameCache->getSpriteFrameByName(frameName.c_str());
        jumpFrames.pushBack(frame);
    }

    Animation* jumpAnimation=Animation::createWithSpriteFrames(jumpFrames,this->_jumpDurationPerFrame);
    jumpAnimation->setLoops(1);
    Animate* jumpAnimate=Animate::create(jumpAnimation);
    this->_jumpBezier=BezierTo::create(0,this->_jumpBezierConf);//boş oluşturuluyor
    this->_jumpBezier->retain();
    Spawn* jumpSpawn=Spawn::create(jumpAnimate,this->_jumpBezier,nullptr);

    Vector<SpriteFrame*> jumpFramesShrink2;

    std::string frameNameShrink2=StringUtils::format("%d.png",this->_jumpFlyFrameCount);
    SpriteFrame* frameShrink2=jumpFrameCache->getSpriteFrameByName(frameNameShrink2.c_str());
    jumpFramesShrink2.pushBack(frameShrink2);

    frameNameShrink2=jumpShrinkFrameFileName;
    frameShrink2=jumpFrameCache->getSpriteFrameByName(frameNameShrink2.c_str());
    jumpFramesShrink2.pushBack(frameShrink2);
    Animation* jumpAnimationShrink2=Animation::createWithSpriteFrames(jumpFramesShrink2,this->_jumpDurationPerFrame);
    jumpAnimationShrink2->setLoops(1);
    Animate* jumpAnimateShrink2=Animate::create(jumpAnimationShrink2);
    MoveBy* jumpMoveByShrink2=MoveBy::create(this->_jumpDurationPerFrame*2,Vec2(0,-this->_jumpShrinkRate));
    Spawn* jumpSpawnShrink2=Spawn::create(jumpAnimateShrink2,jumpMoveByShrink2,nullptr);
    Sequence* jumpSequenceShrink2=Sequence::create(jumpSpawnShrink2,jumpSpawnShrink2->reverse(),nullptr);

    this->_jumpSequence=Sequence::create(jumpSequenceShrink1,jumpSpawn,jumpSequenceShrink2,nullptr);
    this->_jumpSequence->retain();
}
void Spider::_preStartJumpSettings(){
    //spider ayarları
    Vec2 pos1;
    pos1.x=(this->getPosition().x-this->_knitPos->x)*0.5+this->_knitPos->x;
    pos1.y=2*this->getBoundingBox().size.height+this->getPosition().y;
    Vec2 pos2;
    pos2.x=(this->getPosition().x-this->_knitPos->x)*0.25+this->_knitPos->x;
    pos2.y=this->getPosition().y-(this->getPosition().y-this->_knitPos->y)/2;
    this->_jumpBezierConf.controlPoint_1=pos1;
    this->_jumpBezierConf.controlPoint_2=pos2;
    this->_jumpBezierConf.endPosition=*this->_knitPos;
    this->_jumpBezier->initWithDuration((this->_jumpFlyFrameCount-1)*this->_jumpDurationPerFrame,this->_jumpBezierConf);
}
void Spider::_finishedJumpSettings(){
    //webString ayarları
    this->_webString->scat(this->getPosition().x,this->getPosition().y,this->_attackLocationY-this->getBoundingBox().size.height/LOCATION_Y_BODY_HEIGHT);
}
void Spider::_initInnerKnitAction(std::string pngFile,std::string plistFile){
    SpriteBatchNode* innerKnitSpriteBatch=SpriteBatchNode::create(pngFile);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* innerKnitFrameCache=SpriteFrameCache::getInstance();
    innerKnitFrameCache->addSpriteFramesWithFile(plistFile);

    this->_innerKnitDuration=INNER_KNIT_DURATION;//tek bir döngü değil bu süre, knit animasyonu birden fazla kere tekrar edecek muhtemelen
    this->_innerKnitFrameCount=INNER_KNIT_FRAME_COUNT;
    this->_innerKnitAnimationLoopsCount=INNER_KNIT_ANIMATION_LOOPS_COUNT;


    Vector<SpriteFrame*> innerKnitframes;
    for(int i=1;i<=this->_innerKnitFrameCount;i++){
        std::string frameName=StringUtils::format("%d.png",i);
        SpriteFrame* frame=innerKnitFrameCache->getSpriteFrameByName(frameName.c_str());
        innerKnitframes.pushBack(frame);
    }

    Animation* innerKnitAnimation=Animation::createWithSpriteFrames(innerKnitframes,-1);
    innerKnitAnimation->setLoops(this->_innerKnitAnimationLoopsCount);
    this->_innerKnitAnimate=Animate::create(innerKnitAnimation);
    this->_innerKnitAnimate->setDuration(this->_innerKnitDuration);
    this->_innerKnitAnimate->retain();

}
void Spider::_preStartInnerKnitSettings(){
    //web ayarları
    Web* web=Web::createWeb(&(this->getPosition()));
    this->_parentLayer->addChild(web,WEB_ZORDER);
    this->_liveWebs->pushBack(web);
    this->_liveWebs->back()->rise();
}
void Spider::_initClimbAction(std::string pngFile,std::string plistFile){
    SpriteBatchNode* climbSpriteBatch=SpriteBatchNode::create(pngFile);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* climbFrameCache=SpriteFrameCache::getInstance();
    climbFrameCache->addSpriteFramesWithFile(plistFile);

    //bu değerler cihaza göre değişebilir
    this->_climbFrameCount=CLIMB_FRAME_COUNT;
    this->_climbDuration=CLIMB_DURATION;

    Vector<SpriteFrame*> climbFrames;
    for(int i=1;i<=this->_climbFrameCount;i++){
        std::string frameName=StringUtils::format("%d.png",i);
        SpriteFrame* frame=climbFrameCache->getSpriteFrameByName(frameName.c_str());
        climbFrames.pushBack(frame);
    }

    Animation* climbAnimation=Animation::createWithSpriteFrames(climbFrames,this->_climbDuration/(2*this->_climbFrameCount));
    climbAnimation->setLoops(2);
    Animate* climbAnimate=Animate::create(climbAnimation);

    this->_climbMoveBy=MoveBy::create(this->_climbDuration,Vec2::ZERO);//boş oluşturuluyor
    this->_climbMoveBy->retain();
    EaseBackInOut* climbEase=EaseBackInOut::create(this->_climbMoveBy);
    this->_climbSpawn=Spawn::create(climbAnimate,climbEase,nullptr);
    this->_climbSpawn->retain();
}
void Spider::_preStartClimbSettings(){
    //spider ayarları
    this->_climbMoveBy->initWithDuration(this->_climbDuration,Vec2(0,this->_attackLocationY-this->getPosition().y));
}
void Spider::_finishedClimbSettings(){
    //webStringAyarları
    this->_webString->shorten();
}
void Spider::_initGoBackActions(std::string pngFile,std::string plistFile){
    SpriteBatchNode* goBackSpriteBatch=SpriteBatchNode::create(pngFile);

    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* goBackFrameCache=SpriteFrameCache::getInstance();
    goBackFrameCache->addSpriteFramesWithFile(plistFile);

    this->_goBackFrameCount=WALK_FRAME_COUNT;
    this->_goBackDuration=GO_BACK_DURATION;

    Vector<SpriteFrame*> goBackFrames;
    for(int i=1;i<=this->_goBackFrameCount;i++){
        std::string frameName=StringUtils::format("%d.png",i);
        SpriteFrame* frame=goBackFrameCache->getSpriteFrameByName(frameName.c_str());
        goBackFrames.pushBack(frame);
    }

    MoveTo* goBackMoveL=MoveTo::create(this->_goBackDuration,*(this->_attackLocationLeft));
    EaseSineInOut* goBackEaseL=EaseSineInOut::create(goBackMoveL);
    Animation* goBackAnimationL=Animation::createWithSpriteFrames(goBackFrames,this->_goBackDuration/this->_goBackFrameCount);
    goBackAnimationL->setLoops(1);
    Animate* goBackAnimateL=Animate::create(goBackAnimationL);
    this->_goBackSpawnL=Spawn::create(goBackEaseL,goBackAnimateL,nullptr);
    this->_goBackSpawnL->retain();

    goBackFrames.reverse();
    MoveTo* goBackMoveR=MoveTo::create(this->_goBackDuration,*(this->_attackLocationRight));
    EaseSineInOut* goBackEaseR=EaseSineInOut::create(goBackMoveR);
    Animation* goBackAnimationR=Animation::createWithSpriteFrames(goBackFrames,this->_goBackDuration/this->_goBackFrameCount);
    goBackAnimationR->setLoops(1);
    Animate* goBackAnimateR=Animate::create(goBackAnimationR);
    this->_goBackSpawnR=Spawn::create(goBackEaseR,goBackAnimateR,nullptr);
    this->_goBackSpawnR->retain();
}
void Spider::knitInterrupt(){
    //sadece innerKnit action'u çalışırken bu fonksiyon çalıştırılmalı, jump, climb ve goBack action'şarında çağırılmamalı
    if(this->_status!=innerKnitting)return;

    //spider ayarları
    this->_status=interruptKnit;

    //web ayarları
    this->_liveWebs->back()->riseInterrupt();
}
void Spider::addWebStringToLayer(){
    this->_parent->addChild(this->_webString,WEBSTRING_ZORDER);
}
void Spider::moveWebFromLiveWebsToDeadWebs(Web*const web){
    this->_liveWebs->eraseObject(web);
    this->_deadWebs->pushBack(web);
}
int Spider::getWebCount(){
    return this->_webCount;
}
int Spider::getBonusWebCount(){
    return this->_bonusWebCount;
}
void Spider::decreaseWebCount(){
    this->_webCount--;
}
void Spider::increaseWebCountByBonus(){
    this->_webCount+=this->_bonusWebCount;
    this->_bonusWebCount=0;
}
void Spider::_removeWebFromLayerUpdate(float dt){
    for(Vector<Web*>::iterator itr=this->_deadWebs->begin();itr!=this->_deadWebs->end();itr++){
        Web* web=*itr;
        if(web->getStatus()==Web::dead){//dead olup olmadığı kontrol edilir, çünkü, ağ daha kapanmaya başladığı anda this->_deadWebs içine ekleniyor
            this->_parentLayer->removeChild(web);
            this->_deadWebs->eraseObject(web);
            itr--;
        }
    }
}
Vector<Web*>* Spider::getLiveWebs(){
    return this->_liveWebs;
}
WebString* Spider::getWebString(){
    return this->_webString;
}
GamePlayLayer* Spider::getParentLayer(){
    return this->_parentLayer;
}
Spider::Status Spider::getStatus(){
    return this->_status;
}
Spider::Side Spider::getCurrentSide(){
    return this->_currentSide;
}
void Spider::update(float dt){
    switch(this->_status){
    case triggerKnit:
        this->stopAllActions();
        this->_status=jumping;
        this->_preStartJumpSettings();
        this->runAction(this->_jumpSequence);
    break;
    case jumping:
        if(this->_jumpSequence->isDone()){
            this->_status=innerKnitting;
            this->_finishedJumpSettings();
            this->_preStartInnerKnitSettings();
            this->runAction(this->_innerKnitAnimate);
        }
    break;
    case innerKnitting:
        if(this->_innerKnitAnimate->isDone()){
            this->_status=climbing;
            this->_preStartClimbSettings();
            this->runAction(this->_climbSpawn);
        }

    break;
    case interruptKnit:
        this->stopAction(this->_innerKnitAnimate);
        this->_status=climbing;
        this->_preStartClimbSettings();
        this->runAction(this->_climbSpawn);
    break;
    case climbing:
        if(this->_climbSpawn->isDone()){
            this->_status=goingBack;
            this->_finishedClimbSettings();
            this->runAction(this->_goBackSpawn);
        }
    break;
    case goingBack:
        if(this->_goBackSpawn->isDone()){
            this->_status=waiting;
        }
    break;
    case triggerWalk:
        this->_status=walking;
        this->runAction(this->_walkSpawn);
    break;
    case walking:
        if(this->_walkSpawn->isDone()){
            this->_status=waiting;
            this->_finishedWalkSettings();
        }
    break;
    case triggerSameSide:
        this->_status=sameSiding;
        this->runAction(this->_sameSideAnimate);
    break;
    case sameSiding:
        if(this->_sameSideAnimate->isDone()){
            this->_status=waiting;
        }
        break;
    case waiting:
    break;
    }
}
