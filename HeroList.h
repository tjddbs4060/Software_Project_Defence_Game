#ifndef __HERO_LIST_H__
#define __HERO_LIST_H__

#include "cocos2d.h"
#include "Order.h"

#define TAG_HERO_LIST_MAIN 8000
#define TAG_HERO_LIST_MAIN_TYPE 9100
#define TAG_HERO_LIST_MAIN_ATK 9200
#define TAG_HERO_LIST_MAIN_MAP 9000
#define TAG_HERO_LIST_MAIN_BOSS 9001
#define TAG_HERO_LIST_MAIN_HELP 9002

class HeroList : public cocos2d::Layer
{
public:
	HeroList();
	virtual ~HeroList();

	void setHero(cocos2d::Sprite* sprite);
	void setMap(char* name);
	void setBoss(char* name);
	void setHelp(char* name);
	void setType(char* name);
	void setAtk(int damage);
	void setCount(int num);

	cocos2d::Sprite* getHero();
	cocos2d::Sprite* getMap();
	cocos2d::Sprite* getBoss();
	cocos2d::Sprite* getHelp();
	char* getType();
	int getCount();
	int getAtk();
	
	void init(int size);
	void release();

private:
	cocos2d::Sprite* hero;
	cocos2d::Sprite* map;
	cocos2d::Sprite* boss;
	cocos2d::Sprite* help;
	char type[2];
	int count;
	int atk;
};

#endif