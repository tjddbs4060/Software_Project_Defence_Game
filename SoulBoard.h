#ifndef __SOUL_BOARD_H__
#define __SOUL_BOARD_H__

#include "cocos2d.h"
#include "Order.h"

#define TAG_SOUL_HERO 500
#define TAG_SOUL_GOLD 501
#define TAG_SOUL_JEWELRY 502
#define TAG_SOUL_SOUL 503

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