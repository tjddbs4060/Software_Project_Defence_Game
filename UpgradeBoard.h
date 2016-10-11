#ifndef __UPGRADE_BOARD_H__
#define __UPGRADE_BOARD_H__

#include "cocos2d.h"
#include "Order.h"

#define TAG_BOARD_D_01 300
#define TAG_BOARD_D_02 301
#define TAG_BOARD_C_01 302
#define TAG_BOARD_C_02 303
#define TAG_BOARD_B_01 304
#define TAG_BOARD_B_02 305
#define TAG_BOARD_A_01 306
#define TAG_BOARD_A_02 307
#define TAG_BOARD_S_01 308
#define TAG_BOARD_S_02 309
#define TAG_BOARD_SS_01 310
#define TAG_BOARD_SS_02 311

class UpgradeBoard : public cocos2d::Sprite
{
public:
	UpgradeBoard();
	virtual ~UpgradeBoard();

	static UpgradeBoard* create();

	void setD(int d);
	void setC(int c);
	void setB(int b);
	void setA(int a);
	void setS(int s);
	void setSS(int ss);
	int getD();
	int getC();
	int getB();
	int getA();
	int getS();
	int getSS();

	void addNumberSprite(int tag, cocos2d::Point position, float scale);	//숫자 추가
	void changeNumber(int tag, int number);	//숫자 변경

	void update();

private:
	int D;
	int C;
	int B;
	int A;
	int S;
	int SS;
};

#endif