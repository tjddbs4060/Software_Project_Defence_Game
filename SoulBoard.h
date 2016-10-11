#ifndef __SOUL_BOARD_H__
#define __SOUL_BOARD_H__

#include "cocos2d.h"
#include "Order.h"

#define TAG_SOUL_HERO_1 500
#define TAG_SOUL_HERO_2 502
#define TAG_SOUL_GOLD_1 503
#define TAG_SOUL_GOLD_2 504
#define TAG_SOUL_JEWELRY_1 505
#define TAG_SOUL_JEWELRY_2 506
#define TAG_SOUL_SOUL_1 507
#define TAG_SOUL_SOUL_2 508

class SoulBoard : public cocos2d::Sprite
{
public:
	SoulBoard();
	virtual ~SoulBoard();

	void setSoul(int soul);
	void setGold(int gold);
	void setHero(int Hero);
	void setJewelry(int jewelry);
	int getSoul();
	int getGold();
	int getHero();
	int getJewelry();

	static SoulBoard* create();

	void addNumberSprite(int tag, cocos2d::Point position, float scale);	//숫자 추가
	void changeNumber(int tag, int number);	//숫자 변경

	void updateNumber(SoulBoard* soulBoard);

private:
	int mSoul;
	int mGold;
	int mHero;
	int mJewelry;
};

#endif