#ifndef GameOver_h__
#define GameOver_h__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
	GameOver();
	~GameOver();

public:

	static cocos2d::Scene* createScene();

	virtual bool init();  

	CREATE_FUNC(GameOver);
	void menuCallBack(cocos2d::Ref* ref);

	void restartGame(float);

	static bool isTurn;

private:

};
#endif // GameOver_h__
