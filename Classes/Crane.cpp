#include "Crane.h"

USING_NS_CC;

int Crane::_cranePoint=CRANE_POINT;

Crane::Crane(){}
Crane::~Crane(){}
void Crane::_initVectorRandom(){
    this->_point=CRANE_POINT;

    float randomXInt=std::rand()%(int)(this->_velocity-1)+1;
    float randomXDecimal=(float)(std::rand()%9999)/10000;
    float randomX=randomXInt+randomXDecimal;

    float y=sqrt(pow(this->_velocity,2)-pow(randomX,2));

    int signX=std::rand()%2;
    if(!signX)randomX*=-1;

    int signY=std::rand()%2;
    if(!signY)y*=-1;

    this->_vector=new Vec2(randomX,y);
}
int Crane::getPoint(){
    return Crane::_cranePoint;
}
