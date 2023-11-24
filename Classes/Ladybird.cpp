#include "Ladybird.h"

USING_NS_CC;

int Ladybird::_ladybirdPoint=LADYBIRD_POINT;

Ladybird::Ladybird(){}
Ladybird::~Ladybird(){}
void Ladybird::_initVectorRandom(){
    this->_point=LADYBIRD_POINT;

    float x=this->_velocity;
    float randomMultiplierY=(std::rand()%10)*0.1f;
    float y=randomMultiplierY*x;

    int signX=std::rand()%2;
    if(!signX)x*=-1;

    int signY=std::rand()%2;
    if(!signY)y*=-1;

    this->_vector=new Vec2(x,y);
}
void Ladybird::_flyUpdate(){
    this->setPositionX(this->getPositionX()+this->_vector->x);
    this->setPositionY(this->getPositionY()+this->_sinAmplitudeFactor*this->_sinAmplitudeVar*this->_vector->x*sin(this->getPositionX()*PI/this->_sinPeriodFactor)+this->_vector->y);

    this->_overflowXControl();
    this->_overflowYControl();
}
int Ladybird::getPoint(){
    return Ladybird::_ladybirdPoint;
}
