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
	void setBody(char* body_name);
	void setType(char* rank);
	void setRight(bool tf);
	void setCount(int num);
	
	float getMaxSpeed();
	float getCurSpeed();
	float getDamage();
	float getRange();
	char* getType();
	char* getName();
	bool getRight();
	int getCount();
	cocos2d::Sprite* getBody();

	void release();

private:
	cocos2d::Sprite* body;	//이미지

	float max_speed;	//공격 속도
	float cur_speed;	//현재 공격 속도
	float damage;		//공격력
	float range;		//사거리
	char type[2];			//유닛 타입
	char name[64];		//유닛 이름
	int count;			//유닛 번호
	bool right;			//유닛 공격 방향
};

#endif