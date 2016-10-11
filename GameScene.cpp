#include "GameScene.h"

#pragma execution_character_set("UTF-8")

USING_NS_CC;

Game::Game() : Game_Start(false), touch(false), touch_unit(false), touch_gamble(false), touch_upgrade(false), new_soul_1(false), new_soul_2(false), touch_soul(false),
skip(false), now_unit(NULL), summon_monster(0), anc_height(0), anc_width(0), stage(0), monster_index(0)
{
	for (int i = 0; i < 2; i++)
		monster_hp_def[i] = 0;

	for (int i = 0; i < 6; i++)
		upgrade_count[i] = 0;

	strcpy(id, "asdf");
}

Scene* Game::scene()
{
	auto scene = Scene::create();
	auto layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}
	srand((unsigned int)time(NULL));

	Size winSize = Director::getInstance()->getWinSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Plist.plist");
	SpriteBatchNode* spriteBatchNodeSurface = SpriteBatchNode::create("Plist.png");
	addChild(spriteBatchNodeSurface, ZORDER_UNIT, TAG_UNIT);

	Sprite* menuNormal = Sprite::createWithSpriteFrameName("Soul.png");
	MenuItemSprite* menuSoul = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(Game::onMenu, this));
	menuSoul->setTag(TAG_MENU_SOUL);
	menuSoul->setPosition(Point(-(winSize.width / 2) + 30, 30));

	menuNormal = Sprite::createWithSpriteFrameName("skip.png");
	MenuItemSprite* menuSkip = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(Game::onMenu, this));
	menuSkip->setTag(TAG_MENU_SKIP);
	menuSkip->setPosition(Point(-(winSize.width / 2) + 30, winSize.height / 4));

	Sprite* skip_back = Sprite::createWithSpriteFrameName("skip_background.png");
	skip_back->setVisible(false);
	skip_back->setTag(TAG_MENU_SKIP_BACK);
	skip_back->setPosition(menuNormal->getContentSize().width / 2, menuNormal->getContentSize().height / 2);

	RotateBy* rotate = RotateBy::create(2.f, 360);
	RepeatForever* repeat = RepeatForever::create(rotate);

	skip_back->runAction(repeat);
	menuSkip->addChild(skip_back);

	menuNormal = Sprite::createWithSpriteFrameName("Gamble.png");
	MenuItemSprite* menuGamble = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(Game::onMenu, this));
	menuGamble->setTag(TAG_MENU_GAMBLE);
	menuGamble->setPosition(Point(-(winSize.width / 2) + 30, -20));

	menuNormal = Sprite::createWithSpriteFrameName("upgrade.png");
	MenuItemSprite* menuUpgrade = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(Game::onMenu, this));
	menuUpgrade->setTag(TAG_MENU_UPGRADE);
	menuUpgrade->setPosition(Point(-(winSize.width / 2) + 30, -70));

	Menu* menu = Menu::create(menuSoul, menuSkip, menuGamble, menuUpgrade, NULL);
	addChild(menu, ZORDER_MENU, TAG_MENU);

	for (int i = 1; i <= 10; i++)
	{
		char szFile[32] = { 0, };

		sprintf(szFile, "soul_number_%d.png", i);
		Sprite* sprite = Sprite::createWithSpriteFrameName(szFile);
		sprite->setPosition(30, 30);
		sprite->setTag(TAG_SOUL_NUMBER + i);
		sprite->setVisible(false);
		menuSoul->addChild(sprite);
	}

	InforBoard* inforBoard = InforBoard::create();
	inforBoard->setTag(TAG_INFORBOARD);
	inforBoard->setAnchorPoint(Point(0.5, 1));
	inforBoard->setPosition(Point(winSize.width / 2, winSize.height));
	inforBoard->setScale(winSize.width / inforBoard->getContentSize().width);
	spriteBatchNodeSurface->addChild(inforBoard);

	SoulBoard* soulBoard = SoulBoard::create();
	soulBoard->setTag(TAG_INTERFACE_SOUL);
	soulBoard->setPosition(Point(winSize.width / 2, winSize.height / 2));
	soulBoard->setVisible(false);
	spriteBatchNodeSurface->addChild(soulBoard);

	UpgradeBoard* upgradeBoard = UpgradeBoard::create();
	upgradeBoard->setTag(TAG_INTERFACE_UPGRADE);
	upgradeBoard->setPosition(Point(winSize.width / 2, winSize.height / 2));
	upgradeBoard->setVisible(false);
	spriteBatchNodeSurface->addChild(upgradeBoard);

	Gamble* gamble = Gamble::create();
	gamble->setTag(TAG_INTERFACE_GAMBLE);
	gamble->setPosition(Point(winSize.width / 2, winSize.height / 2));
	gamble->setVisible(false);
	spriteBatchNodeSurface->addChild(gamble);

	Sprite* sprite_background = Sprite::createWithSpriteFrameName("background.png");
	sprite_background->setAnchorPoint(Point(0, 0));
	addChild(sprite_background, ZORDER_BACKGROUND, TAG_BACKGROUND);

	int i = sprite_background->getTag();

	monster_location_init(sprite_background);

	if (winSize.height < sprite_background->getContentSize().height)
		anc_height = sprite_background->getContentSize().height - winSize.height;

	if (winSize.width < sprite_background->getContentSize().width)
		anc_width = sprite_background->getContentSize().width - winSize.width;

	schedule(schedule_selector(Game::zorder_assort));
	//schedule(schedule_selector(Game::addmonster), 1.f);
	schedule(schedule_selector(Game::unit_atk_cooltime));
	schedule(schedule_selector(Game::unit_atk_monster));
	schedule(schedule_selector(Game::add_unit_queue));
	schedule(schedule_selector(Game::server_continue), 0.9f);
	//addunit(0.1f);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(Game::onTouchesBegan, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(Game::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(Game::onTouchesEnded, this);
	listener->onTouchesMoved = CC_CALLBACK_2(Game::onTouchesMoved, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	return true;
}

