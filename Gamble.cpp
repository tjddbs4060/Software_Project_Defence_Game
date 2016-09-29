#include "Gamble.h"

USING_NS_CC;

Gamble::Gamble()
{
}

Gamble::~Gamble()
{
}

Gamble* Gamble::create()
{
	Gamble* gamble = new Gamble();
	Size winSize = Director::getInstance()->getWinSize();

	if (gamble && gamble->initWithSpriteFrameName("gamble_select_menu.png"))
	{
		gamble->autorelease();

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
