#include "MovedTile.h"
#include "GameDefine.h"
USING_NS_CC;

MovedTile::MovedTile()
{
}

MovedTile::~MovedTile()
{
}

bool MovedTile::init()
{
	
	if ( !Node::init() )
	{
		return false;
	}
	//������
	auto bk = LayerColor::create(Color4B(255, 0, 156, 64) ,GAME_TILE_WIDTH,GAME_TILE_HEIGHT);
	this->addChild(bk);


	//���ֲ�
	int n = rand()%10;
	this->m_number = n>0 ? 2:4; //nΪ0-9��nΪ0�����ռ10%��4���ֵĸ���Ϊ10%
	auto label = Label::createWithTTF(__String::createWithFormat("%d",m_number)->getCString(),"fonts/arial.ttf",40);
	label->setColor(Color3B::BLACK);
	label->setPosition(GAME_TILE_WIDTH/2,GAME_TILE_HEIGHT/2);
	bk->addChild(label);
	return true;
}

void MovedTile::showAt(int r, int c)
{

}

void MovedTile::moveTo(int r, int c)
{
	m_row = r;
	m_col = c;
	this->setPosition(GAME_TILE_WIDTH*m_col + GAME_TILE_GAP*(m_col+1), 
		GAME_TILE_HEIGHT*m_row + GAME_TILE_GAP*(m_row+1));
}