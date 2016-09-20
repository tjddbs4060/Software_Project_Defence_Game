#include "SoulBoard.h"

USING_NS_CC;

SoulBoard::SoulBoard() : mSoul(0), mGold(0), mHero(0), mJewelry(0)
{
}

SoulBoard::~SoulBoard()
{
}

void SoulBoard::setSoul(int soul)
{
	mSoul = soul;
}

void SoulBoard::setGold(int gold)
{
	mGold = gold;
}

void SoulBoard::setHero(int hero)
{
	mHero = hero;
}

void SoulBoard::setJewelry(int jewelry)
{
	mJewelry = jewelry;
}

int SoulBoard::getSoul()
{
	return mSoul;
}

int SoulBoard::getGold()
{
	return mGold;
}

int SoulBoard::getHero()
{
	return mHero;
}

int SoulBoard::getJewelry()
{
	return mJewelry;
}

SoulBoard* SoulBoard::create()
{
	SoulBoard* soulBoard = new SoulBoard();
	Size winSize = Director::getInstance()->getWinSize();

	if (soulBoard && soulBoard->initWithSpriteFrameName("soul_select_menu.png"))
	{
		soulBoard->autorelease();

		//¿µ¿õ
		soulBoard->addNumberSprite(TAG_SOUL_HERO, Point(75, 98), 0.5);
		soulBoard->addNumberSprite(TAG_SOUL_GOLD, Point(126, 98), 0.5);
		soulBoard->addNumberSprite(TAG_SOUL_JEWELRY, Point(177, 98), 0.5);
		
		soulBoard->addNumberSprite(TAG_SOUL_SOUL, Point(126, 165), 1);
		
		return soulBoard;
	}

	CC_SAFE_DELETE(soulBoard);

	return NULL;
}

void SoulBoard::addNumberSprite(int tag, Point position, float scale)
{
	Sprite* sprite = Sprite::createWithSpriteFrameName("number_0.png");
	sprite->setPosition(position);
	sprite->setScale(scale);
	sprite->setTag(tag);
	addChild(sprite);
}

void SoulBoard::changeNumber(int tag, int number)
{
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	char szFile[32] = { 0, };

	sprintf(szFile, "number_%d.png", number % 10);
	SpriteFrame* frame = frameCache->getSpriteFrameByName(szFile);
	((Sprite*)getChildByTag(tag))->setDisplayFrame(frame);
}

void SoulBoard::updateNumber(SoulBoard* soulBoard)
{
	changeNumber(TAG_SOUL_HERO, soulBoard->getHero());
	changeNumber(TAG_SOUL_GOLD, soulBoard->getGold());
	changeNumber(TAG_SOUL_JEWELRY, soulBoard->getJewelry());
	changeNumber(TAG_SOUL_SOUL, soulBoard->getSoul());
}
