#ifndef __DRAGONFLY_H__
#define __DRAGONFLY_H__

#include "PowerfulBug.h"

#define DRAGONFLY_POINT 4
#define DRAGONFLY_FLY_SPRITE_PNG_FILE "bugs/spritesheet/dragonflyFlySprite.png"
#define DRAGONFLY_FLY_SPRITE_PLIST_FILE "bugs/spritesheet/dragonflyFlySprite.plist"
#define DRAGONFLY_FLY_SPRITE_POS_H_RATE 1.9f
#define DRAGONFLY_FLY_SPRITE_POS_V_RATE 2.0f
class Dragonfly : public PowerfulBug{
private:
    void _initVectorRandom()override;
    void _changeRunParametersRandom()override;
    static int _dragonflyPoint;
    void _initFlySprite()override;

public:
    Dragonfly();
    ~Dragonfly();
    float getInitialY();
    int getPoint()override;
};

#endif
