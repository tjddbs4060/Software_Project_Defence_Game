#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Order.h"
#include "Monster.h"
#include "Unit.h"
#include "InforBoard.h"

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
	void addunit(float dt);	//���� �߰�
	void addunit_mix(cocos2d::Point pt);	//�����Ͽ� ���� ����
	void addunit_sell(cocos2d::Point pt);	//�ȾƼ� ���� ����
	void addattack(Monster* monster);	//���� ���
	void unit_atk_monster(float dt);	//�������κ��� ���� ����
	void unit_atk_cooltime(float dt);	//������ ���� ���� ����
	void unit_atk_motion(Unit* unit);			//���� ���� ���
	void unit_range(Unit* unit);	//���� ���ݹ��� ���̱�

	void monster_location_init(cocos2d::Sprite* sprite);	//���� �̵� �⺻����
	void zorder_assort(float dt);		//zorder ����
	void screen_out();		//ȭ�� ��� Ȯ��

	void selfRemover(Node* sender);	//����
	void monsterRemover(Node* sender);	//���� ����
	void unitRemover(Node* sender);		//���� ����
	
	cocos2d::Point map_out_check(cocos2d::Point point);	//���� ������� Ȯ��
	Unit* touch_unit_check();	//������ Ŭ���ߴ��� ����

	float calDistance(cocos2d::Point from, cocos2d::Point to);	//�� �� ������ �Ÿ� ���

	std::vector<Monster*> arr_monster;	//���� �迭
	std::vector<Unit*> arr_unit;	//���� �迭
	std::vector<cocos2d::Sprite*> arr_location;	//���� ���� ��ġ

	bool touch;		//��ġ ����
	bool Game_Start;	//���� ���� ����
	bool touch_unit;	//������ ��ġ�Ͽ����� ����
	int summon_monster;	//���� ��ȯ ������
	float anc_height;	//��Ŀ����Ʈ�� �̵��� �� �ִ� �ִ� ����
	float anc_width;	//�ִ� ����
	cocos2d::Point touch_point;	//���� ��ġ����
};

#endif