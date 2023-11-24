#include "Boundries.h"

USING_NS_CC;

Size Boundries::_frameSize;
Size Boundries::_designSizePratic;
Size Boundries::_designSizeTheoric;

Vec2 Boundries::_pointBottomLeft;
Vec2 Boundries::_pointBottomRight;
Vec2 Boundries::_pointTopLeft;
Vec2 Boundries::_pointTopRight;
Vec2 Boundries::_pointBridgeLeft;
Vec2 Boundries::_pointBridgeRight;
float Boundries::_pointSpiderRightX;
float Boundries::_pointSpiderLeftX;

float Boundries::_pointBridgeY;
float Boundries::_pointTop;
float Boundries::_pointBottom;
float Boundries::_pointLeft;
float Boundries::_pointRight;

const Vec2& Boundries::getPointBottomLeft(){
    return Boundries::_pointBottomLeft;
}
const Vec2& Boundries::getPointBottomRight(){
    return Boundries::_pointBottomRight;
}
const Vec2& Boundries::getPointTopLeft(){
    return Boundries::_pointTopLeft;
}
const Vec2& Boundries::getPointTopRight(){
    return Boundries::_pointTopRight;
}
const Vec2& Boundries::getPointBridgeLeft(){
    return Boundries::_pointBridgeLeft;
}
const Vec2& Boundries::getPointBridgeRight(){
    return Boundries::_pointBridgeRight;
}
const float Boundries::getPointSpiderRightX(){
    return Boundries::_pointSpiderRightX;
}
const float Boundries::getPointSpiderLeftX(){
    return Boundries::_pointSpiderLeftX;
}
const float Boundries::getPointBridgeY(){
    return Boundries::_pointBridgeY;
}
const float Boundries::getPointTop(){
    return Boundries::_pointTop;
}
const float Boundries::getPointBottom(){
    return Boundries::_pointBottom;
}
const float Boundries::getPointLeft(){
    return Boundries::_pointLeft;
}
const float Boundries::getPointRight(){
    return Boundries::_pointRight;
}
const float Boundries::getTheoricWidth(){
    return Boundries::_designSizeTheoric.width;
}
const float Boundries::getTheoricHeight(){
    return Boundries::_designSizeTheoric.height;
}

void Boundries::init(const Size& frameSize,const Size& designSizePratic,const Size& designSizeTheoric){
    Boundries::_frameSize=Size(frameSize);
    Boundries::_designSizePratic=Size(designSizePratic);
    Boundries::_designSizeTheoric=Size(designSizeTheoric);

    float x,y;

    x=(Boundries::_designSizePratic.width-Boundries::_designSizeTheoric.width)/2;
    if(x<0.0f)x=0.0f;
    y=0.0f;
    Boundries::_pointBottomLeft=Vec2(x,y);
    Boundries::_pointBottom=y;
    Boundries::_pointLeft=x;

    x=(Boundries::_designSizePratic.width-Boundries::_designSizeTheoric.width)/2;
    if(x<0.0f)x=0.0f;
    x+=MIN(Boundries::_designSizePratic.width,Boundries::_designSizeTheoric.width);
    y=0;
    Boundries::_pointBottomRight=Vec2(x,y);
    Boundries::_pointRight=x;

    x=(Boundries::_designSizePratic.width-Boundries::_designSizeTheoric.width)/2;
    if(x<0.0f)x=0.0f;
    y=Boundries::_designSizeTheoric.height;
    Boundries::_pointTopLeft=Vec2(x,y);
    Boundries::_pointTop=y;

    x=(Boundries::_designSizePratic.width-Boundries::_designSizeTheoric.width)/2;
    if(x<0.0f)x=0.0f;
    x+=MIN(Boundries::_designSizePratic.width,Boundries::_designSizeTheoric.width);
    y=Boundries::_designSizeTheoric.height;
    Boundries::_pointTopRight=Vec2(x,y);

    x=(Boundries::_designSizePratic.width-Boundries::_designSizeTheoric.width)/2;
    if(x<0.0f)x=0.0f;
    y=Boundries::_designSizeTheoric.height-(Boundries::_designSizeTheoric.height*BRIDGE_Y_RATE);
    Boundries::_pointBridgeY=y;
    Boundries::_pointBridgeLeft=Vec2(x,Boundries::_pointBridgeY);


    x=(Boundries::_designSizePratic.width-Boundries::_designSizeTheoric.width)/2;
    if(x<0.0f)x=0.0f;
    x+=MIN(Boundries::_designSizePratic.width,Boundries::_designSizeTheoric.width);
    Boundries::_pointBridgeRight=Vec2(x,Boundries::_pointBridgeY);

    x=(Boundries::_designSizePratic.width-Boundries::_designSizeTheoric.width)/2;
    if(x<0.0f)x=0.0f;
    x+=MIN(Boundries::_designSizePratic.width,Boundries::_designSizeTheoric.width);
    x-=MIN(Boundries::_designSizePratic.width,Boundries::_designSizeTheoric.width)*0.25f;//kenardan %25 oranında içeride
    Boundries::_pointSpiderRightX=x;

    x=(Boundries::_designSizePratic.width-Boundries::_designSizeTheoric.width)/2;
    if(x<0.0f)x=0.0f;
    x+=MIN(Boundries::_designSizePratic.width,Boundries::_designSizeTheoric.width)*0.25f;//kenardan %25 oranında içeride
    Boundries::_pointSpiderLeftX=x;
}
