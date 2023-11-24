#include "StrongestBug.h"
#include "Robberfly.h"
#include "Gadfly.h"
#include "Web.h"
#include "Boundries.h"

USING_NS_CC;

StrongestBug::StrongestBug(){}
StrongestBug::~StrongestBug(){
    this->_flySprite->release();
    this->_flySpriteSequence->release();
    delete this->_revolveCenterPoint;
}
StrongestBug* StrongestBug::createStrongestBug(BugType bugType,float velocity){
    StrongestBug* strongestBug;
    std::string pngFile;
    std::string plistFile;
    if(bugType==gadfly){
        strongestBug=new Gadfly();
        pngFile=GADFLY_PNG_FILE;
        plistFile=GADFLY_PLIST_FILE;
    }else if(bugType==robberfly){
        strongestBug=new Robberfly();
        pngFile=ROBBERFLY_PNG_FILE;
        plistFile=ROBBERFLY_PLIST_FILE;
    }

    if(strongestBug&& strongestBug->initStrongestBug(bugType,pngFile,plistFile,velocity)){
        strongestBug->autorelease();
        return strongestBug;
    }
    CC_SAFE_DELETE(strongestBug);

    return NULL;
}
bool StrongestBug::initStrongestBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity){
    if(!this->initBug(pngFile,plistFile,velocity))return false;

    this->_bugType=bugType;
    this->_mighty=true;
    this->_revolveQuarterR=Web::getActiveR()*REVOLVE_QUARTES_R_FACTOR;

    this->_initVectorRandom();//this->_vector...
    this->_initFlip();//hareket edeceği yöne göre döndür
    this->_initEntrance();
    this->_initFlySprite();

    this->_revolveCenterPoint=new Vec2(Vec2::ZERO);
    this->_initFlyStatusRandom();
    this->_setFlyStatusFunctionPointer();

    return true;
}
void StrongestBug::disableMighty(){
    this->_explosionSprite->runAction(this->_explosionSequence);
    this->_flySprite->setVisible(false);
    this->_flySprite->stopAction(this->_flySpriteSequence);
    this->_mighty=false;
}
void StrongestBug::_initFlyStatusRandom(){
    this->_flyStatus=(FlyStatus)(std::rand()%2);
}
void StrongestBug::_flyUpdate(){
    if(this->_flyStatusTimer<=0){
        this->_setFlyStatus((FlyStatus)((((int)this->_flyStatus)+1)%2));// run(0) iken, revolve(1)'a, revolve(1) iken run(0)'a geçiyor. _flyStatusTimer'a bağlı olarak asla revolveBack(2) durumuna geçmez, geçmemeli
        this->_setFlyStatusFunctionPointer();
    }

    (this->*_flyStatusFunctionPointer)();
    this->_flyStatusTimer--;

}
void StrongestBug::_runFunction(){
    this->setPosition(this->getPosition()+*(this->_vector));

    this->_overflowXControl();
    this->_overflowYControl();
}
void StrongestBug::_revolveFunction(){
    Vec2 revolvePoint;
    revolvePoint.x=this->_revolveCenterPoint->x+cos(this->_revolveCurrentAngle*PI/180.0f)*this->_revolveR;
    revolvePoint.y=this->_revolveCenterPoint->y+sin(this->_revolveCurrentAngle*PI/180.0f)*this->_revolveR;
    this->setPosition(revolvePoint);
    this->_revolveCurrentAngle+=this->_revolveIterationValue;
    this->_revolveOverflowControl();
}
void StrongestBug::_revolveBackFunction(){
    this->_revolveCurrentAngle+=this->_revolveIterationValue;
    this->_flyStatusTimer++;//revolveBack durumunda iken _flyStatusTimer hiçbir zaman 0 olmamalı, _flyStatusTimer'dan bağımsız olarak çalışmalı

    Vec2 revolvePoint;
    revolvePoint.x=this->_revolveCenterPoint->x+cos(this->_revolveCurrentAngle*PI/180.0f)*this->_revolveR;
    revolvePoint.y=this->_revolveCenterPoint->y+sin(this->_revolveCurrentAngle*PI/180.0f)*this->_revolveR;
    this->setPosition(revolvePoint);

    if(this->_revolveOverflowType==top){
        if(this->getPositionY()<=this->_revolveStartY){
            this->setPositionY(this->_revolveStartY);

            this->_setFlyStatus(run);
            this->_setFlyStatusFunctionPointer();
        }
    }else if(this->_revolveOverflowType==bottom){
         if(this->getPositionY()>=this->_revolveStartY){
            this->setPositionY(this->_revolveStartY);

            this->_setFlyStatus(run);
            this->_setFlyStatusFunctionPointer();
        }
    }else if(this->_revolveOverflowType==left){
        if(this->_revolveIterationValue>0){//this->_revolveIterationValue, _changeRevolveBackParametersRandom içinde -1 ile çarpıldığı için < yerine >
            if(this->getPositionY()>=this->_revolveStartY){
                this->setPositionY(this->_revolveStartY);

                this->_setFlyStatus(run);
                this->_setFlyStatusFunctionPointer();
            }
        }else if(this->_revolveIterationValue<0){ //this->_revolveIterationValue, _changeRevolveBackParametersRandom içinde -1 ile çarpıldığı için > yerine <
            if(this->getPositionY()<=this->_revolveStartY){
                this->setPositionY(this->_revolveStartY);

                this->_setFlyStatus(run);
                this->_setFlyStatusFunctionPointer();
            }
        }
    }else if(this->_revolveOverflowType==right){
        if(this->_revolveIterationValue<0){
            if(this->getPositionY()>=this->_revolveStartY){
                this->setPositionY(this->_revolveStartY);

                this->_setFlyStatus(run);
                this->_setFlyStatusFunctionPointer();
            }
        }else if(this->_revolveIterationValue>0){
                if(this->getPositionY()<=this->_revolveStartY){
                    this->setPositionY(this->_revolveStartY);

                    this->_setFlyStatus(run);
                    this->_setFlyStatusFunctionPointer();
                }
        }
    }
}
void StrongestBug::_setFlyStatus(FlyStatus flyStatus){
    this->_flyStatus=flyStatus;
}
void StrongestBug::_setFlyStatusFunctionPointer(){
    if(this->_flyStatus==run){
        this->_flyStatusFunctionPointer=&StrongestBug::_runFunction;
        this->_changeRunParametersRandom();
    }else if(this->_flyStatus==revolve){
        this->_flyStatusFunctionPointer=&StrongestBug::_revolveFunction;
        this->_changeRevolveParametersRandom();
    }else if(this->_flyStatus==revolveBack){
        this->_flyStatusFunctionPointer=&StrongestBug::_revolveBackFunction;
        this->_changeRevolveBackParameters();
    }
}
void StrongestBug::_changeRevolveBackParameters(){
    this->_revolveIterationValue*=-1;
    this->_flyStatusTimer=100;//revolveBack durumunda iken _flyStatusTimer hiçbir zaman 0 olmamalı, _flyStatusTimer'dan bağımsız olarak çalışmalı
}
void StrongestBug::_revolveOverflowControl(){
    if(this->getPositionY()<this->_boundaryBottom){
        this->_revolveOverflowType=bottom;
        this->_setFlyStatus(revolveBack);
        this->_setFlyStatusFunctionPointer();
        return;
    }else if(this->getPositionY()>this->_boundaryTop){
        this->_revolveOverflowType=top;
        this->_setFlyStatus(revolveBack);
        this->_setFlyStatusFunctionPointer();
        return;
    }

    if(this->getPositionX()<this->_boundaryLeft){
        this->_revolveOverflowType=left;
        this->_setFlyStatus(revolveBack);
        this->_setFlyStatusFunctionPointer();
        return;
    }else if(this->getPositionX()>this->_boundaryRight){
        this->_revolveOverflowType=right;
        this->_flyStatus=revolveBack;
        this->_setFlyStatus(revolveBack);
        this->_setFlyStatusFunctionPointer();
        return;
    }
}
void StrongestBug::catchStart(Web* web){
    Bug::catchStart(web);
    this->_flySprite->setVisible(false);
    this->_flySprite->stopAction(this->_flySpriteSequence);
}
