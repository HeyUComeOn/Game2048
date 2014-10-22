#include "AppDelegate.h"
#include "SplashScene.h"
#include "GameDefine.h"
/*//使用预编译，对当前平台为Android时才会导入如下头文件

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif*/

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
		glview->setFrameSize(GAME_SCREEN_WIDTH,GAME_SCREEN_HEIGHT);
    }

	glview->setDesignResolutionSize(320,480,ResolutionPolicy::EXACT_FIT);
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    //void doSdkLogin(boolean isLandScape, boolean isBgTransparent)

    //静态函数示例2.有参数，无返回值------------------------------$$$$$$$--------------------------------
/*
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台
        JniMethodInfo minfo;//定义Jni函数信息结构体
        //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
        bool isHave = JniHelper::getStaticMethodInfo(minfo,
                                                    "org/cocos2dx/cpp/AppActivity", "getObj","()Ljava/lang/Object;");
		
		//先获得类的对象，然后用这个对象去调用它的非静态函数
		jobject jobj;

        if (!isHave) {
            log("jni:此函数不存在");

        }else{
            log("jni:此函数存在");
            //调用此函数
			//minfo.env->CallVoidMethod(jobj,minfo.methodID);
            jobj = minfo.env->CallStaticObjectMethod(minfo.classID,minfo.methodID);
        }

		//getMethodInfo判断java定义的类非静态函数是否存在，返回bool
		bool re = JniHelper::getMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","login","()V");
		if(re)
		{
			log("call no-static method");
			//非静态函数调用的时候，需要的是对象，所以与静态函数调用的第一个参数不同
			minfo.env->CallVoidMethod(jobj,minfo.methodID);
			log("successful");
		}

    #endif
*/

	auto scene = Splash::createScene();

	// run
	director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
