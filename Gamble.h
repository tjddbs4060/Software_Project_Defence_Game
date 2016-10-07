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

	int low_gamble();
	int middle_gamble();
	int high_gamble();
};

#endif
