#ifndef __BUMBLEBEE_H__
#define __BUMBLEBEE_H__

#include "PowerfulBug.h"

#define BUMBLEBEE_POINT 6
#define BUMBLEBEE_FLY_SPRITE_PNG_FILE "bugs/spritesheet/bumblebeeFlySprite.png"
#define BUMBLEBEE_FLY_SPRITE_PLIST_FILE "bugs/spritesheet/bumblebeeFlySprite.plist"
#define BUMBLEBEE_FLY_SPRITE_POS_H_RATE 1.8f
#define BUMBLEBEE_FLY_SPRITE_POS_V_RATE 2.0f
class Bumblebee : public PowerfulBug{
private:
    cocos2d::Size _visibleSize;
    void _initVectorRandom()override;
    void _changeRunParametersRandom()override;
    static int _bumblebeePoint;
    void _initFlySprite()override;

public:
    Bumblebee();
    ~Bumblebee();
    int getPoint()override;
};

#endif
