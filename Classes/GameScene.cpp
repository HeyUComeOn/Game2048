#include "GameScene.h"
#include "GameDefine.h"
#include "MovedTile.h"
#include "Dlog.h"
#include "SimpleAudioEngine.h"
#include "GameOver.h"
using namespace CocosDenshion;
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

	//��ʾ��Ϸ����
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

	//��ʼ������
	m_score = 0;
	auto labelScore = Label::createWithBMFont("fonts/futura-48.fnt","score:0");
	labelScore->setPosition(Vec2(GAME_SCREEN_WIDTH/2,
		GAME_SCREEN_HEIGHT-labelGame->getContentSize().height));
	this->addChild(labelScore);
	labelScore->setScale(0.8f);
	labelScore->setTag(120);

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

	return true;
}

void GameScene::moveAllTile(E_MOVE_DIRECT direct)
{
	m_sound_clear = false;
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
	if (m_sound_clear)
	{
		SimpleAudioEngine::getInstance()->playEffect("musics/clear.wav");
	}
	else
	{
		SimpleAudioEngine::getInstance()->playEffect("musics/move.wav");
	}
	//�����仯
	auto labelScore = (Label *)this->getChildByTag(120);
	labelScore->setString(__String::createWithFormat("score:%d",m_score)->getCString());

	//�ж���Ӯ
	//�����¿�
	newMoveTile();
}

void GameScene::newMoveTile()
{
	auto tile = MovedTile::create();
	int freeCount = 16-m_allTile.size();

	if (freeCount==0)
	{
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
	tile->showAt(row, col);
	colorBack->addChild(tile);
	m_allTile.pushBack(tile);
	//��Ϊ�������ݽṹ����ʾ��m_allTile.getIndex(tile)+1�����ƶ��ÿ�
	map[row][col] = m_allTile.getIndex(tile)+1;//��ʾ��m_allTile.getIndex(tile)�����ƶ��ÿ�
	Dlog::showLog("%d,%d",row,col);

	if(freeCount==1)//�ж����һ����
	{
		//�ж���Ϸ��Ӯ
		//���������Ƿ����ƶ�
		for (int r = 0;r<GAME_ROWS;r++)
		{
			for (int c = 0;c<GAME_COLS;c++)
			{
				//��r�е�c�е���ֵ
				int num = m_allTile.at(map[r][c]-1)->m_number;
				int objNum = 0;
				//��
				if(r+1<GAME_ROWS)
				{
					objNum = m_allTile.at(map[r+1][c]-1)->m_number;
					if (num == objNum)
					{
						return;//���ٲ����¿飬������ƶ�
					}
				}
				//��
				if(r-1>=0)
				{
					objNum = m_allTile.at(map[r-1][c]-1)->m_number;
					if (num == objNum)
					{
						return;//���ٲ����¿飬������ƶ�
					}
				}
				//��
				if(c-1>=0)
				{
					objNum = m_allTile.at(map[r][c-1]-1)->m_number;
					if (num == objNum)
					{
						return;//���ٲ����¿飬������ƶ�
					}
				}
				//��
				if(c+1<GAME_COLS)
				{
					objNum = m_allTile.at(map[r][c+1]-1)->m_number;
					if (num == objNum)
					{
						return;//���ٲ����¿飬������ƶ�
					}
				}
			}
		}
		//��ת����
		auto sc = GameOver::createScene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f,sc));
		return;
	}
}

void GameScene::moveUp()//�Ӵ˿���
{
	//�����ƶ����еĿ�
	for (int col = 0;col<GAME_COLS; col++)
	{
		for (int row = GAME_ROWS-1;row>=0;row--)//row>0��Ϊrow>=0
		{
			if (map[row][col]>0)//���ĳһ��������
			{
				for (int row1 =row;row1<GAME_ROWS-1;row1++)//�ж���һ����������
				{
					if(map[row1+1][col]==0)
					{
						//�߼�ש��
						map[row1+1][col] = map[row1][col];
						map[row1][col] = 0;

						//ʵ��ש�飬��һ���±�
						m_allTile.at(  map[row1+1][col]  - 1)->moveTo(row1+1,col);
					}
					else
					{
						
						//�ж��Ƿ��������
						int numObj = m_allTile.at(map[row1+1][col]-1)->m_number;
						int numNow = m_allTile.at(map[row1][col]-1)->m_number;
						if (numObj==numNow)
						{
							m_sound_clear = true;
							m_score += numObj*2;
							m_allTile.at(map[row1+1][col] - 1)->doubleNumber();
							m_allTile.at(map[row1][col]-1)->removeFromParent();
							int index = map[row1][col];
							m_allTile.erase(map[row1][col]-1);

							//������һ�����²���map[row1][col]����������
							
							for (int r = 0; r < GAME_ROWS; r++)
							{
								for (int c = 0; c < GAME_COLS; c++)
								{
									if (map[r][c]>index)
									{
										map[r][c]--;
									}
								}
							}
							map[row1][col] = 0;
						}
						break;
					}
				}
			}
		}
	}
	//�ж��Ƿ��п��������Ŀ�
	//������ǰ���

}

