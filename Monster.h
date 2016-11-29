#ifndef __DEFENCE_MONSTER_H__
#define __DEFENCE_MONSTER_H__

#include "cocos2d.h"
#include "Order.h"

class Monster
{
public:
	Monster();
	virtual ~Monster();

	void setEnergy(float max);
	void setBody(char* name);
	void setDefence(float def);
	void setNum(int index);
	void setBoss(float energy);
	float getEnergy();
	float getDefence();
	int getNum();

	cocos2d::Sprite* getBody();
	float subEnergy(float damage);
	void release();

private:
	cocos2d::Sprite* body;	//이미지
	cocos2d::Sprite* hp_bar;	//hp 뒷배경
	cocos2d::Sprite* hp;	//실제 hp 이미지

	float maxEnergy;		//최대 체력
	float curEnergy;		//현재 체력
	float defence;			//방어력
	int num;
};

#endif