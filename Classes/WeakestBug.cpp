#include "WeakestBug.h"
#include "Mosquito.h"
#include "Crane.h"

USING_NS_CC;

WeakestBug::WeakestBug(){}
WeakestBug::~WeakestBug(){}
WeakestBug* WeakestBug::createWeakestBug(BugType bugType,float velocity){
    WeakestBug* weakestBug;
    std::string pngFile;
    std::string plistFile;

    if(bugType==mosquito){
        weakestBug=new Mosquito();
        pngFile=MOSQUITO_PNG_FILE;
        plistFile=MOSQUITO_PLIST_FILE;
    }else if(bugType==crane){
        weakestBug=new Crane();
        pngFile=CRANE_PNG_FILE;
        plistFile=CRANE_PLIST_FILE;
    }

    if(weakestBug && weakestBug->initWeakestBug(bugType,pngFile,plistFile,velocity)){
        weakestBug->autorelease();
        return weakestBug;
    }
    CC_SAFE_DELETE(weakestBug);

    return NULL;
}
bool WeakestBug::initWeakestBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity){
    if(!this->initBug(pngFile,plistFile,velocity))return false;

    this->_bugType=bugType;
    this->_mighty=false;
    this->_initVectorRandom();//this->_vector...
    this->_initFlip();//hareket edeceği yöne göre döndür
    this->_initEntrance();

    return true;
}
void WeakestBug::_flyUpdate(){
    this->setPosition(this->getPosition()+*(this->_vector));

    this->_overflowXControl();
    this->_overflowYControl();
}
