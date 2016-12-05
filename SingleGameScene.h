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
	void addmonster(float dt);	//���� �߰�
	void addmonster_death(cocos2d::Point pt);	//���Ͱ� ������ ���� �ִϸ��̼�
	void addunit(char* sprite_name, char* name, char* type, int number, float speed, float range, float damage, bool help);		//���� �߰�
	void addunit_mix(cocos2d::Point pt);	//�����Ͽ� ���� ����
	void addunit_sell(cocos2d::Point pt);	//�ȾƼ� ���� ����
	void addattack(Monster* monster, float scale);	//���� ���
	void unit_atk_motion(Unit* unit, bool right);	//���� ���� ���
	void move_unit(Unit* unit, bool right);		//���� ������ ���
	void addlabel(char* name, int gold, int choice);	//�� ����
	void upgrade_update(char* up);		//���׷��̵� �� ���ݷ����� �ٲٱ�
	bool rand_cal(float per);		//Ȯ�� ����
	void mix_hero_init();	//����ǥ �ʱ�ȭ	/////////////
	void GameOver();	//���� ���� �� ����
	void update_hero_list();	//���� ��� ������Ʈ
	void create_boss(char* name, float hp, float def);	//���� ����
	void setID(char* name);
	char* getID();

	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);	//http ����
	void get_db_data(char * data, int port);		//http�� data ����

	void onMenu(cocos2d::Object* sender);	//�������̽� �޴�

	void monster_location_init(cocos2d::Sprite* sprite);	//���� �̵� �⺻����
	void zorder_assort(float dt);		//zorder ����
	void screen_out();		//ȭ�� ��� Ȯ��
	void add_unit_queue(float dt);	//���� ���� ť(ť�� ������ �ٷ� ����)
	void unit_atk_monster(float dt);	//�������κ��� ���� ����
	void unit_atk_cooltime(float dt);	//������ ���� ���� ����
	void atk_boss(float dt);		//���� ����

	void selfRemover(Node* sender);	//����
	void monsterRemover(Node* sender);	//���� ����
	void unitRemover(Node* sender);		//���� ����
	void allstop_motion(Node* sender);	//��� ����
	void labelRemover(Node* sender);	//�� ����
	void removeChild_background_boss(Node* sender);	//�ʿ��� ���� �� ������
	void removeChild_boss_background(Node* sender);	//�����濡�� ���� �� ��

	cocos2d::Point map_out_check(cocos2d::Point point);	//���� ������� Ȯ��
	void touch_unit_check();	//������ Ŭ���ߴ��� ����

	float calDistance(cocos2d::Point from, cocos2d::Point to);	//�� �� ������ �Ÿ� ���

	Monster* boss;

	std::vector<Monster*> arr_monster;	//���� �迭
	std::vector<Unit*> arr_unit;		//���� �� ���� �迭
	std::vector<Unit*> arr_boss_room_unit;	//���� �� ���� �迭	/////////////
	std::vector<HeroList*> arr_hero_list;		//���� ���	/////////////
	std::vector<Use_String*> arr_unit_queue;	//�����͸� ������ ���� ť
	std::vector<cocos2d::Sprite*> arr_location;	//���� ���� ��ġ
	std::vector<cocos2d::Sprite*> arr_label;	//������ �� �迭
	std::vector<Mix_hero*> arr_mix_hero;	//����ǥ ť	///////////////

	bool touch;		//��ġ ����
	bool touch_soul;	//�ù� �������̽� ��ġ ����
	bool touch_gamble;	//���� �������̽� ��ġ ����
	bool touch_upgrade;		//���׷��̵� �������̽� ��ġ ����
	bool touch_mix;			//���� ��ġ ����
	bool touch_capsule;		//�̱� ��ġ ����
	bool touch_hero;		//����� �޴� ��ġ ����
	bool touch_boss;		//���� �޴� ��ġ ����
	bool hero_menu_move;	//����� �޴� ��ũ�� ����
	bool Game_Start;	//���� ���� ����
	bool touch_unit;	//������ ��ġ�Ͽ����� ����
	bool new_soul_1;		//�߰��� �ù� Ȯ�� ����
	bool new_soul_2;		//�߰��� �ù� Ȯ�� ����
	bool skip;			//skip ����
	bool alive_boss;	//���� ���� ����
	int mix_list;	//����ǥ�� ��� ��
	int boss_stage;		//���� ��������
	int summon_monster;	//���� ��ȯ ������
	int monster_index;	//���� index
	float anc_height;	//��Ŀ����Ʈ�� �̵��� �� �ִ� �ִ� ����
	float anc_width;	//�ִ� ����
	float monster_hp_def[2];	//���� ������ hp, def
	char id[32];		//������ id
	int upgrade_count[6];		//��� ���׷��̵� ����

	cocos2d::Point touch_point;	//���� ��ġ����

	Unit* now_unit;	//���� ������ ����

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