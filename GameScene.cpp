#include "GameScene.h"

#pragma execution_character_set("UTF-8")

USING_NS_CC;

Game::Game() : Game_Start(false), touch(false), touch_unit(false), touch_gamble(false), new_soul_1(false), new_soul_2(false), touch_soul(false),
skip(false), now_unit(NULL), summon_monster(0), anc_height(0), anc_width(0) {}

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

	Menu* menu = Menu::create(menuSoul, menuSkip, menuGamble, NULL);
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
	monster->setEnergy(100.f);		//Ã¼·Â
	monster->setDefence(0.f);
	monster->setGold(1);
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

	//2.5¸¦ °öÇÑ ÀÌÀ¯ = animate ÇÑ¹ø´ç 0.4ÃÊ°¡ °É¸²
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
	//Á¶ÇÕÇÏ¿© »ç¶óÁö´Â°Í
	//ÆÈ¾Æ¼­ »ç¶óÁö´Â°Í
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

void Game::addunit()
{
	//·£´ýÀ¸·Î ³ª¿Í¶ó
	addlabel("SSS±Þ ¿µ¿õ ¸ð½Ã¸¶ÄÉ¾Æ¸®¸¶¼Ä", 0, 0);

	Size winSize = Director::getInstance()->getWinSize();
	Point pt = getChildByTag(TAG_BACKGROUND)->getContentSize();
	float xpos = pt.x / 2;
	float ypos = pt.y / 2;
	Unit* unit = new Unit();

	unit->setBody("unit_left_atk_1.png");
	unit->setDamage(70.f);
	unit->setRange(100.f);
	unit->setSpeed(2.f);
	unit->getBody()->setPosition(Point(xpos, ypos));

	Sprite* sprite = Sprite::createWithSpriteFrameName("range.png");
	sprite->setAnchorPoint(Point(0.5, 0.5));
	sprite->setPosition(Point(unit->getBody()->getContentSize().width / 2, unit->getBody()->getContentSize().height / 2));
	sprite->setOpacity(70);
	sprite->setVisible(false);
	sprite->setScale(unit->getRange() / 50.f);		//¿ÖÀÎÁö ¸ð¸£°Ú´Âµ¥ 2¹è Â÷ÀÌ

	getChildByTag(TAG_BACKGROUND)->addChild(unit->getBody(), ZORDER_CHARACTER, TAG_CHARACTER);
	unit->getBody()->addChild(sprite, ZORDER_RANGE, TAG_RANGE);

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	arr_unit.push_back(unit);
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
						inforBoard->setGold(inforBoard->getGold() + monster->getGold());
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

	if (touch_soul == true || touch_gamble == true)
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
				addunit();

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

void Game::onTouchesMoved(const std::vector<Touch*>& touches, Event *event)
{
	if (touch_unit == true || touch_soul == true || touch_gamble == true)
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
		if (touch_gamble == true) break;

		touch_soul = true;
		new_soul_1 = false;
		new_soul_2 = false;

		getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_SOUL)->setVisible(true);
		break;
	case TAG_MENU_GAMBLE:
		if (touch_soul == true) break;

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
		sprintf(szFile, "%s ¼ÒÈ¯", name);
		label = Label::createWithSystemFont(szFile, "Arial", 10);
		break;
	case 1:
		sprintf(szFile, "%d °ñµå È¹µæ", gold);
		label = Label::createWithSystemFont(szFile, "Arial", 10);
		break;
	case 2:
		sprintf(szFile, "1 º¸¼® È¹µæ");
		label = Label::createWithSystemFont(szFile, "Arial", 10);
		break;
	case 3:
		sprintf(szFile, "½Ã¹Î %d È¹µæ", gold);
		label = Label::createWithSystemFont(szFile, "Arial", 10);
		break;
	case 4:
		sprintf(szFile, "°ñµå ºÎÁ·");
		label = Label::createWithSystemFont(szFile, "Arial", 10);
		break;
	case 5:
		sprintf(szFile, "µµ¹Ú ¼º°ø! %d °ñµå È¹µæ!", gold);
		label = Label::createWithSystemFont(szFile, "Arial", 10);
		break;
	}

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
