#include "Monster.h"

Monster::Monster() : body(NULL), hp(NULL), maxEnergy(100), curEnergy(0), defence(0), mGold(0)
{
	hp_bar = cocos2d::Sprite::createWithSpriteFrameName("hp_background.png");
}

Monster::~Monster()
{
	release();
}

void Monster::release()
{
	if (NULL != body)
	{
		body->removeFromParentAndCleanup(true);
		body = NULL;
	}
}

void Monster::setEnergy(float max)
{
	maxEnergy = curEnergy = max;
}

void Monster::setDefence(float def)
{
	defence = def;
}

float Monster::subEnergy(float damage)
{
	curEnergy -= damage;

	float per = curEnergy / maxEnergy;

	if (hp != NULL) hp->removeFromParentAndCleanup(true);

	hp_bar->setVisible(true);
	hp = cocos2d::Sprite::create("hp.png", cocos2d::Rect(0, 0, 30 * per, 5));
	hp->setAnchorPoint(cocos2d::Point(0, 0.5));
	hp->setPosition(cocos2d::Point(1, hp_bar->getContentSize().height / 2));

	hp_bar->addChild(hp);

	if (curEnergy <= 0)
		curEnergy = 0;
	return curEnergy;
}

void Monster::setBody(char* name)
{
	body = cocos2d::Sprite::createWithSpriteFrameName(name);

	hp_bar->setPosition(cocos2d::Point(body->getContentSize().width / 2, -5));
	hp_bar->setVisible(false);

	body->addChild(hp_bar);
}

cocos2d::Sprite* Monster::getBody()
{
	return body;
}

float Monster::getEnergy()
{
	return curEnergy;
}

float Monster::getDefence()
{
	return defence;
}

void Monster::setGold(int gold)
{
	mGold = gold;
}

int Monster::getGold()
{
	return mGold;
}