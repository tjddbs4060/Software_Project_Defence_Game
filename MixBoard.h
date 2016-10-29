#ifndef __MIX_BOARD_H__
#define __MIX_BOARD_H__

#include "cocos2d.h"
#include "Order.h"

class MixBoard : public cocos2d::Sprite
{
public:
	MixBoard();
	virtual ~MixBoard();

	static MixBoard* create();

	void sprite_init();

	cocos2d::Sprite* get_esc();

private:
	cocos2d::Sprite* low;
	cocos2d::Sprite* middle;
	cocos2d::Sprite* high;
	cocos2d::Sprite* esc;
};

#endif
