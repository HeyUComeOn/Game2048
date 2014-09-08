#ifndef SplashScene_h__
#define SplashScene_h__

#include "cocos2d.h"

class Splash :public cocos2d::Layer
{
public:
	Splash();
	~Splash();

	static cocos2d::Scene* createScene();

	virtual bool init();  

	void JumpToGame(float);

	CREATE_FUNC(Splash);
private:

};

#endif // SplashScene_h__
