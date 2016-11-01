#ifndef __HERO_BOARD_H__
#define __HERO_BOARD_H__

#include "cocos2d.h"
#include "Order.h"

class HeroBoard : public cocos2d::Sprite
{
public:
	HeroBoard();
	~HeroBoard();

	static HeroBoard* create();

	void sprite_init();

	cocos2d::Sprite* get_esc();

private:
	cocos2d::Sprite* esc;
};

#endif