#include "GameScene.h"
#include "GameDefine.h"

#include "Dlog.h"
#include "SimpleAudioEngine.h"
#include "GameOver.h"
#include "GameWin.h"
using namespace CocosDenshion;
USING_NS_CC;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	
}

void GameScene::onExit()
{
	Layer::onExit();
	Director::getInstance()->getEventDispatcher()->removeEventListener(static_cast<EventListener*>(m_Event));
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

	//显示游戏名称
	auto labelGame = Label::createWithBMFont("fonts/futura-48.fnt","2048");
	labelGame->setPosition(labelGame->getContentSize().width/2 ,
		GAME_SCREEN_HEIGHT-labelGame->getContentSize().height/2);
	labelGame->setScale(0.5f);
	addChild(labelGame);

	//显示游戏声音控制菜单
	auto item1 = MenuItemImage::create("sound.png","sound.png");
	auto item2 = MenuItemImage::create("soundOff.png","soundOff.png");
	auto mntoggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(GameScene::soundCallback,this),item1,item2,NULL);
	mntoggle->setPosition(GAME_SCREEN_WIDTH-mntoggle->getContentSize().width/2 ,
		GAME_SCREEN_HEIGHT-mntoggle->getContentSize().height/2);
	auto mn = Menu::create(mntoggle,NULL);
	mn->setPosition(Point::ZERO);
	this->addChild(mn,1);
	//起始喇叭开启
	UserDefault::getInstance()->setBoolForKey("loseMusicOff",false);

	//加载判断是否到2048的数据
	UserDefault::getInstance()->setBoolForKey("On2048",false);

	//创建背景网格
	auto backWidth = GAME_TILE_WIDTH*GAME_COLS+GAME_TILE_GAP*(GAME_COLS+1);
	auto backHeight = GAME_TILE_HEIGHT*GAME_ROWS+GAME_TILE_GAP*(GAME_ROWS+1);
	colorBack = LayerColor::create(Color4B(170,170,170,255),backWidth,backHeight);
	//colorBack->setAnchorPoint(Vec2(0.5,0.5));锚点无法更改bug
	colorBack->setPosition(GAME_SCREEN_WIDTH/2 - backWidth/2,GAME_SCREEN_HEIGHT/2-backHeight/2) ;
	addChild(colorBack);

	//初始化分数
	m_score = 0;
	auto labelScore = Label::createWithBMFont("fonts/futura-48.fnt","score:0");
	labelScore->setPosition(Vec2(GAME_SCREEN_WIDTH/2,
		GAME_SCREEN_HEIGHT-labelGame->getContentSize().height));
	this->addChild(labelScore);
	labelScore->setScale(0.8f);
	labelScore->setTag(120);

	//初始化网格的每一个块
	for (int row = 0; row<GAME_ROWS; row++)
	{
		for(int col = 0; col<GAME_COLS; col++)
		{
			auto layerTile = LayerColor::create(Color4B(70,70,70,255),GAME_TILE_WIDTH,GAME_TILE_HEIGHT);

			layerTile->setPosition(GAME_TILE_WIDTH*col+GAME_TILE_GAP*(col+1), GAME_TILE_HEIGHT*row+GAME_TILE_GAP*(row+1));
			
			colorBack->addChild(layerTile);
		}
		
	}
	//初始化"逻辑的网格"数组
	for(int i= 0;i<GAME_ROWS;i++)
	{
		for(int j = 0;j<GAME_COLS;j++)
		{
			map[i][j] = 0;//空白
		}
	}
	//初始化数字块
	newMoveTile();
	
	//添加反悔按钮
	auto backSpace = LayerColor::create(Color4B(70,70,70,255),46,32);
	backSpace->setPosition(GAME_SCREEN_WIDTH/2-backWidth/2 ,
		GAME_SCREEN_HEIGHT/2+backHeight/2);
	addChild(backSpace);
	auto itemBack = MenuItemImage::create("btn-back-0.png", "btn-back-1.png", 
		CC_CALLBACK_1(GameScene::backCallback,this));
	auto menuBack = Menu::create(itemBack,nullptr);
	menuBack->setPosition(Point::ZERO);
	itemBack->setPosition(backSpace->getContentSize().width/2,backSpace->getContentSize().height/2);
	backSpace->addChild(menuBack);

	//触摸的处理
	m_Event = EventListenerTouchOneByOne::create();
	m_Event->onTouchBegan = [&](Touch*tou,Event* event){
		m_x = tou->getLocation().x;
		m_y = tou->getLocation().y;
		m_startMove = true;
		return true;
	};
	m_Event->onTouchMoved = [&](Touch*tou,Event* event){
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
			moveAllTile(direct);//所有的元素块
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_Event, this);

	return true;
}

