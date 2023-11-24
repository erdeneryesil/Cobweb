#include "Dragonfly.h"

USING_NS_CC;

int Dragonfly::_dragonflyPoint=DRAGONFLY_POINT;

Dragonfly::Dragonfly(){}
Dragonfly::~Dragonfly(){}

void Dragonfly::_initVectorRandom(){
    this->_point=DRAGONFLY_POINT;

    float x=this->_velocity;
    float y=0;

    int signX=std::rand()%2;
    if(!signX)x*=-1;

    this->_vector=new Vec2(x,y);
}
void Dragonfly::_changeRunParametersRandom(){
    this->_flyStatusTimer=50+std::rand()%200; //uçacağı sayaç this->_flyInterval*this->_flyCounter kadar süre uçacak 200 değiştirilebilir en az 50 birim
}
float Dragonfly::getInitialY(){
    return this->_initialY;
}
int Dragonfly::getPoint(){
    return Dragonfly::_dragonflyPoint;
}
void Dragonfly::_initFlySprite(){
    int frameCount=POWERFULBUG_FLY_SPRITE_FRAME_COUNT;

    this->_flySprite=Sprite::create();
    SpriteBatchNode* spriteBatch=SpriteBatchNode::create(DRAGONFLY_FLY_SPRITE_PNG_FILE);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCache=SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(DRAGONFLY_FLY_SPRITE_PLIST_FILE);

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

    this->_flySprite->setPosition(Vec2(this->getBoundingBox().size.width/DRAGONFLY_FLY_SPRITE_POS_V_RATE,this->getBoundingBox().size.height/DRAGONFLY_FLY_SPRITE_POS_H_RATE));
    this->_flySprite->retain();
    this->addChild(this->_flySprite,-1);
    this->_flySprite->runAction(this->_flySpriteSequence);
}
