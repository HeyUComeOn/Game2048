#include "GameWin.h"
#include "SimpleAudioEngine.h"
#include "GameDefine.h"
#include "GameScene.h"
USING_NS_CC;
using namespace CocosDenshion;

GameWin::GameWin()
{

}

GameWin::~GameWin()
{

}

Scene* GameWin::createScene()
{
	auto scene = Scene::create();

	auto layer = GameWin::create();

	scene->addChild(layer);

	return scene;
}


bool GameWin::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	//播放庆祝音乐
	if (!UserDefault::getInstance()->getBoolForKey("loseMusicOff"))
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("musics/win.ogg");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("musics/win.wav");
#endif
	}
	return true;
}

void GameWin::onEnter()
{
	Layer::onEnter();
	//todo
	//创建背景网格
	auto backWidth = GAME_TILE_WIDTH*GAME_COLS+GAME_TILE_GAP*(GAME_COLS+1);
	auto backHeight = GAME_TILE_HEIGHT*GAME_ROWS+GAME_TILE_GAP*(GAME_ROWS+1);
	LayerColor*colorBack = LayerColor::create(Color4B(2, 67,142,219),backWidth,backHeight);
	colorBack->setPosition(GAME_SCREEN_WIDTH/2 - backWidth/2,GAME_SCREEN_HEIGHT/2-backHeight/2) ;
	auto ac = TintBy::create(0.5f,-127, -255, -127);
	colorBack->runAction(RepeatForever::create(ac));
	addChild(colorBack);

	int score = UserDefault::getInstance()->getIntegerForKey("Score");
	int H_score = UserDefault::getInstance()->getIntegerForKey("HighestScore",0);
	int newH_score = H_score>score?H_score:score;
	UserDefault::getInstance()->setIntegerForKey("HighestScore", newH_score);
	
	//普通分数标签
	auto labelScore = Label::createWithTTF(__String::createWithFormat("the Score: %d", score)->getCString(), "fonts/arial.ttf",35);
	labelScore->setPosition(backWidth/2,backHeight-labelScore->getContentSize().height);
	colorBack->addChild(labelScore);

	//历史最高分标签
	auto labelHScore = Label::createWithTTF(__String::createWithFormat("the Best: %d", newH_score)->getCString(), "fonts/arial.ttf",35);
	labelHScore->setPosition(backWidth/2,backHeight/2+labelHScore->getContentSize().height/2);
	colorBack->addChild(labelHScore);

	//播放跳舞动画
	auto sp=Sprite::createWithSpriteFrameName("grossini_dance_01.png");
	sp->setPosition(backWidth/2, sp->getContentSize().height/2);
	auto animation = AnimationCache::getInstance()->getAnimation("Dance");
	colorBack->addChild(sp);
	sp->runAction(Animate::create(animation));

	this->scheduleOnce(schedule_selector(GameWin::EndCallback),30.0f);

}

void GameWin::onExit()
{
	Layer::onExit();
	//Director::getInstance()->getEventDispatcher()->removeEventListener(/**/);
}

void GameWin::EndCallback(float delta)
{
	Director::getInstance()->replaceScene(GameScene::createScene());
}