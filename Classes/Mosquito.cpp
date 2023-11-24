#include "Mosquito.h"

USING_NS_CC;

int Mosquito::_mosquitoPoint=MOSQUITO_POINT;

Mosquito::Mosquito(){}
Mosquito::~Mosquito(){}
void Mosquito::_initVectorRandom(){
    this->_point=MOSQUITO_POINT;

    float x=this->_velocity;
    float y=0;

    int signX=std::rand()%2;
    if(!signX)x*=-1;

    this->_vector=new Vec2(x,y);
}
int Mosquito::getPoint(){
    return Mosquito::_mosquitoPoint;
}
