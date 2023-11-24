#ifndef __ROBBERFLY_H__
#define __ROBBERFLY_H__

#include "StrongestBug.h"

#define ROBBERFLY_POINT 7
#define ROBBERFLY_FLY_SPRITE_PNG_FILE "bugs/spritesheet/robberflyFlySprite.png"
#define ROBBERFLY_FLY_SPRITE_PLIST_FILE "bugs/spritesheet/robberflyFlySprite.plist"
#define ROBBERFLY_FLY_SPRITE_POS_H_RATE 2.1f
#define ROBBERFLY_FLY_SPRITE_POS_V_RATE 2.0f
class Robberfly: public StrongestBug{

private:
    void _initVectorRandom() override;
    void _changeRunParametersRandom()override;
    void _changeRevolveParametersRandom()override;
    static int _robberflyPoint;
    void _initFlySprite()override;
public:
    Robberfly();
    ~Robberfly();
    int getPoint()override;
};

#endif
