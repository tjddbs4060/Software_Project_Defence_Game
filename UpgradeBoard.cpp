#include "UpgradeBoard.h"

USING_NS_CC;

UpgradeBoard::UpgradeBoard() : D(0), C(0), B(0), A(0), S(0), SS(0)
{
}

UpgradeBoard::~UpgradeBoard()
{
}

void UpgradeBoard::setD(int d)
{
	D = d;

	update();
}

void UpgradeBoard::setC(int c)
{
	C = c;

	update();
}

void UpgradeBoard::setB(int b)
{
	B = b;

	update();
}

void UpgradeBoard::setA(int a)
{
	A = a;

	update();
}

void UpgradeBoard::setS(int s)
{
	S = s;

	update();
}

void UpgradeBoard::setSS(int ss)
{
	SS = ss;

	update();
}

int UpgradeBoard::getD()
{
	return D;
}

int UpgradeBoard::getC()
{
	return C;
}

int UpgradeBoard::getB()
{
	return B;
}

int UpgradeBoard::getA()
{
	return A;
}

int UpgradeBoard::getS()
{
	return S;
}

int UpgradeBoard::getSS()
{
	return SS;
}

UpgradeBoard* UpgradeBoard::create()
{
	UpgradeBoard* upgradeBoard = new UpgradeBoard();
	Size winSize = Director::getInstance()->getWinSize();

	if (upgradeBoard && upgradeBoard->initWithSpriteFrameName("upgrade_select_menu.png"))
	{
		upgradeBoard->autorelease();

		upgradeBoard->addNumberSprite(TAG_BOARD_D_01, Point(71, 102), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_D_02, Point(80, 102), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_C_01, Point(122, 102), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_C_02, Point(131, 102), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_B_01, Point(172, 102), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_B_02, Point(182, 102), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_A_01, Point(71, 42), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_A_02, Point(80, 42), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_S_01, Point(122, 42), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_S_02, Point(131, 42), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_SS_01, Point(172, 42), 0.7);
		upgradeBoard->addNumberSprite(TAG_BOARD_SS_02, Point(182, 42), 0.7);

		return upgradeBoard;
	}

	CC_SAFE_DELETE(upgradeBoard);

	return NULL;
}

void UpgradeBoard::addNumberSprite(int tag, Point position, float scale)
{
	Sprite* sprite = Sprite::createWithSpriteFrameName("number_0.png");
	sprite->setPosition(position);
	sprite->setScale(scale);
	sprite->setTag(tag);
	addChild(sprite);
}

void UpgradeBoard::changeNumber(int tag, int number)
{
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	char szFile[32] = { 0, };

	sprintf(szFile, "number_%d.png", number % 10);
	SpriteFrame* frame = frameCache->getSpriteFrameByName(szFile);
	((Sprite*)getChildByTag(tag))->setDisplayFrame(frame);
}

void UpgradeBoard::update()
{
	if (getD() < 100)
	{
		changeNumber(TAG_BOARD_D_01, (getD() / 10) % 10);
		changeNumber(TAG_BOARD_D_02, getD() % 10);
	}
	if (getC() < 100)
	{
		changeNumber(TAG_BOARD_C_01, (getC() / 10) % 10);
		changeNumber(TAG_BOARD_C_02, getC() % 10);
	}
	if (getB() < 100)
	{
		changeNumber(TAG_BOARD_B_01, (getB() / 10) % 10);
		changeNumber(TAG_BOARD_B_02, getB() % 10);
	}
	if (getB() < 100)
	{
		changeNumber(TAG_BOARD_A_01, (getA() / 10) % 10);
		changeNumber(TAG_BOARD_A_02, getA() % 10);
	}
	if (getB() < 100)
	{
		changeNumber(TAG_BOARD_S_01, (getS() / 10) % 10);
		changeNumber(TAG_BOARD_S_02, getS() % 10);
	}
	if (getB() < 100)
	{
		changeNumber(TAG_BOARD_SS_01, (getSS() / 10) % 10);
		changeNumber(TAG_BOARD_SS_02, getSS() % 10);
	}
}
