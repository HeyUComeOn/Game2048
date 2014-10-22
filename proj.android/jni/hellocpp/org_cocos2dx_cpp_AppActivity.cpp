#include "org_cocos2dx_cpp_AppActivity.h"
#include "cocos2d.h"//一定要引入这个，有cocos2d的宏

#include "GameScene.h"
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include <jni.h>

using namespace std;

using namespace cocos2d;

#ifdef __cplusplus

extern "C" {

#endif

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_startGame
  (JNIEnv *, jclass) {

		//实现的方法下面自己发挥--参数也可以使用

		log("responds = %s","===============>test_JNI_JAVA");

		//auto director = Director::getInstance();
		// create a scene. it's an autorelease object
		//auto scene = GameScene::createScene();


		// run
		//director->replaceScene(scene);

	}

#ifdef __cplusplus

}

#endif

//#endif
