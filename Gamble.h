#ifndef __GAMBLE_BOARD_H__
#define __GAMBLE_BOARD_H__

#include "cocos2d.h"
#include "Order.h"

class Gamble : public cocos2d::Sprite
{
public:
	Gamble();
	virtual ~Gamble();

	static Gamble* create();

	void sprite_init();
	
	cocos2d::Sprite* get_low();
	cocos2d::Sprite* get_middle();
	cocos2d::Sprite* get_high();
	cocos2d::Sprite* get_esc();

	int low_gamble();
	int middle_gamble();
	int high_gamble();

private:
	cocos2d::Sprite* low;
	cocos2d::Sprite* middle;
	cocos2d::Sprite* high;
	cocos2d::Sprite* esc;
};

#endif
