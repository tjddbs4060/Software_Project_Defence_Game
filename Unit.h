#ifndef __DEFENCE_UNIT_H__
#define __DEFENCE_UNIT_H__

#include "cocos2d.h"
#include "Order.h"

class Unit
{
public:
	Unit();
	virtual ~Unit();

	void setSpeed(float spd);
	void setDamage(float dmg);
	void setRange(float ran);
	void setCurSpeed(float spd);	//현재 공격 쿨타임
	void setBody(char* name);
	void setRight(bool tf);
	
	float getMaxSpeed();
	float getCurSpeed();
	float getDamage();
	float getRange();
	bool getRight();
	cocos2d::Sprite* getBody();

	void release();

private:
	cocos2d::Sprite* body;	//이미지

	float max_speed;	//공격 속도
	float cur_speed;	//현재 공격 속도
	float damage;		//공격력
	float range;		//사거리
	int type;			//유닛 타입
	bool right;			//유닛 공격 방향
};

#endif