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
	void showAt(int r, int c);//在某个位置显示这个块
	void moveTo(int r, int c);//移动到r行 c列
private:

};


#endif // MovedTile_h__