void Game::addmonster(float dt)
{
	int i = 0;
	Point move[4];
	Sprite* location = new Sprite();
	InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);

	for (std::vector<Sprite*>::iterator iter = arr_location.begin(); iter != arr_location.end(); iter++)
	{
		location = (Sprite*)*iter;
		move[i++] = location->getPosition();
		location = NULL;
	}
	summon_monster--;

	int monster_type = rand() % 3 + 1;
	char szFile[64] = { 0, };

	Size winSize = Director::getInstance()->getWinSize();
	Monster* monster = new Monster;

	sprintf(szFile, "monster_0%d_02.png", monster_type);
	monster->setBody(szFile);
	monster->setEnergy(monster_hp_def[0]);		//ü��
	monster->setDefence(monster_hp_def[1]);
	monster->setNum(monster_index++);
	monster->getBody()->setPosition(move[0]);

	getChildByTag(TAG_BACKGROUND)->addChild(monster->getBody(), ZORDER_MONSTER, TAG_MONSTER);

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation_down = Animation::create();
	auto animation_left = Animation::create();
	auto animation_right = Animation::create();
	auto animation_up = Animation::create();

	for (int i = 1; i < 5; i++)
	{
		sprintf(szFile, "monster_%02d_%02d.png", monster_type, i);
		animation_down->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation_down->setDelayPerUnit(0.1);

	for (int i = 5; i < 9; i++)
	{
		sprintf(szFile, "monster_%02d_%02d.png", monster_type, i);
		animation_right->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation_right->setDelayPerUnit(0.1);

	for (int i = 9; i < 13; i++)
	{
		sprintf(szFile, "monster_%02d_%02d.png", monster_type, i);
		animation_up->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation_up->setDelayPerUnit(0.1);

	for (int i = 13; i < 17; i++)
	{
		sprintf(szFile, "monster_%02d_%02d.png", monster_type, i);
		animation_left->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation_left->setDelayPerUnit(0.1);

	float w_speed = winSize.width * 0.005;
	float h_speed = winSize.height * 0.005;

	auto animate_down = Animate::create(animation_down);
	auto animate_left = Animate::create(animation_left);
	auto animate_right = Animate::create(animation_right);
	auto animate_up = Animate::create(animation_up);

	//2.5�� ���� ���� = animate �ѹ��� 0.4�ʰ� �ɸ�
	auto rep_down = Repeat::create(animate_down, h_speed * 2.5);
	auto rep_left = Repeat::create(animate_left, w_speed * 2.5);
	auto rep_right = Repeat::create(animate_right, w_speed * 2.5);
	auto rep_up = Repeat::create(animate_up, h_speed * 2.5);

	MoveTo* move_1 = MoveTo::create(h_speed, move[1]);
	MoveTo* move_2 = MoveTo::create(w_speed, move[2]);
	MoveTo* move_3 = MoveTo::create(h_speed, move[3]);
	MoveTo* move_4 = MoveTo::create(w_speed, move[0]);

	auto Spawn_down = Spawn::create(move_1, rep_down, NULL);
	auto Spawn_right = Spawn::create(move_2, rep_right, NULL);
	auto Spawn_up = Spawn::create(move_3, rep_up, NULL);
	auto Spawn_left = Spawn::create(move_4, rep_left, NULL);

	auto sequence = Sequence::create(Spawn_down, Spawn_right, Spawn_up, Spawn_left, NULL);
	auto rep = RepeatForever::create(sequence);

	monster->getBody()->runAction(rep);

	arr_monster.push_back(monster);

	inforBoard->setMonster(inforBoard->getMonster() + 1);

	sprintf(szFile, "add_monster/%s/%g/%g/%d", id, monster->getEnergy(), monster->getDefence(), monster->getNum());
	get_db_data(szFile);
}

void Game::selfRemover(Node* sender)
{
	sender->removeFromParentAndCleanup(true);
}

void Game::monsterRemover(Node* sender)
{
	Monster* monster = NULL;
	int tag = sender->getTag();
	if (TAG_MONSTER == tag)
	{
		for (std::vector<Monster*>::iterator iter = arr_monster.begin(); iter != arr_monster.end(); iter++)
		{
			monster = (Monster*)*iter;
			if (monster->getBody() == sender)
			{
				arr_monster.erase(iter);
				break;
			}
			monster = NULL;
		}
	}

	if (NULL != monster)
	{
		addmonster_death(sender->getPosition());
		monster->release();
		delete monster;
	}
}

void Game::addmonster_death(Point pt)
{
	char szFile[64] = { 0, };

	Sprite* sprite_death = Sprite::createWithSpriteFrameName("death_01.png");
	sprite_death->setPosition(pt);

	getChildByTag(TAG_BACKGROUND)->addChild(sprite_death, ZORDER_MONSTER_DEATH);

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	for (int i = 1; i < 4; i++)
	{
		sprintf(szFile, "death_%02d.png", i);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation->setDelayPerUnit(0.05f);

	Animate* animate = Animate::create(animation);
	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(Game::selfRemover, this));
	Sequence* sequence = Sequence::create(animate, callfunc, NULL);

	sprite_death->runAction(sequence);
}

void Game::unitRemover(Node* sender)
{
	//�����Ͽ� ������°�
	//�ȾƼ� ������°�
	Unit* unit = NULL;
	int tag = sender->getTag();
	if (TAG_MONSTER == tag)
	{
		for (std::vector<Unit*>::iterator iter = arr_unit.begin(); iter != arr_unit.end(); iter++)
		{
			unit = (Unit*)*iter;
			if (unit->getBody() == sender)
			{
				arr_unit.erase(iter);
				break;
			}
			unit = NULL;
		}
	}

	if (NULL != unit)
	{
		unit->release();
		delete unit;
	}
}

void Game::addunit(char* name, char* type, int number, float speed, float range, float damage)
{
	Size winSize = Director::getInstance()->getWinSize();
	char szFile[64] = { 0, };

	addlabel(name, 0, 0);

	Point pt = getChildByTag(TAG_BACKGROUND)->getContentSize();
	float xpos = pt.x / 2;
	float ypos = pt.y / 2;
	Unit* unit = new Unit();

	//name�� �̿��� �̹���
	unit->setBody("unit_left_atk_1.png");
	unit->setDamage(damage);
	unit->setRange(range);
	unit->setSpeed(speed);
	unit->setType(type);
	unit->getBody()->setPosition(Point(xpos, ypos));

	Sprite* sprite = Sprite::createWithSpriteFrameName("range.png");
	sprite->setAnchorPoint(Point(0.5, 0.5));
	sprite->setPosition(Point(unit->getBody()->getContentSize().width / 2, unit->getBody()->getContentSize().height / 2));
	sprite->setOpacity(70);
	sprite->setVisible(false);
	sprite->setScale(unit->getRange() / 50.f);		//������ �𸣰ڴµ� 2�� ����

	getChildByTag(TAG_BACKGROUND)->addChild(unit->getBody(), ZORDER_CHARACTER, TAG_CHARACTER);
	unit->getBody()->addChild(sprite, ZORDER_RANGE, TAG_RANGE);

	arr_unit.push_back(unit);

	upgrade_update(type);

	sprintf(szFile, "add_unit/%s/%g/%g/%s", id, unit->getDamage(), unit->getRange(), id);
	get_db_data(szFile);
}

void Game::addunit_mix(Point pt)
{
	
}

void Game::addunit_sell(Point pt)
{

}

float Game::calDistance(Point from, Point to)
{
	float x = from.x - to.x;
	float y = from.y - to.y;

	return sqrtf(pow(x, 2) + pow(y, 2));
}

void Game::addattack(Monster* monster)
{
	Sprite* sprite_bullet = Sprite::createWithSpriteFrameName("bullet.png");
	sprite_bullet->setPosition(Point(15, 15));

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	for (int i = 1; i < 4; i++)
	{
		animation->addSpriteFrame(frameCache->getSpriteFrameByName("bullet.png"));
	}
	animation->setDelayPerUnit(0.05f);

	Animate* animate = Animate::create(animation);
	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(Game::selfRemover, this));
	Sequence* sequence = Sequence::create(animate, callfunc, NULL);

	sprite_bullet->runAction(sequence);

	monster->getBody()->addChild(sprite_bullet, ZORDER_BULLET);
}

void Game::unit_atk_monster(float dt)
{
	InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);

	Size winSize = Director::getInstance()->getWinSize();
	Unit* unit = NULL;
	Monster* monster = NULL;
	bool right;

	for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
	{
		unit = (Unit*)*iterUnit;

		for (std::vector<Monster*>::iterator iterMonster = arr_monster.begin(); iterMonster != arr_monster.end(); iterMonster++)
		{
			monster = (Monster*)*iterMonster;

			if (calDistance(unit->getBody()->getPosition(), monster->getBody()->getPosition()) <= unit->getRange())
			{
				if (unit->getMaxSpeed() <= unit->getCurSpeed())
				{
					if (unit->getBody()->getPositionX() - monster->getBody()->getPositionX() > 0)
						right = false;
					else right = true;

					unit->setCurSpeed(0);
					unit_atk_motion(unit, right);
					addattack(monster);
					//addattack(unit->getBody()->getPosition(), monster->getBody()->getPosition(), unit);
					
					if (0 >= monster->subEnergy(unit->getDamage()))
					{
						char szFile[64] = { 0, };

						sprintf(szFile, "delete_monster/%d", monster->getNum());
						get_db_data(szFile);

						addmonster_death(monster->getBody()->getPosition());
						monster->release();
						delete monster;
						iterMonster = arr_monster.erase(iterMonster);

						inforBoard->setMonster(inforBoard->getMonster() - 1);

						if (iterMonster == arr_monster.end())
							break;
					}
					else
						break;
				}
			}
		}
	}
}

void Game::unit_atk_cooltime(float dt)
{
	Unit* unit = NULL;

	for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
	{
		unit = (Unit*)*iterUnit;

		unit->setCurSpeed(unit->getCurSpeed() + dt);
		if (unit->getCurSpeed() >= unit->getMaxSpeed())
			unit->setCurSpeed(unit->getMaxSpeed());
	}
}

void Game::unit_atk_motion(Unit* unit, bool right)
{
	char szFile[64] = { 0, };

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	if (right == true)
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(szFile, "unit_right_atk_%d.png", i);
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
		}
		animation->addSpriteFrame(frameCache->getSpriteFrameByName("unit_right_atk_1.png"));
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(szFile, "unit_left_atk_%d.png", i);
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
		}
		animation->addSpriteFrame(frameCache->getSpriteFrameByName("unit_left_atk_1.png"));
	}

	animation->setDelayPerUnit(unit->getMaxSpeed() * 0.05f);

	/*
	for (int i = 2; i < 4; i++)
	{
		sprintf(szFile, "unit_%02d.png", i);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation->addSpriteFrame(frameCache->getSpriteFrameByName("unit_01.png"));
	animation->setDelayPerUnit(unit->getMaxSpeed() * 0.1f);
	*/
	Animate* animate = Animate::create(animation);

	unit->getBody()->runAction(animate);
}

