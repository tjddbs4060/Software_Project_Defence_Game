#include "Unit.h"

Unit::Unit() : body(NULL), max_speed(1.f), cur_speed(0), range(50.f), damage(0), right(false)
{
	for (int i = 0; i < 2; i++)
		type[i] = 0;
}

Unit::~Unit()
{
	release();
}

void Unit::setSpeed(float spd)
{
	max_speed = cur_speed = spd;
}

void Unit::setDamage(float dmg)
{
	damage = dmg;
}

void Unit::setRange(float ran)
{
	range = ran;
}

void Unit::setCurSpeed(float spd)
{
	cur_speed = spd;
}

void Unit::setRight(bool tf)
{
	right = tf;
}

void Unit::setType(char* rank)
{
	strcpy(type, rank);
}

void Unit::setCount(int num)
{
	count = num;
}

float Unit::getMaxSpeed()
{
	return max_speed;
}

float Unit::getCurSpeed()
{
	return cur_speed;
}

float Unit::getDamage()
{
	return damage;
}

float Unit::getRange()
{
	return range;
}

bool Unit::getRight()
{
	return right;
}

char* Unit::getType()
{
	return type;
}

void Unit::setBody(char* body_name)
{
	char szFile[64] = { 0, };

	strcpy(name, body_name);
	sprintf(szFile, "%s.png", name);
	body = cocos2d::Sprite::createWithSpriteFrameName(szFile);
}

int Unit::getCount()
{
	return count;
}

cocos2d::Sprite* Unit::getBody()
{
	return body;
}

void Unit::release()
{
	if (NULL != body)
	{
		body->removeFromParentAndCleanup(true);
		body = NULL;
	}
}

char* Unit::getName()
{
	return name;
}
