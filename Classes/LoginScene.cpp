#include "LoginScene.h"
#include "GameDefine.h"
#include "GameScene.h"
#include "MyUtility.h"
#include "Dlog.h"
//ʹ��Ԥ���룬�Ե�ǰƽ̨ΪAndroidʱ�Żᵼ������ͷ�ļ�

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif
USING_NS_CC;

bool LoginScene::isTurn=false;

LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{
}

Scene* LoginScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LoginScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sp=Sprite::create("login.jpg");
	sp->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(sp,1);

	label=Label::createWithBMFont("fonts/Login.fnt",MyUtility::getUTF8Char("lblLogin").c_str());

	label->setScale(0.5f);
	label->setPosition(visibleSize.width/2,visibleSize.height-label->getContentSize().height/2);
	this->addChild(label,2);
	auto ac1 = FadeIn::create(0.5f);
	auto ac2 = FadeOut::create(0.5f);
	label->runAction(RepeatForever::create(Sequence::create(ac1,ac2,nullptr)));


	return true;
}

void LoginScene::JumpToGame(float delta)
{
	if(isTurn)
	{
		Dlog::showLog("now JumpToTheGameScene");
		label->removeFromParent();
		Director::getInstance()->replaceScene(TransitionFade::create(3.0f,GameScene::createScene()));
	}
}


void LoginScene::onEnter()
{
	Layer::onEnter();
	auto listener=EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LoginScene::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(LoginScene::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(LoginScene::onTouchEnded,this);

	auto eventdispatcher=Director::getInstance()->getEventDispatcher();
	eventdispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	//Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(LoginScene::JumpToGame),this, 0,false);
	schedule(SEL_SCHEDULE(&LoginScene::JumpToGame),0);
	
}

void LoginScene::onExit()
{
	Layer::onExit();
	auto eventdispatcher=Director::getInstance()->getEventDispatcher();
	eventdispatcher->removeAllEventListeners();
	this->unschedule(SEL_SCHEDULE(&LoginScene::JumpToGame));
}

bool LoginScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Dlog::showLog("onTouchBegan");

	//��̬����ʾ��2.�в������޷���ֵ------------------------------$$$$$$$--------------------------------

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
	bool re = JniHelper::getMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","createAdView","()V");
	if(re)
	{
		log("call no-static method");
		//�Ǿ�̬�������õ�ʱ����Ҫ���Ƕ��������뾲̬�������õĵ�һ��������ͬ
		minfo.env->CallVoidMethod(jobj,minfo.methodID);
		log("successful");
	}

#endif

	return false;
}

void LoginScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	Dlog::showLog("onTouchMoved");

}

void LoginScene::onTouchEnded(Touch *touch, Event*unused_event)
{
	Dlog::showLog("onTouchEnded");

}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#ifdef __cplusplus

extern "C" {

#endif

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_startGameScene
		(JNIEnv *, jclass) {

			//ʵ�ֵķ��������Լ�����--����Ҳ����ʹ��

			Dlog::showLog("responds = %s","===============>test_JNI_JAVA");

			//auto director = Director::getInstance();
			// create a scene. it's an autorelease object
			//auto scene = GameScene::createScene();

			LoginScene::isTurn = true;
			// run
			//director->replaceScene(scene);

	}

#ifdef __cplusplus

}

#endif

#endif