#include "HeroBoard.h"

USING_NS_CC;

HeroBoard::HeroBoard()
{
}

HeroBoard::~HeroBoard()
{
}

HeroBoard* HeroBoard::create()
{
	HeroBoard* heroBoard = new HeroBoard();

	if (heroBoard && heroBoard->initWithSpriteFrameName("hero_list_menu.png"))
	{
		heroBoard->autorelease();

		heroBoard->sprite_init();

		return heroBoard;
	}

	CC_SAFE_DELETE(heroBoard);

	return NULL;
}

Sprite* HeroBoard::get_esc()
{
	return esc;
}

void HeroBoard::sprite_init()
{
	esc = Sprite::createWithSpriteFrameName("esc_button_mix.png");
	esc->setPosition(Point(236, 189));
	addChild(esc);

	Sprite* label = Sprite::createWithSpriteFrameName("hero_list_label.png");
	label->setPosition(Point(125, 170));
	addChild(label);
}