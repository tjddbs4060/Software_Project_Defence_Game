#include "InforBoard.h"

USING_NS_CC;

InforBoard::InforBoard() : mTime(5), mGold(0), mJewelry(0), mLevel(0), mMonster(0), mSoul(0)
{
}

InforBoard::~InforBoard()
{
}

void InforBoard::setTime(float time)
{
	mTime = time;

	update_time();
}

void InforBoard::setGold(int gold)
{
	mGold = gold;

	update_G_J(TAG_BOARD_GOLD, mGold);
}

void InforBoard::setJewelry(int jewelry)
{
	mJewelry = jewelry;

	update_G_J(TAG_BOARD_JEWELRY, mJewelry);
}

void InforBoard::setLevel(int level)
{
	mLevel = level;
}

void InforBoard::setMonster(int monster)
{
	mMonster = monster;

	update_monster();
}

float InforBoard::getTime()
{
	return mTime;
}

int InforBoard::getGold()
{
	return mGold;
}

int InforBoard::getJewelry()
{
	return mJewelry;
}

int InforBoard::getLevel()
{
	return mLevel;
}

int InforBoard::getMonster()
{
	return mMonster;
}

InforBoard* InforBoard::create()
{
	InforBoard* inforBoard = new InforBoard();
	Size winSize = Director::getInstance()->getWinSize();

	if (inforBoard && inforBoard->initWithSpriteFrameName("infor_board.png"))
	{
		inforBoard->autorelease();

		//몬스터
		float xPos = 140;

		inforBoard->addNumberSprite(TAG_BOARD_MONSTER + 10, Point(170, 52), 0.7);
		inforBoard->addNumberSprite(TAG_BOARD_MONSTER + 11, Point(160, 52), 0.7);

		for (int i = 0; i < 2; i++)
		{
			inforBoard->addNumberSprite(TAG_BOARD_MONSTER + i, Point(xPos, 52), 0.7);

			xPos -= 10;
		}

		//골드
		xPos = 335;

		for (int i = 0; i < 5; i++)
		{
			inforBoard->addNumberSprite(TAG_BOARD_GOLD + i, Point(xPos, 52), 0.7);

			xPos -= 10;
		}

		//보석
		xPos = 410;

		for (int i = 0; i < 5; i++)
		{
			inforBoard->addNumberSprite(TAG_BOARD_JEWELRY + i, Point(xPos, 52), 0.7);

			xPos -= 10;
		}

		//타이머
		inforBoard->addNumberSprite(TAG_BOARD_TIME_01, Point(247, 40), 1.2);
		inforBoard->addNumberSprite(TAG_BOARD_TIME_02, Point(235, 40), 1.2);

		return inforBoard;
	}

	CC_SAFE_DELETE(inforBoard);

	return NULL;
}

void InforBoard::addNumberSprite(int tag, Point position, float scale)
{
	Sprite* sprite = Sprite::createWithSpriteFrameName("number_0.png");
	sprite->setPosition(position);
	sprite->setScale(scale);
	sprite->setTag(tag);
	addChild(sprite);
}

void InforBoard::changeNumber(int tag, int number)
{
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	char szFile[32] = { 0, };

	sprintf(szFile, "number_%d.png", number % 10);
	SpriteFrame* frame = frameCache->getSpriteFrameByName(szFile);
	((Sprite*)getChildByTag(tag))->setDisplayFrame(frame);
}

void InforBoard::update_time()
{
	int temp = (int)mTime;

	if (mTime < 0)
	{
		setTime(60);///////////
		setLevel(getLevel() + 1);
	}

	changeNumber(TAG_BOARD_TIME_01, temp % 10);
	changeNumber(TAG_BOARD_TIME_02, (temp/10) % 10);
}

void InforBoard::update_G_J(int tag, int number)
{
	int temp = number;

	for (int i = 0; i < 5; i++)
	{
		changeNumber(tag + i, temp);
		temp /= 10;
	}
}

void InforBoard::update_monster()
{
	int temp = mMonster;
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	char szFile[32] = { 0, };

	for (int i = 0; i < 2; i++)
	{
		changeNumber(TAG_BOARD_MONSTER + i, temp);
		temp /= 10;
	}
	changeNumber(TAG_BOARD_MONSTER + 10, 0);
	changeNumber(TAG_BOARD_MONSTER + 11, 5);
}

void InforBoard::setSoul(int soul)
{
	mSoul = soul;
}

int InforBoard::getSoul()
{
	return mSoul;
}