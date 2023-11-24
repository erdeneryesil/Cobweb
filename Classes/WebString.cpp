#include "WebString.h"
USING_NS_CC;

WebString::WebString(){}
WebString::~WebString(){

}
WebString* WebString::createWebString(){
    WebString* webString=new WebString();
    if(webString && webString->initWebString()){
        webString->autorelease();
        return webString;
    }
    CC_SAFE_DELETE(webString);
    return NULL;
}
bool WebString::initWebString(){
    this->initWithFile(WEBSTRING_PNG_FILE);

    this->_minScaleRate=MIN_SCALE_RATE_WEBSTRING;
    this->_originalHeight=this->getContentSize().height;
    this->setScaleY(this->_minScaleRate);
    this->setAnchorPoint(Vec2(0.5f,0.0f));

    this->_initScatAction();
    this->_initShortenAction();

    return true;
}

void WebString::_initScatAction(){
    this->_scatDuration=SCAT_DURATION;
}

void WebString::scat(float x,float yBottom, float yTop){
    this->setScaleY(this->_minScaleRate);
    this->setPosition(Vec2(x,yBottom));

    float finalScaleRate=(yTop-yBottom)/this->_originalHeight;//kısa halden, uzun hale geldiğinde, uzun halinin sahip olduğu uzunluğun oranı

    CallFunc* callbackFunction=CallFunc::create(CC_CALLBACK_0(WebString::_finishedScatAction,this));
    ScaleTo* scale=ScaleTo::create(this->_scatDuration,1,finalScaleRate);
    Sequence* sequence=Sequence::create(scale,callbackFunction,nullptr);

    this->stopAllActions();
    this->runAction(sequence);
}
void WebString::_finishedScatAction(){}

void WebString::_initShortenAction(){
    this->_shortenDuration=SHORTEN_DURATION;
}
void WebString::shorten(){
    CallFunc* callbackFunction=CallFunc::create(CC_CALLBACK_0(WebString::_finishedShortenAction,this));
    MoveBy* moveBy=MoveBy::create(this->_shortenDuration,Vec2(0,this->getBoundingBox().size.height));
    ScaleTo* scale=ScaleTo::create(this->_shortenDuration,1,this->_minScaleRate);
    Spawn* spawn=Spawn::create(moveBy,scale,nullptr);
    Sequence* sequence=Sequence::create(spawn,callbackFunction,nullptr);

    this->stopAllActions();
    this->runAction(sequence);
}
void WebString::_finishedShortenAction(){}