void Game::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
	touch = true;
	touch_point = touches[0]->getLocation();

	if (touch_soul == true || touch_gamble == true || touch_upgrade == true)
		return;

	touch_unit_check();

	if (now_unit != NULL)
	{
		touch_unit = true;
		now_unit->getBody()->stopAllActions();
		now_unit->getBody()->getChildByTag(TAG_RANGE)->setVisible(true);
		//unit_range(now_unit);
	}
	/*
	Touch* touch;
	Vec2 touchPoint;
	for (int index = 0; index < touches.size(); index++)
	{
	touch = touches[index];
	touchPoint = touch->getLocation();
	int touchIndex = touch->getID();
	}
	*/
}

void Game::onTouchesCancelled(const std::vector<Touch*>& touches, Event *event)
{
	touch = false;
	touch_unit = false;

	screen_out();
	/*
	Touch* touch;
	Vec2 touchPoint;
	for (int index = 0; index < touches.size(); index++)
	{
	touch = touches[index];
	touchPoint = touch->getLocation();
	int touchIndex = touch->getID();
	}
	*/
}

void Game::allstop_motion(Node* sender)
{
	sender->stopAllActions();
}

void Game::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	if (touch_unit == true)
	{
		SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
		Size winSize = Director::getInstance()->getWinSize();
		Point pt = getChildByTag(TAG_BACKGROUND)->getPosition();

		MoveTo* moveto = MoveTo::create(calDistance(now_unit->getBody()->getPosition(), touches[0]->getLocation() - pt) / 150.f, touches[0]->getLocation() - pt);
		CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(Game::allstop_motion, this));
		auto animation = Animation::create();

		if (now_unit->getBody()->getPositionX() - (touches[0]->getLocation().x - pt.x) > 0)
		{
			animation->addSpriteFrame(frameCache->getSpriteFrameByName("unit_left_atk_1.png"));
			now_unit->setRight(false);
		}
		else
		{
			animation->addSpriteFrame(frameCache->getSpriteFrameByName("unit_right_atk_1.png"));
			now_unit->setRight(true);
		}
		animation->setDelayPerUnit(0.0001f);

		Animate* animate = Animate::create(animation);
		Sequence* sequence = Sequence::create(moveto, animate, callfunc, NULL);

		move_unit(now_unit, now_unit->getRight());
		now_unit->getBody()->runAction(sequence);

		//now_unit->getBody()->getChildByTag(TAG_RANGE)->setVisible(false);
		//unit_range(now_unit);
		touch_unit = false;
		now_unit = NULL;
	}

	if (touch_soul == true)
	{
		SoulBoard* soulBoard = (SoulBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_SOUL);
		InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);
		
		float x = soulBoard->getPositionX() - (soulBoard->getContentSize().width / 2);
		float y = soulBoard->getPositionY() - (soulBoard->getContentSize().height / 2);

		Rect hero_down = Rect(x + 57, y + 95, 10, 10);
		Rect hero_up = Rect(x + 85, y + 95, 10, 10);
		Rect gold_down = Rect(x + 108, y + 95, 10, 10);
		Rect gold_up = Rect(x + 136, y + 95, 10, 10);
		Rect jewelry_down = Rect(x + 158, y + 95, 10, 10);
		Rect jewelry_up = Rect(x + 186, y + 95, 10, 10);
		Rect esc_button = Rect(x + 230, y + 184, 10, 10);
		Rect ok_button = Rect(x + 107, y + 50, 40, 12);

		if (hero_down.containsPoint(touches[0]->getLocation()))
		{
			if (soulBoard->getHero() > 0)
			{
				soulBoard->setHero(soulBoard->getHero() - 1);
				soulBoard->setSoul(soulBoard->getSoul() + 1);
				inforBoard->setSoul(inforBoard->getSoul() + 1);
			}
		}
		else if (hero_up.containsPoint(touches[0]->getLocation()))
		{
			if (soulBoard->getSoul() > 0)
			{
				soulBoard->setHero(soulBoard->getHero() + 1);
				soulBoard->setSoul(soulBoard->getSoul() - 1);
				inforBoard->setSoul(inforBoard->getSoul() - 1);
			}
		}
		else if (gold_down.containsPoint(touches[0]->getLocation()))
		{
			if (soulBoard->getGold() > 0)
			{
				soulBoard->setGold(soulBoard->getGold() - 1);
				soulBoard->setSoul(soulBoard->getSoul() + 1);
				inforBoard->setSoul(inforBoard->getSoul() + 1);
			}
		}
		else if (gold_up.containsPoint(touches[0]->getLocation()))
		{
			if (soulBoard->getSoul() > 0)
			{
				soulBoard->setGold(soulBoard->getGold() + 1);
				soulBoard->setSoul(soulBoard->getSoul() - 1);
				inforBoard->setSoul(inforBoard->getSoul() - 1);
			}
		}
		else if (jewelry_down.containsPoint(touches[0]->getLocation()))
		{
			if (soulBoard->getJewelry() > 0)
			{
				soulBoard->setJewelry(soulBoard->getJewelry() - 1);
				soulBoard->setSoul(soulBoard->getSoul() + 1);
				inforBoard->setSoul(inforBoard->getSoul() + 1);
			}
		}
		else if (jewelry_up.containsPoint(touches[0]->getLocation()))
		{
			if (soulBoard->getSoul() > 0)
			{
				soulBoard->setJewelry(soulBoard->getJewelry() + 1);
				soulBoard->setSoul(soulBoard->getSoul() - 1);
				inforBoard->setSoul(inforBoard->getSoul() - 1);
			}
		}
		else if (esc_button.containsPoint(touches[0]->getLocation()))
		{
			touch_soul = false;
			new_soul_1 = true;
			new_soul_2 = false;

			inforBoard->setSoul(soulBoard->getHero() + soulBoard->getGold() + soulBoard->getJewelry() + soulBoard->getSoul());

			soulBoard->setVisible(false);
			soulBoard->setHero(0);
			soulBoard->setGold(0);
			soulBoard->setJewelry(0);
			soulBoard->setSoul(inforBoard->getSoul());
		}
		else if (ok_button.containsPoint(touches[0]->getLocation()))
		{
			touch_soul = false;
			new_soul_1 = true;
			new_soul_2 = false;

			int gold_rand;

			for (int i = 0; i < soulBoard->getHero(); i++)
			{
				//hero/type/count
				char send_data[64] = { 0, };
				char * type;
				int type_rand;
				int count;

				if (stage < 15)
					type_rand = 0;
				else {
					type_rand = rand() % 5;
				}

				switch (type_rand) {
				case 0:
				case 1:
				case 2:
				case 3:
					type = "D";
					count = rand() % 10 + 1;
					break;
				case 4:
					type = "C";
					count = rand() % 12 + 1;
					break;
				}

				sprintf(send_data, "hero/%s/%d", type, count);
				get_db_data(send_data);
			}

			for (int i = 0; i < soulBoard->getGold(); i++)
			{
				gold_rand = rand() % 10 + 1;

				inforBoard->setGold(inforBoard->getGold() + gold_rand);
				addlabel(NULL, gold_rand, 1);
			}

			for (int i = 0; i < soulBoard->getJewelry(); i++)
			{
				if (rand_cal(0.5))
				{
					inforBoard->setJewelry(inforBoard->getJewelry() + 1);
					addlabel(NULL, 0, 2);
				}
			}

			inforBoard->setSoul(soulBoard->getSoul());

			soulBoard->setVisible(false);
			soulBoard->setHero(0);
			soulBoard->setGold(0);
			soulBoard->setJewelry(0);
			soulBoard->setSoul(inforBoard->getSoul());
		}

		soulBoard->updateNumber(soulBoard);
	}
	
	if (touch_gamble == true)
	{
		InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);
		Gamble* gamble = (Gamble*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_GAMBLE);

		int temp = 0;
		float x = gamble->getPositionX() - (gamble->getContentSize().width / 2);
		float y = gamble->getPositionY() - (gamble->getContentSize().height / 2);

		Rect low_button = Rect(x + 56, y + 83, 40, 12);
		Rect mid_button = Rect(x + 106, y + 83, 40, 12);
		Rect high_button = Rect(x + 156, y + 83, 40, 12);
		Rect esc_button = Rect(x + 230, y + 184, 10, 10);

		if (low_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - 10 >= 0)
			{
				temp = gamble->low_gamble();

				inforBoard->setGold(inforBoard->getGold() - 10);
				inforBoard->setGold(inforBoard->getGold() + temp);

				addlabel(NULL, temp, 5);
			}
			else addlabel(NULL, temp, 4);
		}
		else if (mid_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - 100 >= 0)
			{
				temp = gamble->middle_gamble();

				inforBoard->setGold(inforBoard->getGold() - 100);
				inforBoard->setGold(inforBoard->getGold() + temp);

				addlabel(NULL, temp, 5);
			}
			else addlabel(NULL, temp, 4);
		}
		else if (high_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - 500 >= 0)
			{
				temp = gamble->high_gamble();

				inforBoard->setGold(inforBoard->getGold() - 500);
				inforBoard->setGold(inforBoard->getGold() + temp);

				addlabel(NULL, temp, 5);
			}
			else addlabel(NULL, temp, 4);
		}
		else if (esc_button.containsPoint(touches[0]->getLocation()))
		{
			touch_gamble = false;

			gamble->setVisible(false);
		}
	}

	if (touch_upgrade == true)
	{
		InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);
		UpgradeBoard* upgrade = (UpgradeBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_UPGRADE);

		float x = upgrade->getPositionX() - (upgrade->getContentSize().width / 2);
		float y = upgrade->getPositionY() - (upgrade->getContentSize().height / 2);

		Rect D_button = Rect(x + 56, y + 110, 40, 40);
		Rect C_button = Rect(x + 106, y + 110, 40, 40);
		Rect B_button = Rect(x + 156, y + 110, 40, 40);
		Rect A_button = Rect(x + 56, y + 50, 40, 40);
		Rect S_button = Rect(x + 106, y + 50, 40, 40);
		Rect SS_button = Rect(x + 156, y + 50, 40, 40);
		Rect esc_button = Rect(x + 230, y + 184, 10, 10);

		if (D_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - (10 + upgrade_count[0]) >= 0)
			{
				upgrade_count[0]++;
				upgrade->setD(upgrade_count[0]);
				upgrade_update("D");
				inforBoard->setGold(inforBoard->getGold() - (10 + upgrade_count[0]));

				addlabel("D", 0, 6);
			}
			else addlabel(NULL, 0, 7);
		}
		else if (C_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - (20 + upgrade_count[1] * 2) >= 0)
			{
				upgrade_count[1]++;
				upgrade->setC(upgrade_count[1]);
				upgrade_update("C");
				inforBoard->setGold(inforBoard->getGold() - (20 + upgrade_count[1] * 2));

				addlabel("C", 0, 6);
			}
			else addlabel(NULL, 0, 7);
		}
		else if (B_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - (50 + upgrade_count[2] * 5) >= 0)
			{
				upgrade_count[2]++;
				upgrade->setB(upgrade_count[2]);
				upgrade_update("B");
				inforBoard->setGold(inforBoard->getGold() - (50 + upgrade_count[2] * 5));

				addlabel("B", 0, 6);
			}
			else addlabel(NULL, 0, 7);
		}
		else if (A_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - (100 + upgrade_count[2] * 10) >= 0)
			{
				upgrade_count[3]++;
				upgrade->setA(upgrade_count[3]);
				upgrade_update("A");
				inforBoard->setGold(inforBoard->getGold() - (100 + upgrade_count[2] * 10));

				addlabel("A", 0, 6);
			}
			else addlabel(NULL, 0, 7);
		}
		else if (S_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - (300 + upgrade_count[2] * 30) >= 0)
			{
				upgrade_count[4]++;
				upgrade->setS(upgrade_count[4]);
				upgrade_update("S");
				inforBoard->setGold(inforBoard->getGold() - (300 + upgrade_count[2] * 30));

				addlabel("S", 0, 6);
			}
			else addlabel(NULL, 0, 7);
		}
		else if (SS_button.containsPoint(touches[0]->getLocation()))
		{
			if (inforBoard->getGold() - (500 + upgrade_count[2] * 50) >= 0)
			{
				upgrade_count[5]++;
				upgrade->setSS(upgrade_count[5]);
				upgrade_update("SS");
				inforBoard->setGold(inforBoard->getGold() - (500 + upgrade_count[2] * 50));

				addlabel("SS", 0, 6);
			}
			else addlabel(NULL, 0, 7);
		}
		else if (esc_button.containsPoint(touches[0]->getLocation()))
		{
			touch_upgrade = false;

			upgrade->setVisible(false);
		}
	}
	
	touch = false;
	touch_unit = false;

	screen_out();
	/*
	Touch* touch;
	Vec2 touchPoint;
	for (int index = 0; index < touches.size(); index++)
	{
	touch = touches[index];
	touchPoint = touch->getLocation();
	int touchIndex = touch->getID();
	}
	*/
}

