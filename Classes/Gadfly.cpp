#include "Gadfly.h"
#include "Web.h"

USING_NS_CC;

int Gadfly::_gadflyPoint=GADFLY_POINT;

Gadfly::Gadfly(){}
Gadfly::~Gadfly(){}

void Gadfly::_initVectorRandom(){
    this->_point=GADFLY_POINT;

    float x=this->_velocity;

    int signX=rand()%2;
    if(!signX)x*=-1;

    this->_vector=new Vec2(x,0);
}
void Gadfly::_changeRunParametersRandom(){
    this->_flyStatusTimer=50+std::rand()%100; //uçacağı sayaç this->_flyInterval*this->_flyCounter kadar süre uçacak 100 değiştirilebilir en az 50 birim
    this->setPositionY(this->_initialY);//döndüğünde 359-361 arası bir açıda dönmüş olabilir, onu düzeltmek amacıyla bu satır
}
void Gadfly::_changeRevolveParametersRandom(){
    this->_revolveR=this->_revolveQuarterR+std::rand()%((int)(this->_revolveQuarterR));//çizilecek çemberin yarıçapı değeri
    this->_revolveIterationValue=(this->_velocity*360)/(2*PI*this->_revolveR);
    this->_flyStatusTimer=(int)(360.0f/this->_revolveIterationValue);//döneceği  sayaç 360 derece
    this->_lapCount=1+std::rand()%GADFLY_MAX_LAP_CAOUNT;//en fazla 2 tur
    this->_flyStatusTimer*=this->_lapCount;

    int signRevolveIterationValue=rand()%2;
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
float Gadfly::getInitialY(){
    return this->_initialY;
}
int Gadfly::getPoint(){
    return Gadfly::_gadflyPoint;
}
void Gadfly::_initFlySprite(){
    int frameCount=STRONGESTBUG_FLY_SPRITE_FRAME_COUNT;

    this->_flySprite=Sprite::create();
    SpriteBatchNode* spriteBatch=SpriteBatchNode::create(GADFLY_FLY_SPRITE_PNG_FILE);
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    SpriteFrameCache* frameCache=SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile(GADFLY_FLY_SPRITE_PLIST_FILE);

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

    this->_flySprite->setPosition(Vec2(this->getBoundingBox().size.width/GADFLY_FLY_SPRITE_POS_V_RATE,this->getBoundingBox().size.height/GADFLY_FLY_SPRITE_POS_H_RATE));
    this->_flySprite->retain();
    this->addChild(this->_flySprite,-1);
    this->_flySprite->runAction(this->_flySpriteSequence);
}
