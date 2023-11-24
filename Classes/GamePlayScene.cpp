#include "GamePlayScene.h"

USING_NS_CC;

GamePlayScene::~GamePlayScene(){
    this->removeChild(this->_gamePlayLayer);
    delete this->_gamePlayLayer;

    this->_background->release();
}

Scene* GamePlayScene::createScene()
{
    return GamePlayScene::create();
}
bool GamePlayScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    this->_background=Sprite::create("backgrounds/gameScene.png");
    this->_background->retain();
    this->_background->setPosition(visibleSize/2);
    this->addChild(this->_background,-1);

    this->_gamePlayLayer=new GamePlayLayer();//delete ile yok et
    this->addChild(this->_gamePlayLayer,0);

    return true;
}