void GameScene::moveDown()
{
	//�����ƶ����еĿ�
	for (int col = 0;col<GAME_COLS; col++)
	{
		for (int row = 0;row<GAME_ROWS;row++)
		{
			if (map[row][col]>0)
			{
				for (int row1 =row;row1>0;row1--)//�ж���һ����������
				{
					if(map[row1-1][col]==0)
					{
						//�߼�ש��
						map[row1-1][col] = map[row1][col];
						map[row1][col] = 0;

						//ʵ��ש�飬��һ���±�
						m_allTile.at(  map[row1-1][col]  - 1)->moveTo(row1-1,col);
					}
					else
					{
						
						//�ж��Ƿ��������
						int numObj = m_allTile.at(map[row1-1][col]-1)->m_number;
						int numNow = m_allTile.at(map[row1][col]-1)->m_number;
						if (numObj==numNow)
						{
							m_sound_clear = true;
							m_score += numObj*2;
							m_allTile.at(map[row1-1][col] - 1)->doubleNumber();
							m_allTile.at(map[row1][col]-1)->removeFromParent();
							int index = map[row1][col];
							m_allTile.erase(map[row1][col]-1);

							//������һ�����²���map[row1][col]����������

							for (int r = 0; r < GAME_ROWS; r++)
							{
								for (int c = 0; c < GAME_COLS; c++)
								{
									if (map[r][c]>index)
									{
										map[r][c]--;
									}
								}
							}
							map[row1][col] = 0;
						}
						break;
					}
				}
			}
		}
	}

}

void GameScene::moveLeft()
{
	//�����ƶ����еĿ�
	for (int row = 0;row<GAME_ROWS; row++)
	{
		for (int col = 0;col<GAME_COLS;col++)
		{
			if (map[row][col]>0)//���ĳһ��������
			{
				for (int col1 =col;col1>0;col1--)//�ж���һ����������
				{
					if(map[row][col1-1]==0)
					{
						//�߼�ש��
						map[row][col1-1] = map[row][col1];
						map[row][col1] = 0;

						//ʵ��ש�飬��һ���±�
						m_allTile.at(  map[row][col1-1]  - 1)->moveTo(row,col1-1);
					}
					else
					{
						
						//�ж��Ƿ��������
						int numObj = m_allTile.at(map[row][col1-1]-1)->m_number;
						int numNow = m_allTile.at(map[row][col1]-1)->m_number;
						if (numObj==numNow)
						{
							m_sound_clear = true;
							m_score += numObj*2;
							m_allTile.at(map[row][col1-1] - 1)->doubleNumber();
							m_allTile.at(map[row][col1]-1)->removeFromParent();
							int index = map[row][col1];
							m_allTile.erase(map[row][col1]-1);

							//������һ�����²���map[row1][col]����������

							for (int r = 0; r < GAME_ROWS; r++)
							{
								for (int c = 0; c < GAME_COLS; c++)
								{
									if (map[r][c]>index)
									{
										map[r][c]--;
									}
								}
							}
							map[row][col1] = 0;
						}
						break;
					}
				}
			}
		}
	}

}

void GameScene::moveRight()
{
	//�����ƶ����еĿ�
	for (int row = 0;row<GAME_ROWS; row++)
	{
		for (int col = GAME_COLS-1;col>=0;col--)
		{
			if (map[row][col]>0)//���ĳһ��������
			{
				for (int col1 =col;col1<GAME_COLS-1;col1++)//�ж���һ����������
				{
					if(map[row][col1+1]==0)
					{
						//�߼�ש��
						map[row][col1+1] = map[row][col1];
						map[row][col1] = 0;

						//ʵ��ש�飬��һ���±�
						m_allTile.at(  map[row][col1+1]  - 1)->moveTo(row,col1+1);
					}
					else
					{
						
						//�ж��Ƿ��������
						int numObj = m_allTile.at(map[row][col1+1]-1)->m_number;
						int numNow = m_allTile.at(map[row][col1]-1)->m_number;
						if (numObj==numNow)
						{
							m_sound_clear = true;
							m_score += numObj*2;
							m_allTile.at(map[row][col1+1] - 1)->doubleNumber();
							m_allTile.at(map[row][col1]-1)->removeFromParent();
							int index = map[row][col1];
							m_allTile.erase(map[row][col1]-1);

							//������һ�����²���map[row1][col]����������

							for (int r = 0; r < GAME_ROWS; r++)
							{
								for (int c = 0; c < GAME_COLS; c++)
								{
									if (map[r][c]>index)
									{
										map[r][c]--;
									}
								}
							}
							map[row][col1] = 0;
						}
						break;
					}
				}
			}
		}
	}

}