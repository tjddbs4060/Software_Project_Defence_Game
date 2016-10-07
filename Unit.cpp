#include "Unit.h"

Unit::Unit() : body(NULL), max_speed(1.f), cur_speed(0), range(50.f), type(0), damage(0), right(false)
{
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

void Unit::setBody(char* name)
{
	body = cocos2d::Sprite::createWithSpriteFrameName(name);
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
