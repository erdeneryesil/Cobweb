#ifndef __GAMEOVERSCENE_H__
#define __GAMEOVERSCENE_H__

#include "cocos2d.h"

#define GAME_OVER_BG_PNG_FILE "backgrounds/menuScene.png"
#define GAME_OVER_FONT_FILE "fonts/menuFont.fnt"

class GameOverScene:public cocos2d::Scene{
public:
    GameOverScene();
    ~GameOverScene();

    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameOverScene);

    void onEnter();
    void finishSplash(float dt);
    void setScene(int score,bool gameOver=true);
private:
    cocos2d::Label* _labelResult;//retain-release
    cocos2d::Label* _labelScoreTitle;//retain-release
    cocos2d::Label* _labelScore;//retain-release
};
#endif
