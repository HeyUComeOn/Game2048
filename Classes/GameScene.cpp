#include "GameScene.h"
#include "GameDefine.h"
#include "MovedTile.h"
#include "Dlog.h"
USING_NS_CC;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

Scene* GameScene::createScene()
{

	auto scene = Scene::create();


	auto layer = GameScene::create();


	scene->addChild(layer);


	return scene;
}


bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	auto labelGame = Label::createWithBMFont("fonts/futura-48.fnt","2048");
	labelGame->setPosition(labelGame->getContentSize().width/2 ,
		GAME_SCREEN_HEIGHT-labelGame->getContentSize().height/2);
	labelGame->setScale(0.5f);
	addChild(labelGame);

	//������������
	auto backWidth = GAME_TILE_WIDTH*GAME_COLS+GAME_TILE_GAP*(GAME_COLS+1);
	auto backHeight = GAME_TILE_HEIGHT*GAME_ROWS+GAME_TILE_GAP*(GAME_ROWS+1);
	colorBack = LayerColor::create(Color4B(170,170,170,255),backWidth,backHeight);
	//colorBack->setAnchorPoint(Vec2(0.5,0.5));ê���޷�����bug
	colorBack->setPosition(GAME_SCREEN_WIDTH/2 - backWidth/2,GAME_SCREEN_HEIGHT/2-backHeight/2) ;
	addChild(colorBack);

	//��ʼ�������ÿһ����
	for (int row = 0; row<GAME_ROWS; row++)
	{
		for(int col = 0; col<GAME_COLS; col++)
		{
			auto layerTile = LayerColor::create(Color4B(70,70,70,255),GAME_TILE_WIDTH,GAME_TILE_HEIGHT);

			layerTile->setPosition(GAME_TILE_WIDTH*col+GAME_TILE_GAP*(col+1), GAME_TILE_HEIGHT*row+GAME_TILE_GAP*(row+1));
			
			colorBack->addChild(layerTile);
		}
		
	}
	//��ʼ��"�߼�������"����
	for(int i= 0;i<GAME_ROWS;i++)
	{
		for(int j = 0;j<GAME_COLS;j++)
		{
			map[i][j] = 0;//�հ�
		}
	}
	//��ʼ�����ֿ�
	newMoveTile();
	
	//�����Ĵ���
	auto event = EventListenerTouchOneByOne::create();
	event->onTouchBegan = [&](Touch*tou,Event* event){
		m_x = tou->getLocation().x;
		m_y = tou->getLocation().y;
		m_startMove = true;
		return true;
	};
	event->onTouchMoved = [&](Touch*tou,Event* event){
		int x = tou->getLocation().x;
		int y = tou->getLocation().y;
		if(m_startMove && (abs(m_x - x)>10||abs(m_y - y)>10))
		{
			m_startMove = false;
			E_MOVE_DIRECT direct;
			if(abs(m_x - x)>abs(m_y - y))
			{
				if (m_x<x)
				{
					direct = E_MOVE_DIRECT::RIGHT; 
				}
				else
				{
					direct = E_MOVE_DIRECT::LEFT;
				}
			}
			else
			{
				if(m_y<y)
				{
					direct = E_MOVE_DIRECT::UP;
				}
				else{
					direct = E_MOVE_DIRECT::DOWN;
				}
			}
			moveAllTile(direct);//���е�Ԫ�ؿ�
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event,this);
	//����

	return true;
}

void GameScene::moveAllTile(E_MOVE_DIRECT direct)
{
	//�ƶ����п�,����
	switch (direct)
	{
	case E_MOVE_DIRECT::UP:
		moveUp();
		break;
	case E_MOVE_DIRECT::DOWN:
		moveDown();
		break;
	case E_MOVE_DIRECT::LEFT:
		moveLeft();
		break;
	case E_MOVE_DIRECT::RIGHT:
		moveRight();
		break;
	default:
		break;
	}
	//��������
	//�ж���Ӯ
	//�����¿�
	newMoveTile();
}

void GameScene::newMoveTile()
{
	auto tile = MovedTile::create();
	int freeCount = 16-m_allTile.size();
	if(freeCount==0)//û�пհ�����
	{
		Dlog::showLog("no Space");
		return;
	}
	int num = rand()%freeCount;
	//����ٴ����ʱ�᲻���ص����˴�ֻҪ�հ���
	int row = 0;
	int col = 0;
	int count = 0;
	bool find = false;
	for (;row<GAME_ROWS;row++)
	{
		for (col = 0;col<GAME_COLS;col++)//�˴�����col=0
		{
			if(map[row][col]==0)
			{
				count++;//��¼�հ׵�����
				if(count >= num)//��==��Ϊ>=
				{
					find = true;
					break;
				}
			}
		}
		if(find)
		{
			break;
		}
	}
	tile->moveTo(row, col);
	colorBack->addChild(tile);
	m_allTile.pushBack(tile);
	//��Ϊ�������ݽṹ����ʾ��m_allTile.getIndex(tile)+1�����ƶ��ÿ�
	map[row][col] = m_allTile.getIndex(tile)+1;//��ʾ��m_allTile.getIndex(tile)�����ƶ��ÿ�
	Dlog::showLog("%d,%d",row,col);
}

void GameScene::moveUp()//�Ӵ˿���
{
	//�ƶ����еĿ�
	for (int col = 0;col<GAME_COLS; col++)
	{
		for (int row = GAME_ROWS-1;row>=0;row--)//row>0��Ϊrow>=0
		{
			if (map[row][col]>0)
			{
				for (int row1 =row;row1<GAME_ROWS-1;row1++)
				{
					if(map[row1+1][col]==0)
					{
						map[row1+1][col] = map[row1][col];
						map[row1][col] = 0;
						m_allTile.at(map[row1+1][col]-1)->moveTo(row1+1,col);
					}else
					{
						break;
					}
				}
			}
		}
	}
}

void GameScene::moveDown()
{
	
}

void GameScene::moveLeft()
{

}

void GameScene::moveRight()
{

}