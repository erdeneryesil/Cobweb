#include "Bumblebee.h"

USING_NS_CC;

int Bumblebee::_bumblebeePoint=BUMBLEBEE_POINT;

Bumblebee::Bumblebee(){}
Bumblebee::~Bumblebee(){}
void Bumblebee::_initVectorRandom(){
    this->_point=BUMBLEBEE_POINT;

    this->_visibleSize=Director::getInstance()->getVisibleSize();

    float randomYInt=rand()%(int)(-1+this->_velocity);
    float randomYDecimal=(float)(rand()%(10*(int)this->_visibleSize.height))/(10*(int)this->_visibleSize.height);
    float randomY=randomYInt+randomYDecimal;

    float x=sqrt(pow(this->_velocity,2)-pow(randomY,2));

    int signX=rand()%2;
    if(!signX)x*=-1;

    int signY=rand()%2;
    if(!signY)randomY*=-1;

    this->_vector=new Vec2(x,randomY);
}
void Bumblebee::_changeRunParametersRandom(){
    this->_flyStatusTimer=50+std::rand()%200; //uçacağı sayaç this->_flyInterval*this->_flyCounter kadar süre uçacak 200 değiştirilebilir en az 50 birim

    float randomYInt=std::rand()%(int)(-1+this->_velocity);
    float randomYDecimal=(float)(rand()%(10*(int)this->_visibleSize.height))/(10*(int)this->_visibleSize.height);

    float randomY=randomYInt+randomYDecimal;

    float x=sqrt(pow(this->_velocity,2)-pow(randomY,2));

    float signX=this->_vector->x<0?-1:1; //vector->x'in işareti değişmemeli yoksa kilitleniyor
    x*=signX;

    float signY=this->_vector->y<0?-1:1; //vector->y'in işareti değişmemeli yoksa kilitleniyor
    randomY*=signY;

    this->_vector->set(x,randomY);
}
int Bumblebee::getPoint(){
    return Bumblebee::_bumblebeePoint;
}

void Bumblebee::_initFlySprite(){
    int frameCount=POWERFULBUG_FLY_SPRITE_FRAME_COUNT;

    this->_flySprite=Sprite::create();
    SpriteBatchNode* spriteBatch=SpriteBatchNode::create(BUMBLEBEE_FLY_SPRITE_PNG_FILE);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCache=SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(BUMBLEBEE_FLY_SPRITE_PLIST_FILE);

    Vector<SpriteFrame*>frames;
    for(int i=1;i<=frameCount;i++){
        std::string frameName=StringUtils::format("%d.png",i);
        SpriteFrame* frame=frameCache->getSpriteFrameByName(frameName.c_str());
        frames.pushBack(frame);
    }
    Animation* animation=Animation::createWithSpriteFrames(frames,POWERFULBUG_FLY_SPRITE_DURATION_PER_FRAME);
    animation->setLoops(-1);
    Animate* animate=Animate::create(animation);
    this->_flySpriteSequence=Sequence::create(animate,nullptr);
    this->_flySpriteSequence->retain();

    this->_flySprite->setPosition(Vec2(this->getBoundingBox().size.width/BUMBLEBEE_FLY_SPRITE_POS_V_RATE,this->getBoundingBox().size.height/BUMBLEBEE_FLY_SPRITE_POS_H_RATE));
    this->_flySprite->retain();
    this->addChild(this->_flySprite,-1);
    this->_flySprite->runAction(this->_flySpriteSequence);
}


