#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__

#include "cocos2d.h"
#include "GamePlayLayer.h"

class GamePlayScene : public cocos2d::Scene
{
private:
    GamePlayLayer* _gamePlayLayer;//new-delete
    cocos2d::Sprite* _background;//retain-release

public:
    ~GamePlayScene();
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GamePlayScene);
};

#endif // __GAMEPLAY_SCENE_H__
