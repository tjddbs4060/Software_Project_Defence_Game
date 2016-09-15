#include "Monster.h"

Monster::Monster() : body(NULL), maxEnergy(100), curEnergy(0), defence(0), mGold(0)
{
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
	if (curEnergy <= 0)
		curEnergy = 0;
	return curEnergy;
}

void Monster::setBody(char* name)
{
	body = cocos2d::Sprite::createWithSpriteFrameName(name);
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