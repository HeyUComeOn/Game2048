#ifndef LoginScene_h__
#define LoginScene_h__

#include "cocos2d.h"

class LoginScene :public cocos2d::Layer
{
public:
	LoginScene();
	~LoginScene();

	static cocos2d::Scene* createScene();

	virtual bool init();  

	void JumpToGame(float);

	void onEnter();

	void onExit();

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event*unused_event);

	CREATE_FUNC(LoginScene);

	static bool isTurn;

private:
	cocos2d::Label*label;
};
#endif // LoginScene_h__
