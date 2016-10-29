#include "SoulBoard.h"

USING_NS_CC;

SoulBoard::SoulBoard() : mSoul(0), mGold(0), mHero(0), mJewelry(0)
{
}

SoulBoard::~SoulBoard()
{
}

void SoulBoard::sprite_init()
{
	esc_button = Sprite::createWithSpriteFrameName("esc_button.png");
	gold_up_button = Sprite::createWithSpriteFrameName("up_button.png");
	gold_down_button = Sprite::createWithSpriteFrameName("down_button.png");
	hero_up_button = Sprite::createWithSpriteFrameName("up_button.png");
	hero_down_button = Sprite::createWithSpriteFrameName("down_button.png");
	jewelry_up_button = Sprite::createWithSpriteFrameName("up_button.png");
	jewelry_down_button = Sprite::createWithSpriteFrameName("down_button.png");
	ok_button = Sprite::createWithSpriteFrameName("ok_button.png");

	esc_button->setPosition(Point(235, 189));
	gold_up_button->setPosition(Point(141, 100));
	gold_down_button->setPosition(Point(113, 100));
	hero_up_button->setPosition(Point(90, 100));
	hero_down_button->setPosition(Point(62, 100));
	jewelry_up_button->setPosition(Point(191, 100));
	jewelry_down_button->setPosition(Point(163, 100));
	ok_button->setPosition(Point(127, 56));

	addChild(esc_button);
	addChild(gold_up_button);
	addChild(gold_down_button);
	addChild(hero_up_button);
	addChild(hero_down_button);
	addChild(jewelry_up_button);
	addChild(jewelry_down_button);
	addChild(ok_button);
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

		soulBoard->sprite_init();

		soulBoard->addNumberSprite(TAG_SOUL_HERO_1, Point(73, 98), 0.5);
		soulBoard->addNumberSprite(TAG_SOUL_HERO_2, Point(78, 98), 0.5);
		soulBoard->addNumberSprite(TAG_SOUL_GOLD_1, Point(124, 98), 0.5);
		soulBoard->addNumberSprite(TAG_SOUL_GOLD_2, Point(129, 98), 0.5);
		soulBoard->addNumberSprite(TAG_SOUL_JEWELRY_1, Point(174, 98), 0.5);
		soulBoard->addNumberSprite(TAG_SOUL_JEWELRY_2, Point(180, 98), 0.5);
		
		soulBoard->addNumberSprite(TAG_SOUL_SOUL_1, Point(118, 165), 1);
		soulBoard->addNumberSprite(TAG_SOUL_SOUL_2, Point(134, 165), 1);

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
	changeNumber(TAG_SOUL_HERO_1, (soulBoard->getHero()/10)%10);
	changeNumber(TAG_SOUL_HERO_2, soulBoard->getHero()%10);
	changeNumber(TAG_SOUL_GOLD_1, (soulBoard->getGold()/10)%10);
	changeNumber(TAG_SOUL_GOLD_2, soulBoard->getGold()%10);
	changeNumber(TAG_SOUL_JEWELRY_1, (soulBoard->getJewelry()/10)%10);
	changeNumber(TAG_SOUL_JEWELRY_2, soulBoard->getJewelry()%10);
	changeNumber(TAG_SOUL_SOUL_1, (soulBoard->getSoul()/10)%10);
	changeNumber(TAG_SOUL_SOUL_2, soulBoard->getSoul()%10);
}

Sprite* SoulBoard::get_esc()
{
	return esc_button;
}

Sprite* SoulBoard::get_gold_up()
{
	return gold_up_button;
}

Sprite* SoulBoard::get_gold_down()
{
	return gold_down_button;
}

Sprite* SoulBoard::get_hero_up()
{
	return hero_up_button;
}

Sprite* SoulBoard::get_hero_down()
{
	return hero_down_button;
}

Sprite* SoulBoard::get_jewelry_up()
{
	return jewelry_up_button;
}

Sprite* SoulBoard::get_jewelry_down()
{
	return jewelry_down_button;
}

Sprite* SoulBoard::get_ok()
{
	return ok_button;
}
