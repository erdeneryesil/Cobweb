#ifndef __GADFLY_H__
#define __GADFLY_H__

#include "StrongestBug.h"

#define GADFLY_POINT 5
#define GADFLY_MAX_LAP_CAOUNT 2
#define GADFLY_FLY_SPRITE_PNG_FILE "bugs/spritesheet/gadflyFlySprite.png"
#define GADFLY_FLY_SPRITE_PLIST_FILE "bugs/spritesheet/gadflyFlySprite.plist"
#define GADFLY_FLY_SPRITE_POS_H_RATE 2.0f
#define GADFLY_FLY_SPRITE_POS_V_RATE 2.0f

class Gadfly : public StrongestBug{
private:
    void _initVectorRandom() override;
    void _changeRunParametersRandom()override;
    void _changeRevolveParametersRandom()override;
    int _lapCount;//rasgele kaç tur dönecek
    static int _gadflyPoint;
    void _initFlySprite()override;
public:
    Gadfly();
    ~Gadfly();
    float getInitialY();
    int getPoint()override;
};

#endif