void Game::add_unit_queue(float dt)
{
	Use_String* queue = NULL;

	for (std::vector<Use_String*>::iterator iterQueue = arr_unit_queue.begin(); iterQueue != arr_unit_queue.end(); iterQueue++)
	{
		queue = (Use_String*)*iterQueue;

		char * rev = queue->getString();
		strtok(rev, "/");
		char * name = strtok(NULL, "/");
		char * type = strtok(NULL, "/");
		char * count = strtok(NULL, "/");
		char * speed = strtok(NULL, "/");
		char * range = strtok(NULL, "/");
		char * damage = strtok(NULL, "/");

		addunit(name, type, atoi(count), atof(speed), atof(range), atof(damage));
		
		delete queue;
		iterQueue = arr_unit_queue.erase(iterQueue);

		if (iterQueue == arr_unit_queue.end())
			break;
	}

	/*
	//String *str = new String(queue->subString(queue->getFindChar('/', 1) + 1, queue->getFindChar('/', 2) - 1));

	Use_String *sub_name = new Use_String();
	Use_String *sub_type = new Use_String();
	Use_String *sub_count = new Use_String();
	Use_String *sub_speed = new Use_String();
	Use_String *sub_range = new Use_String();
	Use_String *sub_damage = new Use_String();

	queue->subString(queue->getFindChar('/', 1) + 1, queue->getFindChar('/', 2) - 1);

	char name[64] = { 0, };
	char type[64] = { 0, };
	char count[64] = { 0, };
	char speed[64] = { 0, };
	char range[64] = { 0, };
	char damgage[64] = { 0, };

	name[0] = queue->Index_char(queue->getFindChar('/', 1) + 1);

	sub_name->setString(name);
	sub_type->setString("D");
	sub_count->setString("8");
	sub_speed->setString("2.00");
	sub_range->setString("100");
	sub_damage->setString("80");
	*/
	/*
	sub_name->setString(queue->subString(queue->getFindChar('/', 1) + 1, queue->getFindChar('/', 2) - 1));
	sub_type->setString(queue->subString(queue->getFindChar('/', 2) + 1, queue->getFindChar('/', 3) - 1));
	sub_count->setString(queue->subString(queue->getFindChar('/', 3) + 1, queue->getFindChar('/', 4) - 1));
	sub_speed->setString(queue->subString(queue->getFindChar('/', 4) + 1, queue->getFindChar('/', 5) - 1));
	sub_range->setString(queue->subString(queue->getFindChar('/', 5) + 1, queue->getFindChar('/', 6) - 1));
	sub_damage->setString(queue->subString(queue->getFindChar('/', 6) + 1, queue->getLength() - 1));
	*/

	//addunit(sub_name->getString(), sub_type->getString(), sub_count->Convert_int(), sub_speed->Conver_float(), sub_range->Conver_float(), sub_damage->Conver_float());

}

