#ifndef GameWin_h__
#define GameWin_h__

#include "cocos2d.h"

class  GameWin:public cocos2d::Layer
{
public:
	 GameWin();
	~ GameWin();
public:
	static cocos2d::Scene* createScene();

	virtual bool init();  
	virtual void onEnter();
	virtual void onExit();
	CREATE_FUNC(GameWin);
	void EndCallback(float delta);
private:

};


#endif // GameWin_h__
