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
	void setCurSpeed(float spd);	//���� ���� ��Ÿ��
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
	cocos2d::Sprite* body;	//�̹���

	float max_speed;	//���� �ӵ�
	float cur_speed;	//���� ���� �ӵ�
	float damage;		//���ݷ�
	float range;		//��Ÿ�
	int type;			//���� Ÿ��
	bool right;			//���� ���� ����
};

#endif