#ifndef GameScene_h__
#define GameScene_h__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
	GameScene();
	~GameScene();

public:

	static cocos2d::Scene* createScene();

	virtual bool init();  

	CREATE_FUNC(GameScene);
private:

};


#endif // GameScene_h__
