#include "Mix_hero.h"

USING_NS_CC;

Mix_hero::Mix_hero():result_hero(NULL), result_hero_count(0)
{
	for (int i = 0; i < 5; i++)
	{
		mat_hero[i] = NULL;
		mat_hero_count[i] = 0;
		mat_hero_type[0][i] = NULL;
		mat_hero_name[0][i] = NULL;
	}
	result_hero_type[0] = 0;
	result_hero_type[1] = 0;
}

Mix_hero::~Mix_hero()
{
}

void Mix_hero::set_result_hero(char* name)
{
	result_hero = Sprite::createWithSpriteFrameName(name);
}

void Mix_hero::set_mat_hero(char* name, int index)
{
	mat_hero[index] = Sprite::createWithSpriteFrameName(name);
}

void Mix_hero::set_result_hero_count(int count)
{
	result_hero_count = count;
}

void Mix_hero::set_result_hero_type(char* type)
{
	strcpy(result_hero_type, type);
}

void Mix_hero::set_result_hero_name(char* name)
{
	strcpy(result_hero_name, name);
}

void Mix_hero::set_mat_hero_count(int count, int index)
{
	mat_hero_count[index] = count;
}

void Mix_hero::set_mat_hero_type(char* type, int index)
{
	strcpy(mat_hero_type[index], type);
}

void Mix_hero::set_mat_hero_name(char* name, int index)
{
	strcpy(mat_hero_name[index], name);
}

Sprite* Mix_hero::get_result_hero()
{
	return result_hero;
}

Sprite* Mix_hero::get_mat_hero(int index)
{
	return mat_hero[index];
}

int Mix_hero::get_result_hero_count()
{
	return result_hero_count;
}

char* Mix_hero::get_result_hero_type()
{
	return result_hero_type;
}

char* Mix_hero::get_result_hero_name()
{
	return result_hero_name;
}

int Mix_hero::get_mat_hero_count(int index)
{
	return mat_hero_count[index];
}

char* Mix_hero::get_mat_hero_type(int index)
{
	return mat_hero_type[index];
}

char* Mix_hero::get_mat_hero_name(int index)
{
	return mat_hero_name[index];
}

void Mix_hero::set_count(int count)
{
	mix_count = count;
}

int Mix_hero::get_count()
{
	return mix_count;
}

void Mix_hero::release()
{
	cocos2d::Sprite* result_hero;	//조합된 영웅
	cocos2d::Sprite* mat_hero[5];	//조합 재료

	if (NULL != result_hero)
	{
		result_hero->removeFromParentAndCleanup(true);
		result_hero = NULL;
	}

	for (int i = 0; i < 3; i++)
	{
		if (NULL != mat_hero[i])
		{
			mat_hero[i]->removeFromParentAndCleanup(true);
			mat_hero[i] = NULL;
		}
	}
}