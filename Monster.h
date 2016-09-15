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
	void setGold(int gold);
	float getEnergy();
	float getDefence();
	int getGold();

	cocos2d::Sprite* getBody();
	float subEnergy(float damage);
	void release();

private:
	cocos2d::Sprite* body;	//이미지

	float maxEnergy;		//최대 체력
	float curEnergy;		//현재 체력
	float defence;			//방어력
	int mGold;
};

#endif