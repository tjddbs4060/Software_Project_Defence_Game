#ifndef __MIX_HERO_H__
#define __MIX_HERO_H__

#include "cocos2d.h"
#include "Order.h"

class Mix_hero
{
public:
	Mix_hero();
	virtual ~Mix_hero();

	void set_result_hero(char* name);
	void set_mat_hero(char* name, int index);
	void set_result_hero_count(int count);
	void set_result_hero_type(char* type);
	void set_result_hero_name(char* name);
	void set_mat_hero_count(int count, int index);
	void set_mat_hero_type(char* type, int index);
	void set_mat_hero_name(char* name, int index);
	void set_hero_type(char* type);
	void set_count(int count);

	cocos2d::Sprite* get_result_hero();
	cocos2d::Sprite* get_mat_hero(int index);
	cocos2d::Sprite* get_hero_type();
	int get_result_hero_count();
	char* get_result_hero_type();
	char* get_result_hero_name();
	int get_mat_hero_count(int index);
	char* get_mat_hero_type(int index);
	char* get_mat_hero_name(int index);
	int get_count();

	void release();
private:
	cocos2d::Sprite* result_hero;	//조합된 영웅
	cocos2d::Sprite* mat_hero[3];	//조합 재료
	cocos2d::Sprite* hero_type;		//조합된 영웅 등급

	int result_hero_count;
	char result_hero_type[2];
	char result_hero_name[64];
	int mat_hero_count[3];
	char mat_hero_type[2][3];
	char mat_hero_name[64][3];
	int mix_count;
};

#endif