#ifndef MovedTile_h__
#define MovedTile_h__

#include "cocos2d.h"

class MovedTile :public cocos2d::Node
{
public:
	MovedTile();
	~MovedTile();

	virtual bool init();  

	CREATE_FUNC(MovedTile);
public:
	int m_row;
	int m_col;
	int m_number;
	void firstInit();
	void showAt(int r, int c);//��ĳ��λ����ʾ�����
	void moveTo(int r, int c);//�ƶ���r�� c��
	void doubleNumber();//����
private:

};


#endif // MovedTile_h__
