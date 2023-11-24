#ifndef __GAMELOADSCENE_H__
#define __GAMELOADSCENE_H__

#include "cocos2d.h"

#define GAME_LOAD_BG_PNG_FILE "backgrounds/menuScene.png"
#define GAME_LOAD_FONT_FILE "fonts/menuFont.fnt"

class GameLoadScene:public cocos2d::Scene{
public:
    GameLoadScene();
    ~GameLoadScene();

    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameLoadScene);

    void onEnter();
    void finishSplash(float dt);
    void setCurrentLevel(int currentLevel);
private:
    cocos2d::Label* _labelLevel;//retain-release
    int _currentLevel;
};
#endif
