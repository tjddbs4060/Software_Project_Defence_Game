#include "MixBoard.h"

USING_NS_CC;

MixBoard::MixBoard()
{
}

MixBoard::~MixBoard()
{
}

void MixBoard::sprite_init()
{
	esc = Sprite::createWithSpriteFrameName("esc_button_mix.png");

	esc->setPosition(Point(185, 240));

	addChild(esc);
}

Sprite* MixBoard::get_esc()
{
	return esc;
}

MixBoard* MixBoard::create()
{
	MixBoard* mixBoard = new MixBoard();
	Size winSize = Director::getInstance()->getWinSize();

	if (mixBoard && mixBoard->initWithSpriteFrameName("mix_menu.png"))
	{
		mixBoard->autorelease();

		mixBoard->sprite_init();

		return mixBoard;
	}

	CC_SAFE_DELETE(mixBoard);

	return NULL;
}