void Game::onTouchesMoved(const std::vector<Touch*>& touches, Event *event)
{
	if (touch_unit == true || touch_soul == true || touch_gamble == true || touch_upgrade == true)
		return;

	Point movePoint = touches[0]->getLocation();
	Point distance = touch_point - movePoint;
	Point destination = getChildByTag(TAG_BACKGROUND)->getPosition() - distance;

	destination = map_out_check(destination);

	touch_point = movePoint;
	/*
	if (destination.x <= -anc_width && destination.y <= -anc_height)
		return;

	if (destination.x >= 0 && destination.y >= 0)
		return;
	*/

	MoveTo* moveto = MoveTo::create(0.f, destination);

	getChildByTag(TAG_BACKGROUND)->runAction(moveto);

	/*
	Touch* touch;
	Vec2 touchPoint;
	for (int index = 0; index < touches.size(); index++)
	{
	touch = touches[index];
	touchPoint = touch->getLocation();
	int touchIndex = touch->getID();
	}
	*/
}

Point Game::map_out_check(Point point)
{
	Point temp = point;

	if (temp.x <= -anc_width)
		temp.setPoint(-anc_width, temp.y);
	if (temp.y <= -anc_height)
		temp.setPoint(temp.x, -anc_height);
	if (temp.x >= 0.f)
		temp.setPoint(0, temp.y);
	if (temp.y >= 0.f)
		temp.setPoint(temp.x, 0);

	return temp;
}

