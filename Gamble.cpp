#include "Gamble.h"

USING_NS_CC;

Gamble::Gamble()
{
}

Gamble::~Gamble()
{
}

void Gamble::sprite_init()
{
	low = Sprite::createWithSpriteFrameName("select_button.png");
	middle = Sprite::createWithSpriteFrameName("select_button.png");
	high = Sprite::createWithSpriteFrameName("select_button.png");
	esc = Sprite::createWithSpriteFrameName("esc_button.png");

	low->setPosition(Point(76, 89));
	middle->setPosition(Point(126, 89));
	high->setPosition(Point(176, 89));
	esc->setPosition(Point(235, 189));

	addChild(low);
	addChild(middle);
	addChild(high);
	addChild(esc);
}

Sprite* Gamble::get_low()
{
	return low;
}

Sprite* Gamble::get_middle()
{
	return middle;
}

Sprite* Gamble::get_high()
{
	return high;
}

Sprite* Gamble::get_esc()
{
	return esc;
}

Gamble* Gamble::create()
{
	Gamble* gamble = new Gamble();
	Size winSize = Director::getInstance()->getWinSize();

	if (gamble && gamble->initWithSpriteFrameName("gamble_select_menu.png"))
	{
		gamble->autorelease();

		gamble->sprite_init();

		return gamble;
	}

	CC_SAFE_DELETE(gamble);

	return NULL;
}

int Gamble::low_gamble()
{
	srand((unsigned int)time(NULL));

	if (rand() % 3 == 0)
		return rand() % 30;
	else return rand() % 15;
}

int Gamble::middle_gamble()
{
	srand((unsigned int)time(NULL));

	if (rand() % 3 == 0)
		return rand() % 300;
	else return rand() % 150;
}

int Gamble::high_gamble()
{
	srand((unsigned int)time(NULL));

	if (rand() % 3 == 0)
		return rand() % 1500;
	else return rand() % 750;
}
