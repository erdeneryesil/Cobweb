#include "Moth.h"

USING_NS_CC;

int Moth::_mothPoint=MOTH_POINT;

Moth::Moth(){}
Moth::~Moth(){}
void Moth::_initVectorRandom(){
    this->_point=MOTH_POINT;

    float x=this->_velocity;

    int signX=std::rand()%2;
    if(!signX)x*=-1;

    this->_vector=new Vec2(x,0);//_vector->y kullanılmayacağı için 0
}
bool Moth::initFeebleBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity){
    if(!FeebleBug::initFeebleBug(bugType,pngFile,plistFile,velocity)){
        return false;
    }

    this->_adaptX=this->_boundaryLeft;

    float amp=this->_getSinAmplitude();
    this->_boundarySinBottom=this->_initialY;
    this->_boundarySinTop=this->_initialY+amp;

    return true;
}
float Moth::_generateEntranceYRandom(){
    float amp=this->_getSinAmplitude();
    float randomY=std::rand()%((int)(this->_boundaryTop-this->_boundaryBottom-amp))+this->_boundaryBottom;

    return randomY;
}
void Moth::_flyUpdate(){
    this->setPositionX(this->getPositionX()+this->_vector->x);
    this->setPositionY(this->getPositionY()+this->_sinAmplitudeFactor*this->_sinAmplitudeVar*this->_vector->x*sin((this->getPositionX()-this->_adaptX)*PI/this->_sinPeriodFactor));

    this->_overflowXControl();
    this->_sinusOverflowYControl();
}
void Moth::_sinusOverflowYControl(){
       if(this->getPositionY()<this->_boundarySinBottom){
            this->setPositionY(this->_boundarySinBottom);
        }else if(this->getPositionY()>this->_boundarySinTop){
            this->setPositionY(this->_boundarySinTop);
        }
}
int Moth::getPoint(){
    return Moth::_mothPoint;
}
