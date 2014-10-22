#include "GameDefine.h"
#include "GameOver.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Dlog.h"
//ʹ��Ԥ���룬�Ե�ǰƽ̨ΪAndroidʱ�Żᵼ������ͷ�ļ�

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif
using namespace CocosDenshion;
USING_NS_CC;

bool GameOver::isTurn=false;

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

	//��������
	if (!UserDefault::getInstance()->getBoolForKey("loseMusicOff"))
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("musics/lose.ogg");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("musics/lose.wav");
#endif
	}

	//��ʾ��Ϸ����
	auto labelOver = Label::createWithBMFont("fonts/futura-48.fnt","Game Over");
	labelOver->setPosition(GAME_SCREEN_WIDTH/2,GAME_SCREEN_HEIGHT/2);
	labelOver->setScale(1.2f);
	addChild(labelOver);

	//���¿�ʼ
	auto labelRe = Label::createWithBMFont("fonts/futura-48.fnt","Restart");
	
	auto labelItem = MenuItemLabel::create(labelRe,
		CC_CALLBACK_1(GameOver::menuCallBack,this));
	labelItem->setPosition(GAME_SCREEN_WIDTH/2,GAME_SCREEN_HEIGHT/4);
	auto menu = Menu::create(labelItem, nullptr);
	menu->setPosition(Point::ZERO);
	addChild(menu);

	schedule(SEL_SCHEDULE(&GameOver::restartGame),0);
	return true;
}

void GameOver::menuCallBack(cocos2d::Ref* ref)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //�жϵ�ǰ�Ƿ�ΪAndroidƽ̨
	JniMethodInfo minfo;//����Jni������Ϣ�ṹ��
	//getStaticMethodInfo �κ�������һ��boolֵ��ʾ�Ƿ��ҵ��˺���
	bool isHave = JniHelper::getStaticMethodInfo(minfo,
		"org/cocos2dx/cpp/AppActivity", "getObj","()Ljava/lang/Object;");

	//�Ȼ����Ķ���Ȼ�����������ȥ�������ķǾ�̬����
	jobject jobj;

	if (!isHave) {
		log("jni:jobj wasn't got");

	}else{
		log("jni:jobj was got");
		//���ô˺���
		//minfo.env->CallVoidMethod(jobj,minfo.methodID);
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID,minfo.methodID);
	}

	//getMethodInfo�ж�java�������Ǿ�̬�����Ƿ���ڣ�����bool
	bool re = JniHelper::getMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","createPayView","()V");
	if(re)
	{
		log("call no-static method");
		//�Ǿ�̬�������õ�ʱ����Ҫ���Ƕ��������뾲̬�������õĵ�һ��������ͬ
		minfo.env->CallVoidMethod(jobj,minfo.methodID);
		log("successful");
	}

#endif

}

void GameOver::restartGame(float delta)
{
	if(isTurn)
	{
		Dlog::showLog("now Restart Game");
		
		auto sc = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(1.5f, sc));
		isTurn = false;
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#ifdef __cplusplus

extern "C" {

#endif

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_ReStartGame
		(JNIEnv *, jclass) {

			//ʵ�ֵķ��������Լ�����--����Ҳ����ʹ��

			Dlog::showLog("responds = %s","===============>test_JNI_JAVA");

			//auto director = Director::getInstance();
			// create a scene. it's an autorelease object
			//auto scene = GameScene::createScene();

			GameOver::isTurn = true;
			// run
			//director->replaceScene(scene);

	}

#ifdef __cplusplus

}

#endif

#endif