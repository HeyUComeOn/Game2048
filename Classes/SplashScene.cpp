#include "SplashScene.h"
#include "GameDefine.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
Splash::Splash()
{
}

Splash::~Splash()
{
}

Scene* Splash::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Splash::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Splash::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//游戏名
	auto labelGame = Label::createWithBMFont("fonts/futura-48.fnt","2048");
	labelGame->setPosition(GAME_SCREEN_WIDTH/2 ,GAME_SCREEN_HEIGHT/2);
	labelGame->setScale(1.5f);
	addChild(labelGame);

	//作者名
	auto labelGameAut = Label::createWithBMFont("fonts/futura-48.fnt","By mitsui 2014/9");
	labelGameAut->setPosition(GAME_SCREEN_WIDTH/2 ,GAME_SCREEN_HEIGHT/4);
	labelGameAut->setScale(0.8f);
	addChild(labelGameAut);

	//计划任务3s后跳转到下一个场景
	this->scheduleOnce(schedule_selector(Splash::JumpToGame),/*3.0*/0.5f);
	return true;
}

void Splash::JumpToGame(float delta)
{
	//加载音效
	SimpleAudioEngine::getInstance()->preloadEffect("musics/move.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("musics/clear.wav");
	
	//加载结束音乐
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("musics/lose.ogg");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("musics/lose.wav");
#endif
	
	Director::getInstance()->replaceScene(TransitionProgressRadialCW::create(3.0f,GameScene::createScene()));
}