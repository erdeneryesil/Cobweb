#ifndef __SIZES_H__
#define __SIZES_H__

#include "cocos2d.h"

#define  BRIDGE_Y_RATE 0.148 //örümceğin durağaı yüksekliğin, toplam yüksekliğe oranı. 0.1521 ile 0.1478 arasında değişiyor çoğu 0.148

class Boundries{
public:
    static void init(const cocos2d::Size& frameSize,const cocos2d::Size& designSizePratic,const cocos2d::Size& designSizeTheoric);

    static const cocos2d::Vec2& getPointBottomLeft();
    static const cocos2d::Vec2& getPointBottomRight();
    static const cocos2d::Vec2& getPointTopLeft();
    static const cocos2d::Vec2& getPointTopRight();
    static const cocos2d::Vec2& getPointBridgeLeft();
    static const cocos2d::Vec2& getPointBridgeRight();
    static const float getPointSpiderRightX();
    static const float getPointSpiderLeftX();
    static const float getPointBridgeY();
    static const float getPointTop();
    static const float getPointBottom();
    static const float getPointLeft();
    static const float getPointRight();
    static const float getTheoricWidth();
    static const float getTheoricHeight();
private:
    static cocos2d::Size _designSizePratic;
    static cocos2d::Size _designSizeTheoric;
    static cocos2d::Size _frameSize;

    static cocos2d::Vec2 _pointBottomLeft;
    static cocos2d::Vec2 _pointBottomRight;
    static cocos2d::Vec2 _pointTopLeft;
    static cocos2d::Vec2 _pointTopRight;
    static cocos2d::Vec2 _pointBridgeLeft;
    static cocos2d::Vec2 _pointBridgeRight;
    static float _pointSpiderRightX;
    static float _pointSpiderLeftX;

    static float _pointBridgeY;
    static float _pointTop;
    static float _pointBottom;
    static float _pointLeft;
    static float _pointRight;
};

#endif
