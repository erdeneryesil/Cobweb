#ifndef __GAMESURE_SCENE_H__
#define __GAMESURE_SCENE_H__

#include "cocos2d.h"

#define GAME_SURE_FONT_FILE "fonts/menuFont.fnt"
#define GAME_SURE_BACKGROUND_PNG_FILE "backgrounds/menuScene.png"

class GameSureScene : public cocos2d::Scene
{
private:
    enum SureItem{yes=0,no};
    cocos2d::Sprite* _background;//retain-release
    void menuItemClick(cocos2d::Ref* sender);
    int _currentLevel;

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameSureScene);
    ~GameSureScene();
};

#endif // __GAMEMENU_SCENE_H__
