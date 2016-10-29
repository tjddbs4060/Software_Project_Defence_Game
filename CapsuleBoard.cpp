#include "CapsuleBoard.h"

USING_NS_CC;

CapsuleBoard::CapsuleBoard()
{
}

CapsuleBoard::~CapsuleBoard()
{
}

CapsuleBoard* CapsuleBoard::create()
{
	CapsuleBoard* capsuleBoard = new CapsuleBoard();
	Size winSize = Director::getInstance()->getWinSize();

	if (capsuleBoard && capsuleBoard->initWithSpriteFrameName("capsule_select_menu.png"))
	{
		capsuleBoard->autorelease();

		capsuleBoard->sprite_init();

		return capsuleBoard;
	}

	CC_SAFE_DELETE(capsuleBoard);

	return NULL;
}

void CapsuleBoard::sprite_init()
{
	D_capsule = Sprite::createWithSpriteFrameName("D_capsule.png");
	C_capsule = Sprite::createWithSpriteFrameName("C_capsule.png");
	B_capsule = Sprite::createWithSpriteFrameName("B_capsule.png");
	A_capsule = Sprite::createWithSpriteFrameName("A_capsule.png");
	S_capsule = Sprite::createWithSpriteFrameName("S_capsule.png");
	SS_capsule = Sprite::createWithSpriteFrameName("SS_capsule.png");
	esc = Sprite::createWithSpriteFrameName("esc_button.png");

	D_capsule->setPosition(Point(76, 130));
	C_capsule->setPosition(Point(126, 130));
	B_capsule->setPosition(Point(176, 130));
	A_capsule->setPosition(Point(76, 70));
	S_capsule->setPosition(Point(126, 70));
	SS_capsule->setPosition(Point(176, 70));
	esc->setPosition(Point(235, 189));

	addChild(D_capsule);
	addChild(C_capsule);
	addChild(B_capsule);
	addChild(A_capsule);
	addChild(S_capsule);
	addChild(SS_capsule);
	addChild(esc);
}

Sprite* CapsuleBoard::get_D_capsule()
{
	return D_capsule;
}

Sprite* CapsuleBoard::get_C_capsule()
{
	return C_capsule;
}

Sprite* CapsuleBoard::get_B_capsule()
{
	return B_capsule;
}

Sprite* CapsuleBoard::get_A_capsule()
{
	return A_capsule;
}

Sprite* CapsuleBoard::get_S_capsule()
{
	return S_capsule;
}

Sprite* CapsuleBoard::get_SS_capsule()
{
	return SS_capsule;
}

Sprite* CapsuleBoard::get_esc()
{
	return esc;
}

int CapsuleBoard::D_gamble()
{
	srand((unsigned int)time(NULL));

	return rand() % 10 + 1;
}

int CapsuleBoard::C_gamble()
{
	srand((unsigned int)time(NULL));

	return rand() % 12 + 1;
}

int CapsuleBoard::B_gamble()
{
	srand((unsigned int)time(NULL));

	return rand() % 15 + 1;
}

int CapsuleBoard::A_gamble()
{
	srand((unsigned int)time(NULL));

	return rand() % 13 + 1;
}

int CapsuleBoard::S_gamble()
{
	srand((unsigned int)time(NULL));

	return rand() % 8 + 1;
}

int CapsuleBoard::SS_gamble()
{
	srand((unsigned int)time(NULL));

	return rand() % 4 + 1;
}