void Game::monster_location_init(Sprite* sprite)
{
	Point location_1a = Point(sprite->getContentSize().width * 0.068, sprite->getContentSize().height * 0.645);
	Point location_1b = Point(sprite->getContentSize().width * 0.163, sprite->getContentSize().height * 0.625);
	Point location_2a = Point(sprite->getContentSize().width * 0.36, sprite->getContentSize().height * 0.07);
	Point location_2b = Point(sprite->getContentSize().width * 0.387, sprite->getContentSize().height * 0.1575);
	Point location_3a = Point(sprite->getContentSize().width * 0.875, sprite->getContentSize().height * 0.325);
	Point location_3b = Point(sprite->getContentSize().width * 0.9, sprite->getContentSize().height * 0.4125);
	Point location_4a = Point(sprite->getContentSize().width * 0.6, sprite->getContentSize().height * 0.9125);
	Point location_4b = Point(sprite->getContentSize().width * 0.68, sprite->getContentSize().height * 0.875);

	Sprite* sprite_monster_location_1 = Sprite::createWithSpriteFrameName("bullet.png");
	Sprite* sprite_monster_location_2 = Sprite::createWithSpriteFrameName("bullet.png");
	Sprite* sprite_monster_location_3 = Sprite::createWithSpriteFrameName("bullet.png");
	Sprite* sprite_monster_location_4 = Sprite::createWithSpriteFrameName("bullet.png");

	sprite_monster_location_1->setPosition(Point(location_1a));
	sprite_monster_location_2->setPosition(Point(location_2a));
	sprite_monster_location_3->setPosition(Point(location_3a));
	sprite_monster_location_4->setPosition(Point(location_4a));

	sprite_monster_location_1->setVisible(false);
	sprite_monster_location_2->setVisible(false);
	sprite_monster_location_3->setVisible(false);
	sprite_monster_location_4->setVisible(false);

	MoveTo* sprite_monster_location_a = MoveTo::create(1.1f, location_1b);
	MoveTo* sprite_monster_location_b = MoveTo::create(1.1f, location_2b);
	MoveTo* sprite_monster_location_c = MoveTo::create(1.1f, location_3b);
	MoveTo* sprite_monster_location_d = MoveTo::create(1.1f, location_4b);

	MoveTo* sprite_monster_location_aa = MoveTo::create(1.1f, location_1a);
	MoveTo* sprite_monster_location_bb = MoveTo::create(1.1f, location_2a);
	MoveTo* sprite_monster_location_cc = MoveTo::create(1.1f, location_3a);
	MoveTo* sprite_monster_location_dd = MoveTo::create(1.1f, location_4a);

	Sequence* sequence_a = Sequence::create(sprite_monster_location_a, sprite_monster_location_aa, NULL);
	Sequence* sequence_b = Sequence::create(sprite_monster_location_b, sprite_monster_location_bb, NULL);
	Sequence* sequence_c = Sequence::create(sprite_monster_location_c, sprite_monster_location_cc, NULL);
	Sequence* sequence_d = Sequence::create(sprite_monster_location_d, sprite_monster_location_dd, NULL);

	auto repeat_a = RepeatForever::create(sequence_a);
	auto repeat_b = RepeatForever::create(sequence_b);
	auto repeat_c = RepeatForever::create(sequence_c);
	auto repeat_d = RepeatForever::create(sequence_d);

	sprite_monster_location_1->runAction(repeat_a);
	sprite_monster_location_2->runAction(repeat_b);
	sprite_monster_location_3->runAction(repeat_c);
	sprite_monster_location_4->runAction(repeat_d);

	addChild(sprite_monster_location_1);
	addChild(sprite_monster_location_2);
	addChild(sprite_monster_location_3);
	addChild(sprite_monster_location_4);

	arr_location.push_back(sprite_monster_location_1);
	arr_location.push_back(sprite_monster_location_2);
	arr_location.push_back(sprite_monster_location_3);
	arr_location.push_back(sprite_monster_location_4);
}

