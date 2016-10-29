#ifndef __CAPSULE_BOARD_H__
#define __CAPSULE_BOARD_H__

#include "cocos2d.h"
#include "Order.h"

class CapsuleBoard : public cocos2d::Sprite
{
public:
	CapsuleBoard();
	~CapsuleBoard();

	static CapsuleBoard* create();

	void sprite_init();

	int D_gamble();
	int C_gamble();
	int B_gamble();
	int A_gamble();
	int S_gamble();
	int SS_gamble();

	cocos2d::Sprite* get_D_capsule();
	cocos2d::Sprite* get_C_capsule();
	cocos2d::Sprite* get_B_capsule();
	cocos2d::Sprite* get_A_capsule();
	cocos2d::Sprite* get_S_capsule();
	cocos2d::Sprite* get_SS_capsule();
	cocos2d::Sprite* get_esc();

private:
	cocos2d::Sprite* D_capsule;
	cocos2d::Sprite* C_capsule;
	cocos2d::Sprite* B_capsule;
	cocos2d::Sprite* A_capsule;
	cocos2d::Sprite* S_capsule;
	cocos2d::Sprite* SS_capsule;
	cocos2d::Sprite* esc;
};

#endif