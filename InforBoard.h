#ifndef __DEFENCE_INFOR_H__
#define __DEFENCE_INFOR_H__

#include "cocos2d.h"
#include "Order.h"

#define TAG_BOARD_TIME_01 1
#define TAG_BOARD_TIME_02 2
#define TAG_BOARD_GOLD 100
#define TAG_BOARD_JEWELRY 200
#define TAG_BOARD_LEVEL 300
#define TAG_BOARD_MONSTER 400
#define TAG_BOARD_STAGE 500

class InforBoard : public cocos2d::Sprite
{
public:
	InforBoard();
	virtual ~InforBoard();

	static InforBoard* create();

	void setTime(float time);
	void setGold(int gold);
	void setJewelry(int jewelry);
	void setLevel(int level);
	void setMonster(int monster);
	void setSoul(int soul);
	void setStage(int stage);
	float getTime();
	int getGold();
	int getJewelry();
	int getLevel();
	int getMonster();
	int getSoul();
	int getStage();

	void addNumberSprite(int tag, cocos2d::Point position, float scale);	//숫자 추가
	void changeNumber(int tag, int number);	//숫자 변경

	void update_time();
	void update_G_J(int tag, int number);
	void update_monster();
	void update_stage();

private:	
	float mTime;
	int mGold;
	int mJewelry;
	int mLevel;
	int mMonster;
	int mSoul;
	int mStage;
};

#endif