void Game::zorder_assort(float dt)
{
	Unit* unit = NULL;
	Monster* monster = NULL;

	for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
	{
		unit = (Unit*)*iterUnit;

		unit->getBody()->setZOrder(1000 - unit->getBody()->getPositionY());
	}

	for (std::vector<Monster*>::iterator iterMonster = arr_monster.begin(); iterMonster != arr_monster.end(); iterMonster++)
	{
		monster = (Monster*)*iterMonster;

		monster->getBody()->setZOrder(1000 - monster->getBody()->getPositionY());
	}

	InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);
	SoulBoard* soulBoard = (SoulBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_SOUL);
	
	inforBoard->setTime(inforBoard->getTime() - dt);

	if (summon_monster > 0 && !((int)inforBoard->getTime()))
	{
		if (summon_monster == 11)
		{
			char szFile[64];

			stage++;
			sprintf(szFile, "monster/%d", stage);
			get_db_data(szFile);
			summon_monster = 10;
			inforBoard->setSoul(inforBoard->getSoul() + 3);
			soulBoard->setSoul(inforBoard->getSoul());
			soulBoard->updateNumber(soulBoard);
			addlabel(NULL, 3, 3);
		}
		schedule(schedule_selector(Game::addmonster), 1.f);
		new_soul_1 = true;
		new_soul_2 = false;
	}
	else if (summon_monster == 0)
	{
		unschedule(schedule_selector(Game::addmonster));
		summon_monster = 11;

		if (skip == true)
			inforBoard->setTime(1);
	}

	if (inforBoard->getSoul() <= 0)
	{
		for (int i = 1; i <= 10; i++)
			getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SOUL)->getChildByTag(TAG_SOUL_NUMBER + i)->setVisible(false);

		return;
	}

	//int temp = inforBoard->getSoul() + soulBoard->getHero() + soulBoard->getGold() + soulBoard->getJewelry() + soulBoard->getSoul();
	int temp = soulBoard->getHero() + soulBoard->getGold() + soulBoard->getJewelry() + soulBoard->getSoul();

	if (new_soul_1 == true)
	{
		if (new_soul_2 == false)
		{
			new_soul_2 = true;

			for (int i = 1; i <= 10; i++)
				getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SOUL)->getChildByTag(TAG_SOUL_NUMBER + i)->setVisible(false);

			if (temp < 10)
				getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SOUL)->getChildByTag(TAG_SOUL_NUMBER + temp)->setVisible(true);
			else getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SOUL)->getChildByTag(TAG_SOUL_NUMBER + 10)->setVisible(true);
		}
	}
	else
	{
		for (int i = 1; i <= 10; i++)
			getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SOUL)->getChildByTag(TAG_SOUL_NUMBER + i)->setVisible(false);
	}
}

void Game::touch_unit_check()
{
	Size winSize = Director::getInstance()->getWinSize();
	Point pt = getChildByTag(TAG_BACKGROUND)->getPosition();

	Unit* unit = NULL;
	for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
	{
		unit = (Unit*)*iterUnit;

		Rect boundingBox = unit->getBody()->getBoundingBox();

		if (boundingBox.containsPoint(touch_point - pt))
		{
			now_unit = unit;
			return;
		}
	}

	return;
}

void Game::screen_out()
{
	getChildByTag(TAG_BACKGROUND)->setPosition(getChildByTag(TAG_BACKGROUND)->getPosition());
}

void Game::onMenu(Object* sender)
{
	InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);

	switch (((Node*)sender)->getTag())
	{
	case TAG_MENU_SOUL:
		if (touch_gamble == true || touch_upgrade == true) break;

		touch_soul = true;
		new_soul_1 = false;
		new_soul_2 = false;

		getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_SOUL)->setVisible(true);
		break;
	case TAG_MENU_GAMBLE:
		if (touch_soul == true || touch_upgrade == true) break;

		touch_gamble = true;

		getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_GAMBLE)->setVisible(true);
		break;
	case TAG_MENU_SKIP:
		if (skip == false)
		{
			skip = true;
			getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SKIP)->getChildByTag(TAG_MENU_SKIP_BACK)->setVisible(true);
			
			if (inforBoard->getTime() < 50 && inforBoard->getTime() > 1)
				inforBoard->setTime(1);
		}
		else
		{
			skip = false;
			getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SKIP)->getChildByTag(TAG_MENU_SKIP_BACK)->setVisible(false);
		}

		break;
	case TAG_MENU_UPGRADE:
		if (touch_soul == true || touch_gamble == true) break;

		touch_upgrade = true;

		getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_UPGRADE)->setVisible(true);
		break;
	}
}

