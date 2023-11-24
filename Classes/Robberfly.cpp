#include "Robberfly.h"

USING_NS_CC;

int Robberfly::_robberflyPoint=ROBBERFLY_POINT;

Robberfly::Robberfly(){}
Robberfly::~Robberfly(){}

void Robberfly::_initVectorRandom(){
    this->_point=ROBBERFLY_POINT;

    float randomYInt=std::rand()%(int)(this->_velocity-1);
    float randomYDecimal=(float)(rand()%9999)/10000;
    float randomY=randomYInt+randomYDecimal;

    float x=sqrt(pow(this->_velocity,2)-pow(randomY,2));

    int signX=rand()%2;
    if(!signX)x*=-1;

    int signY=rand()%2;
    if(!signY)randomY*=-1;

    this->_vector=new Vec2(x,randomY);
}
void Robberfly::_changeRunParametersRandom(){
    this->_flyStatusTimer=50+rand()%100; //uçacağı sayaç this->_flyInterval*this->_flyCounter kadar süre uçacak 100 değiştirilebilir en az 50 birim

    float randomYInt=rand()%(int)(this->_velocity-1);
    float randomYDecimal=(float)(rand()%9999)/10000;
    float randomY=randomYInt+randomYDecimal;

    float x=sqrt(pow(this->_velocity,2)-pow(randomY,2));

    int signX=rand()%2;
    if(!signX)x*=-1;
    if(x<0)this->setFlippedX(false);
    else this->setFlippedX(true);

    int signY=rand()%2;
    if(!signY)randomY*=-1;

    this->_vector->set(x,randomY);
}
void Robberfly::_changeRevolveParametersRandom(){
    this->_revolveR=this->_revolveQuarterR+std::rand()%((int)this->_revolveQuarterR);//çizilecek çemberin yarıçapı değeri....
    this->_revolveIterationValue=(this->_velocity*360)/(2*PI*this->_revolveR);
    int minStatusTimerDefault=(int)(720.0f/this->_revolveIterationValue)+1;//en az 720 derece dönmesi için
    this->_flyStatusTimer=minStatusTimerDefault+std::rand()%(1*minStatusTimerDefault);//döneceği  sayaç en az 720 derece en fazla 360*4 derece, robberfly dönme sayısı bununla uyumlu olmalı

    int signRevolveIterationValue=std::rand()%2;
    if(!signRevolveIterationValue)this->_revolveIterationValue*=-1;

    if(this->_vector->x>0){
        this->_revolveCenterPoint->set(this->getPositionX()+this->_revolveR,this->getPositionY());
        this->_revolveCurrentAngle=180.0f;
    }else{
        this->_revolveCenterPoint->set(this->getPositionX()-this->_revolveR,this->getPositionY());
        this->_revolveCurrentAngle=0.0f;
    }

    this->_revolveStartY=this->getPositionY();
}
int Robberfly::getPoint(){
    return Robberfly::_robberflyPoint;
}
void Robberfly::_initFlySprite(){
    int frameCount=STRONGESTBUG_FLY_SPRITE_FRAME_COUNT;

    this->_flySprite=Sprite::create();
    SpriteBatchNode* spriteBatch=SpriteBatchNode::create(ROBBERFLY_FLY_SPRITE_PNG_FILE);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCache=SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(ROBBERFLY_FLY_SPRITE_PLIST_FILE);

    Vector<SpriteFrame*>frames;
    for(int i=1;i<=frameCount;i++){
        std::string frameName=StringUtils::format("%d.png",i);
        SpriteFrame* frame=frameCache->getSpriteFrameByName(frameName.c_str());
        frames.pushBack(frame);
    }
    Animation* animation=Animation::createWithSpriteFrames(frames,STRONGESTBUG_FLY_SPRITE_DURATION_PER_FRAME);
    animation->setLoops(-1);
    Animate* animate=Animate::create(animation);
    this->_flySpriteSequence=Sequence::create(animate,nullptr);
    this->_flySpriteSequence->retain();

    this->_flySprite->setPosition(Vec2(this->getBoundingBox().size.width/ROBBERFLY_FLY_SPRITE_POS_V_RATE,this->getBoundingBox().size.height/ROBBERFLY_FLY_SPRITE_POS_H_RATE));
    this->_flySprite->retain();
    this->addChild(this->_flySprite,-1);
    this->_flySprite->runAction(this->_flySpriteSequence);
}


