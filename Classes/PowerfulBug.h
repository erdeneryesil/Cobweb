#ifndef __POWERFULBUG_H__
#define __POWERFULBUG_H__

#include "Bug.h"

#define DRAGONFLY_PNG_FILE "bugs/spritesheet/dragonfly.png"
#define DRAGONFLY_PLIST_FILE "bugs/spritesheet/dragonfly.plist"
#define BUMBLEBEE_PNG_FILE "bugs/spritesheet/bumblebee.png"
#define BUMBLEBEE_PLIST_FILE "bugs/spritesheet/bumblebee.plist"
#define POWERFULBUG_FLY_SPRITE_FRAME_COUNT 16
#define POWERFULBUG_FLY_SPRITE_DURATION_PER_FRAME 0.05f

class PowerfulBug : public Bug{
protected:
    enum FlyStatus{run=0,hang};
    virtual void _changeRunParametersRandom()=0;
    void _initVectorRandom()override=0;
    void _flyUpdate() override;

    FlyStatus _flyStatus;
    void _initFlyStatusRandom();
    int _flyStatusTimer;

    void _changeHangParametersRandom();
    typedef void(PowerfulBug ::*FunctionPointer)();
    FunctionPointer _flyStatusFunctionPointer; //fly status a göre bu pointer _flyFunc ya da _hangFunc fonksiyonlardan birine işaret edecek
    void _setFlyStatusFunctionPointer();
    void _setFlyStatus(FlyStatus flyStatus);
    void _runFunction();
    void _hangFunction();
    void _hangOverflowControl();

    cocos2d::Sprite* _flySprite;//retain-release
    cocos2d::Sequence* _flySpriteSequence;//retain-release
    virtual void _initFlySprite()=0;
public:
    PowerfulBug ();
    ~PowerfulBug ();
    static PowerfulBug * createPowerfulBug(BugType bugType,float velocity);
    bool initPowerfulBug(BugType bugType,std::string pngFile,std::string plistFile,float velocity);
    void disableMighty();//Bonus kazanıldığında, güçsüz hale getirir, _powerParticle kapatılır
    int getPoint()override=0;
    void catchStart(Web* web)override;
};

#endif
