#include "GameDefine.h"
#include "GameOver.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

GameOver::GameOver()
{
}

GameOver::~GameOver()
{
}

Scene* GameOver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	//播放音乐
	if (!UserDefault::getInstance()->getBoolForKey("loseMusicOff"))
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("musics/lose.ogg");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("musics/lose.wav");
#endif
	}

	//显示游戏结束
	auto labelOver = Label::createWithBMFont("fonts/futura-48.fnt","Game Over");
	labelOver->setPosition(GAME_SCREEN_WIDTH/2,GAME_SCREEN_HEIGHT/2);
	labelOver->setScale(1.2f);
	addChild(labelOver);

	//重新开始
	auto labelRe = Label::createWithBMFont("fonts/futura-48.fnt","Restart");
	
	auto labelItem = MenuItemLabel::create(labelRe,
		CC_CALLBACK_1(GameOver::menuCallBack,this));
	labelItem->setPosition(GAME_SCREEN_WIDTH/2,GAME_SCREEN_HEIGHT/4);
	auto menu = Menu::create(labelItem, NULL);
	menu->setPosition(Point::ZERO);
	addChild(menu);
	return true;
}

void GameOver::menuCallBack(cocos2d::Ref* ref)
{
	auto sc = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.5f,sc));
}