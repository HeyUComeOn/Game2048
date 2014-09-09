#ifndef GameScene_h__
#define GameScene_h__

#include "cocos2d.h"
#include "GameDefine.h"
#include "MovedTile.h"
class GameScene : public cocos2d::Layer
{
public:
	GameScene();
	~GameScene();

public:

	static cocos2d::Scene* createScene();

	virtual bool init();  

	E_MOVE_DIRECT m_direct;//�ƶ�����
	int map[GAME_ROWS][GAME_COLS];
	cocos2d::Vector<MovedTile *> m_allTile;//�������п�
	void moveAllTile(E_MOVE_DIRECT);//�ƶ����п飬��Ϸ�����߼�
	void newMoveTile();//����һ���¿�
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	CREATE_FUNC(GameScene);
private:
	cocos2d::LayerColor* colorBack;
	bool m_startMove;//�Ƿ�ʼ�ƶ�
	int m_x,m_y;//������ʼ�ĵ�
};


#endif // GameScene_h__
