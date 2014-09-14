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
	virtual void onExit();
	void soundCallback(cocos2d::Ref* pSender);

	E_MOVE_DIRECT m_direct;//�ƶ�����

	void moveAllTile(E_MOVE_DIRECT);//�ƶ����п飬��Ϸ�����߼�
	void newMoveTile();//����һ���¿�
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	CREATE_FUNC(GameScene);
	void backCallback(cocos2d::Ref* pSender);
private:
	cocos2d::LayerColor* colorBack;
	cocos2d::EventListenerTouchOneByOne* m_Event;
	bool m_startMove;//�Ƿ�ʼ�ƶ�
	int m_x,m_y;//������ʼ�ĵ�
	bool m_sound_clear;
	int m_score;//����
	int map[GAME_ROWS][GAME_COLS];//����λ��ש������
	int LastMap[GAME_ROWS][GAME_COLS];//�ϸ�λ�ø�ש������
	cocos2d::Vector<MovedTile *> m_allTile;//�������п�
	cocos2d::Vector<MovedTile *> m_LastAllTile;//�������һ�εĿ飬���ڻ���
};


#endif // GameScene_h__
