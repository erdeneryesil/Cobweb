#include "PowerfulBug.h"
#include "Dragonfly.h"
#include "Bumblebee.h"
#include "Boundries.h"

USING_NS_CC;

PowerfulBug::PowerfulBug(){}
PowerfulBug::~PowerfulBug(){
    this->_flySprite->release();
    this->_flySpriteSequence->release();
}
PowerfulBug* PowerfulBug::createPowerfulBug(BugType bugType,float velocity){
    PowerfulBug* powerfulBug;
    std::string pngFile;
    std::string plistFile;

    if(bugType==dragonfly){
        powerfulBug=new Dragonfly();
        pngFile=DRAGONFLY_PNG_FILE;
        plistFile=DRAGONFLY_PLIST_FILE;
    }else if(bugType==bumblebee){
        powerfulBug=new Bumblebee();
        pngFile=BUMBLEBEE_PNG_FILE;
        plistFile=BUMBLEBEE_PLIST_FILE;
    }

    if(powerfulBug&& powerfulBug->initPowerfulBug(bugType,pngFile,plistFile,velocity)){
        powerfulBug->autorelease();
        return powerfulBug;
    }
    CC_SAFE_DELETE(powerfulBug);

    return NULL;
}
bool PowerfulBug::initPowerfulBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity){
    if(!this->initBug(pngFile,plistFile,velocity))return false;

    this->_bugType=bugType;
    this->_mighty=true;

    this->_initVectorRandom();//this->_vector...
    this->_initFlip();//hareket edeceği yöne göre döndür
    this->_initEntrance();
    this->_initFlySprite();

    this->_initFlyStatusRandom();
    this->_setFlyStatusFunctionPointer();

    return true;
}
void PowerfulBug::disableMighty(){
    this->_explosionSprite->runAction(this->_explosionSequence);
    this->_flySprite->setVisible(false);
    this->_flySprite->stopAction(this->_flySpriteSequence);
    this->_mighty=false;
}
void PowerfulBug::_changeHangParametersRandom(){
    this->_flyStatusTimer=50+std::rand()%100;//askıda kalacağı sayaç this->_flyInterval*this->hangCounter kadar süre askıda kalacak 100 değiştirilebilir en az 50 birim
}
void PowerfulBug::_initFlyStatusRandom(){
    this->_flyStatus=(FlyStatus)(rand()%2);
}
void PowerfulBug::_flyUpdate(){
    if(this->_flyStatusTimer<=0){
        this->_setFlyStatus((FlyStatus)((((int)this->_flyStatus)+1)%2));
        this->_setFlyStatusFunctionPointer();
    }

    (this->*_flyStatusFunctionPointer)();
    this->_flyStatusTimer--;
}
void PowerfulBug::_runFunction(){
    this->setPosition(this->getPosition()+*(this->_vector));

    this->_overflowXControl();
    this->_overflowYControl();
}
void PowerfulBug::_hangFunction(){}
void PowerfulBug::_setFlyStatus(FlyStatus flyStatus){
    this->_flyStatus=flyStatus;
}
void PowerfulBug::_setFlyStatusFunctionPointer(){
    if(this->_flyStatus==run){
        this->_flyStatusFunctionPointer=&PowerfulBug::_runFunction;
        this->_changeRunParametersRandom();
    }
    else if(this->_flyStatus==hang){
        this->_flyStatusFunctionPointer=&PowerfulBug::_hangFunction;
        this->_changeHangParametersRandom();
        this->_hangOverflowControl();//taşmayı bir kere kontrol etmek yeterli, zaten asılı, sabit kalacağı için
    }
}
void PowerfulBug::_hangOverflowControl(){
    if(this->getPositionY()<this->_boundaryBottom){
        this->setPositionY(this->_boundaryBottom);
    }else if(this->getPositionY()>this->_boundaryTop){
        this->setPositionY(this->_boundaryTop);
    }

    if(this->getPositionX()<this->_boundaryLeft){
        this->setPositionX(this->_boundaryLeft);
    }else if(this->getPositionX()>this->_boundaryRight){
        this->setPositionX(this->_boundaryRight);
    }
}
void PowerfulBug::catchStart(Web* web){
    Bug::catchStart(web);
    this->_flySprite->setVisible(false);
    this->_flySprite->stopAction(this->_flySpriteSequence);
}
