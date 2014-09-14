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
	//背景层
	auto bk = LayerColor::create(Color4B(255, 0, 156, 200) ,GAME_TILE_WIDTH,GAME_TILE_HEIGHT);
	this->addChild(bk);
	bk->setTag(110);

/*
	//数字层
	int n = rand()%10;
	this->m_number = n>0 ? 2:4; //n为0-9，n为0的情况占10%，4出现的概率为10%
	auto label = Label::createWithTTF(__String::createWithFormat("%d",m_number)->getCString(),"fonts/arial.ttf",40);
	switch (this->m_number) {
	case 2:
		bk->setColor(Color3B(255, 0, 156));
	case 4:
		bk->setColor(Color3B(230,210,190));
		break;
	}
	label->setColor(Color3B::BLACK);
	label->setPosition(GAME_TILE_WIDTH/2,GAME_TILE_HEIGHT/2);
	bk->addChild(label);
	label->setTag(10);*/
	return true;
}

void MovedTile::firstInit()
{
	//数字层
	int n = rand()%10;
	this->m_number = n>0 ? 2:4; //n为0-9，n为0的情况占10%，4出现的概率为10%
	auto label = Label::createWithTTF(__String::createWithFormat("%d",m_number)->getCString(),"fonts/arial.ttf",40);
	auto bk = static_cast<LayerColor*>(this->getChildByTag(110));
	switch (this->m_number) {
	case 2:
		bk->setColor(Color3B(255, 0, 156));
	case 4:
		bk->setColor(Color3B(230,210,190));
		break;
	}
	label->setColor(Color3B::BLACK);
	label->setPosition(GAME_TILE_WIDTH/2,GAME_TILE_HEIGHT/2);
	bk->addChild(label);
	label->setTag(10);
}

void MovedTile::showAt(int r, int c)
{
	moveTo(r,c);
	//动画
	auto bk = this->getChildByTag(110);
	bk->runAction(Sequence::create(
					ScaleTo::create(0.15f,0.8f),
					ScaleTo::create(0.15f,1.2f),
					ScaleTo::create(0.15f,1.0f),
					NULL)
					);
}

void MovedTile::moveTo(int r, int c)
{
	m_row = r;
	m_col = c;
	this->setPosition(GAME_TILE_WIDTH*m_col + GAME_TILE_GAP*(m_col+1), 
		GAME_TILE_HEIGHT*m_row + GAME_TILE_GAP*(m_row+1));
}

void MovedTile::doubleNumber()
{
	this->m_number = this->m_number*2;
	auto bk = static_cast<LayerColor*>(this->getChildByTag(110));
	auto label = static_cast<Label*>(bk->getChildByTag(10));
	label->setString(__String::createWithFormat("%d",m_number)->getCString());
	//动画
	bk->runAction(Sequence::create(
		ScaleTo::create(0.15f,0.8f),
		ScaleTo::create(0.15f,1.2f),
		ScaleTo::create(0.15f,1.0f),
		NULL)
		);

	switch (this->m_number) {
	/*
	case 2:
		bk->setColor(Color3B(230,220,210));
		bk->setColor(Color3B(255,255,255));*///用不到
	case 4:
		bk->setColor(Color3B(230,210,190));
		break;
	case 8:
		bk->setColor(Color3B(230,150,100));
		label->setColor(Color3B(255,255,255));
		break;
	case 16:
		bk->setColor(Color3B(230,120,80));
		label->setColor(Color3B(255,255,255));
		break;
	case 32:
		bk->setColor(Color3B(230,100,90));
		label->setColor(Color3B(255,255,255));
		break;
	case 64:
		bk->setColor(Color3B(230,70,60));
		label->setColor(Color3B(255,255,255));
		break;
	case 128:
		label->setScale(0.8f);
		bk->setColor(Color3B(230,190,60));
		label->setColor(Color3B(255,255,255));
		break;
	case 256:
		bk->setColor(Color3B(230,190,60));
		label->setColor(Color3B(255,255,255));
		break;
	case 512:
		bk->setColor(Color3B(230,190,60));
		label->setColor(Color3B(255,255,255));
		break;
	case 1024:
		label->setScale(0.7f);
		break;
	case 2048:
		/*label->setScale(0.7);*///在1024已经放缩，再放缩就不合适了
		bk->setColor(Color3B(210,180,30));
		label->setColor(Color3B(255,255,255));
		UserDefault::getInstance()->setBoolForKey("On2048",true);
		break;
	default:
		break;
	}
}