void GameScene::soundCallback(Ref* pSender)
{
	auto item = static_cast<MenuItemToggle*>(pSender);
	switch (item->getSelectedIndex())
	{
	case 0:
		SimpleAudioEngine::getInstance()->resumeAllEffects();
		UserDefault::getInstance()->setBoolForKey("loseMusicOff",false);
		break;
	case 1:
		SimpleAudioEngine::getInstance()->pauseAllEffects();
		UserDefault::getInstance()->setBoolForKey("loseMusicOff",true);
		break;
	default:
		break;
	}

}

void GameScene::moveAllTile(E_MOVE_DIRECT direct)
{
	m_sound_clear = false;
	//移动所有块,消除
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
	//播放音乐
	if (m_sound_clear)
	{
		SimpleAudioEngine::getInstance()->playEffect("musics/clear.wav");
	}
	else
	{
		SimpleAudioEngine::getInstance()->playEffect("musics/move.wav");
	}
	//分数变化
	auto labelScore = (Label *)this->getChildByTag(120);
	labelScore->setString(__String::createWithFormat("score:%d",m_score)->getCString());

	//判定是否赢了
	if(UserDefault::getInstance()->getBoolForKey("On2048"))
	{
		//todo
		auto sc = GameWin::createScene();
		Director::getInstance()->replaceScene(TransitionSlideInB::create(3.0f,sc));
		UserDefault::getInstance()->setIntegerForKey("Score",m_score);

	}
	//产生新块
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
	//检测再次添加时会不会重叠，此处只要空白数
	int row = 0;
	int col = 0;
	int count = 0;
	bool find = false;
	for (;row<GAME_ROWS;row++)
	{
		for (col = 0;col<GAME_COLS;col++)//此处加上col=0
		{
			if(map[row][col]==0)
			{
				count++;//记录空白的数量
				if(count >= num)//由==改为>=
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
	//此为核心数据结构，表示有m_allTile.getIndex(tile)+1个可移动得块
	map[row][col] = m_allTile.getIndex(tile)+1;//[row][col]为砖块的排名
	Dlog::showLog("%d,%d",row,col);

	if(freeCount==1)//判断最后一个块
	{
		//判断游戏输赢
		//上下左右是否还能移动
		for (int r = 0;r<GAME_ROWS;r++)
		{
			for (int c = 0;c<GAME_COLS;c++)
			{
				//第r行第c列的数值
				int num = m_allTile.at(map[r][c]-1)->m_number;
				int objNum = 0;
				//上
				if(r+1<GAME_ROWS)
				{
					objNum = m_allTile.at(map[r+1][c]-1)->m_number;
					if (num == objNum)
					{
						return;//不再产生新块，请继续移动
					}
				}
				//下
				if(r-1>=0)
				{
					objNum = m_allTile.at(map[r-1][c]-1)->m_number;
					if (num == objNum)
					{
						return;//不再产生新块，请继续移动
					}
				}
				//左
				if(c-1>=0)
				{
					objNum = m_allTile.at(map[r][c-1]-1)->m_number;
					if (num == objNum)
					{
						return;//不再产生新块，请继续移动
					}
				}
				//右
				if(c+1<GAME_COLS)
				{
					objNum = m_allTile.at(map[r][c+1]-1)->m_number;
					if (num == objNum)
					{
						return;//不再产生新块，请继续移动
					}
				}
			}
		}
		//跳转场景
		auto sc = GameOver::createScene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f,sc));
		return;
	}
}

void GameScene::moveUp()//从此看起
{
	//向上移动所有的块
	m_LastAllTile = m_allTile;
	for (int col = 0;col<GAME_COLS; col++)
	{
		for (int row = GAME_ROWS-1;row>=0;row--)//row>0改为row>=0
		{
			if (map[row][col]>0)//如果某一块有数字
			{
				for (int row1 =row;row1<GAME_ROWS-1;row1++)//判断这一块上面所有
				{
					if(map[row1+1][col]==0)
					{
						//砖块排名
						map[row1+1][col] = map[row1][col];
						map[row1][col] = 0;

						//实际砖块，减一得下标
						m_allTile.at(  map[row1+1][col]  - 1)->moveTo(row1+1,col);
					}
					else
					{
						
						//判断是否可以消除
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

							//块数减一，重新布局map[row1][col]所代表数字
							
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
	//判断是否有可以消除的块
	//消除最前面的

}

void GameScene::moveDown()
{
	//向下移动所有的块
	m_LastAllTile = m_allTile;
	for (int col = 0;col<GAME_COLS; col++)
	{
		for (int row = 0;row<GAME_ROWS;row++)
		{
			if (map[row][col]>0)
			{
				for (int row1 =row;row1>0;row1--)//判断这一块下面所有
				{
					if(map[row1-1][col]==0)
					{
						//砖块排名
						map[row1-1][col] = map[row1][col];
						map[row1][col] = 0;

						//实际砖块，减一得下标
						m_allTile.at(  map[row1-1][col]  - 1)->moveTo(row1-1,col);
					}
					else
					{
						
						//判断是否可以消除
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

							//块数减一，重新布局map[row1][col]所代表数字

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
	//向左移动所有的块
	m_LastAllTile = m_allTile;
	for (int row = 0;row<GAME_ROWS; row++)
	{
		for (int col = 0;col<GAME_COLS;col++)
		{
			if (map[row][col]>0)//如果某一块有数字
			{
				for (int col1 =col;col1>0;col1--)//判断这一块左面所有
				{
					if(map[row][col1-1]==0)
					{
						//砖块排名
						map[row][col1-1] = map[row][col1];
						map[row][col1] = 0;

						//实际砖块，减一得下标
						m_allTile.at(  map[row][col1-1]  - 1)->moveTo(row,col1-1);
					}
					else
					{
						
						//判断是否可以消除
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

							//块数减一，重新布局map[row1][col]所代表数字

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
	//向右移动所有的块
	m_LastAllTile = m_allTile;
	for (int row = 0;row<GAME_ROWS; row++)
	{
		for (int col = GAME_COLS-1;col>=0;col--)
		{
			if (map[row][col]>0)//如果某一块有数字
			{
				for (int col1 =col;col1<GAME_COLS-1;col1++)//判断这一块右面所有
				{
					if(map[row][col1+1]==0)
					{
						//砖块排名
						map[row][col1+1] = map[row][col1];
						map[row][col1] = 0;

						//实际砖块，减一得下标
						m_allTile.at(  map[row][col1+1]  - 1)->moveTo(row,col1+1);
					}
					else
					{
						
						//判断是否可以消除
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

							//块数减一，重新布局map[row1][col]所代表数字

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

void GameScene::backCallback(cocos2d::Ref* pSender)
{
	/*
			1.现在来看只好先记住各个砖块的排名，即map[row][col]
			2.m_allTile = m_LastAllTile
			3.最后利用m_number来重新铺设真实砖块
			
	map[m_allTile.m_row[m_allTile.m_col] = 0;

	m_allTile = m_LastAllTile;*/
}