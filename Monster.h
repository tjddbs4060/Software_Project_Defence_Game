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
	cocos2d::Sprite* body;	//�̹���
	cocos2d::Sprite* hp_bar;	//hp �޹��
	cocos2d::Sprite* hp;	//���� hp �̹���

	float maxEnergy;		//�ִ� ü��
	float curEnergy;		//���� ü��
	float defence;			//����
	int num;
};

#endif