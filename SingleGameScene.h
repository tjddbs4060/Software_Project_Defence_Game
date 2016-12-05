#ifndef __SINGLE_GAME_SCENE_H__
#define __SINGLE_GAME_SCENE_H__

#include "cocos2d.h"
#include "Order.h"
#include "Monster.h"
#include "Unit.h"
#include "InforBoard.h"
#include "SoulBoard.h"
#include "Gamble.h"
#include "Use_String.h"
#include "UpgradeBoard.h"
#include "Mix_hero.h"
#include "MixBoard.h"
#include "CapsuleBoard.h"
#include "HeroBoard.h"
#include "HeroList.h"
#include "GameOver.h"

#include <string>
#include <vector>
#include <math.h>

#include "network/HttpClient.h"
#include "network/HttpConnection-winrt.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/SocketIO.h"

#define DEFENCEJS 3000

class SingleGame : public cocos2d::Layer
{
public:
	SingleGame();
	virtual bool init();
	static cocos2d::Scene* scene(char* name);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;

	CREATE_FUNC(SingleGame);

private:
	void addmonster(float dt);	//몬스터 추가
	void addmonster_death(cocos2d::Point pt);	//몬스터가 죽으면 폭발 애니메이션
	void addunit(char* sprite_name, char* name, char* type, int number, float speed, float range, float damage, bool help);		//유닛 추가
	void addunit_mix(cocos2d::Point pt);	//조합하여 유닛 삭제
	void addunit_sell(cocos2d::Point pt);	//팔아서 유닛 삭제
	void addattack(Monster* monster, float scale);	//공격 모션
	void unit_atk_motion(Unit* unit, bool right);	//유닛 공격 모션
	void move_unit(Unit* unit, bool right);		//유닛 움직임 모션
	void addlabel(char* name, int gold, int choice);	//라벨 생성
	void upgrade_update(char* up);		//업그레이드 된 공격력으로 바꾸기
	bool rand_cal(float per);		//확률 계산기
	void mix_hero_init();	//조합표 초기화	/////////////
	void GameOver();	//게임 오버 시 실행
	void update_hero_list();	//영웅 목록 업데이트
	void create_boss(char* name, float hp, float def);	//보스 생성
	void setID(char* name);
	char* getID();

	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);	//http 연결
	void get_db_data(char * data, int port);		//http에 data 전송

	void onMenu(cocos2d::Object* sender);	//인터페이스 메뉴

	void monster_location_init(cocos2d::Sprite* sprite);	//몬스터 이동 기본설정
	void zorder_assort(float dt);		//zorder 정렬
	void screen_out();		//화면 벗어남 확인
	void add_unit_queue(float dt);	//유닛 생성 큐(큐에 들어오면 바로 생성)
	void unit_atk_monster(float dt);	//유닛으로부터 몬스터 공격
	void unit_atk_cooltime(float dt);	//유닛의 공격 가능 여부
	void atk_boss(float dt);		//보스 공격

	void selfRemover(Node* sender);	//삭제
	void monsterRemover(Node* sender);	//몬스터 삭제
	void unitRemover(Node* sender);		//유닛 삭제
	void allstop_motion(Node* sender);	//모션 멈춤
	void labelRemover(Node* sender);	//라벨 제거
	void removeChild_background_boss(Node* sender);	//맵에서 제거 후 보스방
	void removeChild_boss_background(Node* sender);	//보스방에서 제거 후 맵

	cocos2d::Point map_out_check(cocos2d::Point point);	//맵을 벗어나는지 확인
	void touch_unit_check();	//유닛을 클릭했는지 여부

	float calDistance(cocos2d::Point from, cocos2d::Point to);	//두 점 사이의 거리 계산

	Monster* boss;

	std::vector<Monster*> arr_monster;	//몬스터 배열
	std::vector<Unit*> arr_unit;		//나의 맵 유닛 배열
	std::vector<Unit*> arr_boss_room_unit;	//보스 방 유닛 배열	/////////////
	std::vector<HeroList*> arr_hero_list;		//유닛 목록	/////////////
	std::vector<Use_String*> arr_unit_queue;	//데이터를 가져온 유닛 큐
	std::vector<cocos2d::Sprite*> arr_location;	//몬스터 생성 위치
	std::vector<cocos2d::Sprite*> arr_label;	//생성된 라벨 배열
	std::vector<Mix_hero*> arr_mix_hero;	//조합표 큐	///////////////

	bool touch;		//터치 여부
	bool touch_soul;	//시민 인터페이스 터치 여부
	bool touch_gamble;	//도박 인터페이스 터치 여부
	bool touch_upgrade;		//업그레이드 인터페이스 터치 여부
	bool touch_mix;			//조합 터치 여부
	bool touch_capsule;		//뽑기 터치 여부
	bool touch_hero;		//히어로 메뉴 터치 여부
	bool touch_boss;		//보스 메뉴 터치 여부
	bool hero_menu_move;	//히어로 메뉴 스크롤 여부
	bool Game_Start;	//게임 시작 여부
	bool touch_unit;	//유닛을 터치하였는지 여부
	bool new_soul_1;		//추가된 시민 확인 여부
	bool new_soul_2;		//추가된 시민 확인 여부
	bool skip;			//skip 여부
	bool alive_boss;	//보스 생사 여부
	int mix_list;	//조합표의 목록 수
	int boss_stage;		//보스 스테이지
	int summon_monster;	//몬스터 소환 마리수
	int monster_index;	//몬스터 index
	float anc_height;	//앵커포인트의 이동할 수 있는 최대 높이
	float anc_width;	//최대 넓이
	float monster_hp_def[2];	//현재 몬스터의 hp, def
	char id[32];		//유저의 id
	int upgrade_count[6];		//등급 업그레이드 정도

	cocos2d::Point touch_point;	//시작 터치지점

	Unit* now_unit;	//현재 선택한 유닛

	void sound_interface_open();
	void sound_ok_button();
	void sound_gamble();
	void sound_upgrade();
	void sound_capsule();
	void sound_gameover();
	void sound_boss_create();
	void sound_boss_room_open();
	void sound_hero_mix();
	void sound_boss_delete();
	void sound_stage();
	void sound_unit_move();
};

#endif