void Game::move_unit(Unit* unit, bool right)
{
	char szFile[64] = { 0, };

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	if (right == true)
	{
		for (int i = 1; i < 10; i++)
		{
			sprintf(szFile, "unit_right_%d.png", i);
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
		}
		animation->addSpriteFrame(frameCache->getSpriteFrameByName("unit_right_atk_3.png"));
	}
	else
	{
		for (int i = 1; i < 10; i++)
		{
			sprintf(szFile, "unit_left_%d.png", i);
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
		}
		animation->addSpriteFrame(frameCache->getSpriteFrameByName("unit_left_atk_3.png"));
	}

	animation->setDelayPerUnit(unit->getMaxSpeed() * 0.02f);

	Animate* animate = Animate::create(animation);
	RepeatForever* repeat = RepeatForever::create(animate);

	unit->getBody()->runAction(repeat);
	unit->getBody()->getChildByTag(TAG_RANGE)->setVisible(false);
}

void Game::addlabel(char* name, int gold, int choice)
{
	Size winSize = Director::getInstance()->getWinSize();
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	Sprite* sprite = Sprite::createWithSpriteFrameName("Label.png");
	Label* label;
	char szFile[64] = { 0, };
	float height = arr_label.size() * 15;

	switch (choice)
	{
	case 0:
		sprintf(szFile, "%s ��ȯ", name);
		break;
	case 1:
		sprintf(szFile, "%d ��� ȹ��", gold);
		break;
	case 2:
		sprintf(szFile, "1 ���� ȹ��");
		break;
	case 3:
		sprintf(szFile, "�ù� %d ȹ��", gold);
		break;
	case 4:
		sprintf(szFile, "��� ����");
		break;
	case 5:
		sprintf(szFile, "���� ����! %d ��� ȹ��!", gold);
		break;
	case 6:
		sprintf(szFile, "%s ��� ���׷��̵�!", name);
		break;
	case 7:
		sprintf(szFile, "���׷��̵� ��� ����!");
		break;
	}
	label = Label::createWithSystemFont(szFile, "Arial", 10);

	sprite->setPosition(winSize.width / 2, winSize.height * 0.8 - height);
	sprite->setOpacity(100);

	label->setPosition(Point(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2));
	addChild(sprite, ZORDER_LABEL, TAG_LABEL);
	sprite->addChild(label, ZORDER_LABEL, TAG_LABEL);

	FadeOut* fadeout = FadeOut::create(1.5f);
	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(Game::labelRemover, this));
	Sequence* sequence = Sequence::create(fadeout, callfunc, NULL);

	sprite->runAction(sequence);

	arr_label.push_back(sprite);
}

void Game::labelRemover(Node* sender)
{
	Sprite* sprite = NULL;

	for (std::vector<Sprite*>::iterator iterLabel = arr_label.begin(); iterLabel != arr_label.end(); iterLabel++)
	{
		sprite = (Sprite*)*iterLabel;

		sprite->removeFromParentAndCleanup(true);
		delete sprite;
		iterLabel = arr_label.erase(iterLabel);

		break;
	}
}

bool Game::rand_cal(float per)
{
	float num = rand() % 10000;

	if (num / 10000 < per)
		return true;
	else return false;
}

void Game::onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response)
{
	std::vector<char> * buffer = response->getResponseData();

	Use_String * use_string = new Use_String();
	char szFile[64] = { 0, };
	char temp[64] = { 0, };
	char * compare;
	for (unsigned int i = 0; i < buffer->size(); i++)
		szFile[i] = (*buffer)[i];

	if (buffer->size() == 0)
		return;

	strcpy(temp, szFile);
	compare = strtok(temp, "/");

	if (!strcmp(compare, "hero"))
	{
		use_string->setString(szFile);
		arr_unit_queue.push_back(use_string);
	}
	else if (!strcmp(compare, "monster"))
	{
		strtok(szFile, "/");
		char * hp = strtok(NULL, "/");
		char * def = strtok(NULL, "/");

		monster_hp_def[0] = atof(hp);
		monster_hp_def[1] = atof(def);
	}

	/*
	printf("Response Code : %li \n", response->getResponseCode());

	if (200 == response->getResponseCode())
	printf("Successful");
	else
	printf("Failed");
	*/
}

void Game::get_db_data(char * data)
{
	__String * dataToSend = __String::create(data);

	cocos2d::network::HttpRequest * request = new cocos2d::network::HttpRequest();
	request->setUrl("localhost:3000");
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setRequestData(dataToSend->getCString(), dataToSend->length());
	request->setResponseCallback(CC_CALLBACK_2(Game::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void Game::server_continue(float dt)
{
	InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);

	char szFile[64] = { 0, };

	sprintf(szFile, "time/%.2f/%s", inforBoard->getTime(), id);
	get_db_data(szFile);
}

void Game::upgrade_update(char* up)
{
	float rate;
	float damage;
	int index;

	Unit* unit = NULL;

	if (!strcmp(up, "D"))
	{
		rate = 1.1;
		damage = 100;
		index = 0;
	}
	else if (!strcmp(up, "C"))
	{
		rate = 1.15;
		damage = 350;
		index = 1;
	}
	else if (!strcmp(up, "B"))
	{
		rate = 1.15;
		damage = 1000;
		index = 2;
	}
	else if (!strcmp(up, "A"))
	{
		rate = 1.18;
		damage = 2500;
		index = 3;
	}
	else if (!strcmp(up, "S"))
	{
		rate = 1.2;
		damage = 7000;
		index = 4;
	}
	else if (!strcmp(up, "SS"))
	{
		rate = 1.2;
		damage = 20000;
		index = 5;
	}

	for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
	{
		unit = (Unit*)*iterUnit;

		if (!strcmp(unit->getType(), up))
			unit->setDamage(damage * pow(rate, upgrade_count[index]));
	}
}