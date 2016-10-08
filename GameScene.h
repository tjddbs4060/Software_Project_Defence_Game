#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Order.h"
#include "Monster.h"
#include "Unit.h"
#include "InforBoard.h"
#include "SoulBoard.h"
#include "Gamble.h"
#include "Use_String.h"

#include <string>

#include "network/HttpClient.h"
#include "network/HttpConnection-winrt.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/SocketIO.h"

class Game : public cocos2d::Layer
{
public:
	Game();
	virtual bool init();
	static cocos2d::Scene* scene();

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;

	CREATE_FUNC(Game);

private:
	void addmonster(float dt);	//���� �߰�
	void addmonster_death(cocos2d::Point pt);	//���Ͱ� ������ ���� �ִϸ��̼�
	void addunit(char* name, char* type, int number, float speed, float range, float damage);		//���� �߰�
	void addunit_mix(cocos2d::Point pt);	//�����Ͽ� ���� ����
	void addunit_sell(cocos2d::Point pt);	//�ȾƼ� ���� ����
	void addattack(Monster* monster);	//���� ���
	void unit_atk_monster(float dt);	//�������κ��� ���� ����
	void unit_atk_cooltime(float dt);	//������ ���� ���� ����
	void unit_atk_motion(Unit* unit, bool right);	//���� ���� ���
	void move_unit(Unit* unit, bool right);		//���� ������ ���
	void addlabel(char* name, int gold, int choice);	//�� ����
	bool rand_cal(float per);		//Ȯ�� ����

	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);	//http ����
	void get_db_data(char * data);		//http�� data ����

	void onMenu(cocos2d::Object* sender);	//�������̽� �޴�

	void monster_location_init(cocos2d::Sprite* sprite);	//���� �̵� �⺻����
	void zorder_assort(float dt);		//zorder ����
	void screen_out();		//ȭ�� ��� Ȯ��
	void add_unit_queue(float dt);	//���� ���� ť(ť�� ������ �ٷ� ����)

	void selfRemover(Node* sender);	//����
	void monsterRemover(Node* sender);	//���� ����
	void unitRemover(Node* sender);		//���� ����
	void allstop_motion(Node* sender);	//��� ����
	void labelRemover(Node* sender);	//�� ����
	
	cocos2d::Point map_out_check(cocos2d::Point point);	//���� ������� Ȯ��
	void touch_unit_check();	//������ Ŭ���ߴ��� ����

	float calDistance(cocos2d::Point from, cocos2d::Point to);	//�� �� ������ �Ÿ� ���
	
	std::vector<Monster*> arr_monster;	//���� �迭
	std::vector<Unit*> arr_unit;	//���� �迭
	std::vector<Use_String*> arr_unit_queue;	//�����͸� ������ ���� ť
	std::vector<cocos2d::Sprite*> arr_location;	//���� ���� ��ġ
	std::vector<cocos2d::Sprite*> arr_label;	//������ �� �迭

	bool touch;		//��ġ ����
	bool touch_soul;	//�ù� �������̽� ��ġ ����
	bool touch_gamble;	//���� �������̽� ��ġ ����
	bool Game_Start;	//���� ���� ����
	bool touch_unit;	//������ ��ġ�Ͽ����� ����
	bool new_soul_1;		//�߰��� �ù� Ȯ�� ����
	bool new_soul_2;		//�߰��� �ù� Ȯ�� ����
	bool skip;			//skip ����
	int summon_monster;	//���� ��ȯ ������
	int stage;
	float anc_height;	//��Ŀ����Ʈ�� �̵��� �� �ִ� �ִ� ����
	float anc_width;	//�ִ� ����
	cocos2d::Point touch_point;	//���� ��ġ����

	Unit* now_unit;	//���� ������ ����
};

#endif