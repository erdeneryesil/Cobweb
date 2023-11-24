#ifndef __BUG_H__
#define __BUG_H__

#include "cocos2d.h"

#define BUG_FLY_INTERVAL_NUMERATOR 1.0f //flyInterval pay
#define BUG_FLY_INTERVAL_DENOMINATOR 60.0f//flyInterval payda
#define BUG_FLY_FRAME_COUNT 12
#define BUG_DEATH_FRAME_COUNT 8
#define BUG_CATCH_FRAME_COUNT 6
#define BUG_FLY_DURATION_PER_FRAME 0.05f
#define BUG_DEATH_DURATION 1.0
#define BUG_CATCH_DURATION_PER_FRAME 0.01f
#define BUG_CATCH_ROTATE_DURATION 0.1f
#define BUG_EXPLOSION_SPRITE_PNG_FILE "bugs/spritesheet/explosionSprite.png"
#define BUG_EXPLOSION_SPRITE_PLIST_FILE "bugs/spritesheet/explosionSprite.plist"
#define BUG_EXPLOSION_SPRITE_FRAME_COUNT 10
#define BUG_EXPLOSION_SPRITE_DURATION_PER_FRAME 0.04f
class Web;

class Bug : public cocos2d::Sprite
{
public:
    enum BugType{mosquito=0,crane,moth,ladybird,dragonfly,bumblebee,gadfly,robberfly,bonusbug};
    enum Status{idle=0,triggerFly,fly,triggerCatch,catched,triggerDying,dying,dead};
    Bug();
    ~Bug();
    bool initBug(std::string pngFile,std::string plistFile,float velocity);

    BugType getBugType();
    void flyStart();
    virtual void catchStart(Web* web);//BonusBug, StrongestBug, PowerfulBug içinde override edildi
    void death();

    Status getStatus();
    bool isMighty();
    virtual int getPoint()=0;

protected:
    void update(float dt);//sadece actionlarını kontrol etmekte kullanılacak

    bool _mighty;//güç; eğer true ise, ağ örüldüğü anda ağa temas ederse ağı parçalar, false ise hiçbir etkisi olmaz
    BugType _bugType;
    float _boundaryTop;
    float _boundaryBottom;
    float _boundaryLeft;
    float _boundaryRight;
    bool _overflowTop;
    bool _overflowBottom;
    bool _overflowLeft;
    bool _overflowRight;
    int _overflowCounterTop;
    int _overflowCounterBottom;
    int _overflowCounterLeft;
    int _overflowCounterRight;
    int _overflowCounterMax;
    void _overflowXControl();
    void _overflowYControl();
    cocos2d::Vec2* _vector;
    Status _status;
    int _point;
    Web* _web;//silinmeyecek asla, Bug ı yakalayan Web nesnesinin referansı

    float _velocity;
    float _initialX;//başlangıç x
    float _initialY;//başlangıç y
    void _initFlip();
    void _initEntrance();
    virtual float _generateEntranceYRandom();

    int _flyFrameCount;
    int _deathFrameCount;
    int _catchFrameCount;
    float _flyDurationPerFrame;
    float _deathDuration;
    float _catchDurationPerFrame;
    float _catchRotateDuration;
    cocos2d::Animate* _flyAnimate;      //retain-release
    cocos2d::Sequence* _deathSequence;  //retain-release
    void _finishedDeathSettings();
    cocos2d::Spawn* _catchSpawn;        //retain-release

    void _startCatchSettings();

    void _initActions(std::string pngFile,std::string plistFile);


    cocos2d::Sprite* _explosionSprite;      //retain-release
    cocos2d::Sequence* _explosionSequence;  //retain-release
    void _initExplosionSprite(std::string pngFile,std::string plistFile);

    float _flyInterval;
    virtual void _flyUpdate()=0;    //virtual çünkü her sineğin fly türü farklı
    virtual void _initVectorRandom()=0; //virtual çünkü her sineğin direction türü farklı;
};

#endif
