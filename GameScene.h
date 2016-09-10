#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Order.h"
#include "Monster.h"
#include "Unit.h"

class Game : public cocos2d::Layer
{
public:
	virtual bool init();
	static cocos2d::Scene* scene();

	CREATE_FUNC(Game);

private:
	void addmonster(float dt);	//몬스터 추가
	void addmonster_death(cocos2d::Point pt);	//몬스터가 죽으면 폭발 애니메이션
	void addunit(float dt);	//유닛 추가
	void addunit_mix(cocos2d::Point pt);	//조합하여 유닛 삭제
	void addunit_sell(cocos2d::Point pt);	//팔아서 유닛 삭제
	void addattack(Monster* monster);	//공격 모션
	void unit_atk_monster(float dt);	//유닛으로부터 몬스터 공격
	void unit_atk_cooltime(float dt);	//유닛의 공격 가능 여부
	void unit_atk_motion(Unit* unit);			//유닛 공격 모션

	void selfRemover(Node* sender);	//삭제
	void monsterRemover(Node* sender);	//몬스터 삭제
	void unitRemover(Node* sender);		//유닛 삭제]

	float calDistance(cocos2d::Point from, cocos2d::Point to);	//두 점 사이의 거리 계산

	std::vector<Monster*> arr_monster;	//몬스터 배열
	std::vector<Unit*> arr_unit;	//유닛 배열

	int cur_level;	//현재 단계
	int cur_leave;	//현재 몬스터 수
	int cur_monster;	//현재 레벨에서 나온 몬스터 수
};

#endif