#include "SingleGameScene.h"

#pragma execution_character_set("UTF-8")

USING_NS_CC;

SingleGame::SingleGame() : Game_Start(false), touch(false), touch_unit(false), touch_gamble(false), touch_upgrade(false), touch_mix(false), touch_capsule(false), touch_hero(false), touch_boss(false), hero_menu_move(false)
, new_soul_1(false), new_soul_2(false), touch_soul(false), skip(false), alive_boss(false), mix_list(52), now_unit(NULL), summon_monster(0), anc_height(0), anc_width(0), boss_stage(0), monster_index(0), atk_start_boss(false)
{
	for (int i = 0; i < 2; i++)
		monster_hp_def[i] = 0;

	for (int i = 0; i < 6; i++)
		upgrade_count[i] = 0;

	boss = NULL;
}

Scene* SingleGame::scene(char* name)
{
	auto scene = Scene::create();
	auto layer = SingleGame::create();
	layer->setID(name);
	scene->addChild(layer);

	return scene;
}

bool SingleGame::init()
{
	if (!Layer::init())
	{
		return false;
	}
	srand((unsigned int)time(NULL));
	mix_hero_init();
	
	CocosDenshion::SimpleAudioEngine* audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	if (audioEngine->isBackgroundMusicPlaying() == false)
		audioEngine->playBackgroundMusic("game_bgm.mp3", true);
		
	Size winSize = Director::getInstance()->getWinSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Thing.plist");
	SpriteBatchNode* spriteBatchNodeSurface = SpriteBatchNode::create("Thing.png");
	addChild(spriteBatchNodeSurface, ZORDER_UNIT, TAG_UNIT);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero.plist");
	SpriteBatchNode* spriteBatchNodeHero = SpriteBatchNode::create("Hero.png");
	addChild(spriteBatchNodeHero, ZORDER_HERO, TAG_HERO);

	Sprite* menuNormal = Sprite::createWithSpriteFrameName("Soul.png");
	MenuItemSprite* menuSoul = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(SingleGame::onMenu, this));
	menuSoul->setTag(TAG_MENU_SOUL);
	menuSoul->setPosition(Point(-(winSize.width / 2) + 30, 30));

	menuNormal = Sprite::createWithSpriteFrameName("skip.png");
	MenuItemSprite* menuSkip = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(SingleGame::onMenu, this));
	menuSkip->setTag(TAG_MENU_SKIP);
	menuSkip->setPosition(Point(-(winSize.width / 2) + 30, winSize.height / 4));

	Sprite* skip_back = Sprite::createWithSpriteFrameName("skip_background.png");
	skip_back->setVisible(false);
	skip_back->setTag(TAG_MENU_SKIP_BACK);
	skip_back->setPosition(menuNormal->getContentSize().width / 2, menuNormal->getContentSize().height / 2);

	FadeOut* fadeOut = FadeOut::create(1.0);
	FadeIn* fadeIn = FadeIn::create(1.0);
	Sequence* sequence = Sequence::create(fadeOut, fadeIn, NULL);

	RepeatForever* repeat = RepeatForever::create(sequence);

	skip_back->runAction(repeat);
	menuSkip->addChild(skip_back);

	menuNormal = Sprite::createWithSpriteFrameName("Gamble.png");
	MenuItemSprite* menuGamble = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(SingleGame::onMenu, this));
	menuGamble->setTag(TAG_MENU_GAMBLE);
	menuGamble->setPosition(Point(-(winSize.width / 2) + 30, -20));

	menuNormal = Sprite::createWithSpriteFrameName("upgrade.png");
	MenuItemSprite* menuUpgrade = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(SingleGame::onMenu, this));
	menuUpgrade->setTag(TAG_MENU_UPGRADE);
	menuUpgrade->setPosition(Point(-(winSize.width / 2) + 30, -70));

	menuNormal = Sprite::createWithSpriteFrameName("mix.png");
	MenuItemSprite* menuMix = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(SingleGame::onMenu, this));
	menuMix->setTag(TAG_MENU_MIX);
	menuMix->setPosition(Point((winSize.width / 2) - 30, -50));

	menuNormal = Sprite::createWithSpriteFrameName("capsule.png");
	MenuItemSprite* menuCapsule = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(SingleGame::onMenu, this));
	menuCapsule->setTag(TAG_MENU_CAPSULE);
	menuCapsule->setPosition(Point(-(winSize.width / 2) + 30, -120));

	menuNormal = Sprite::createWithSpriteFrameName("hero.png");
	MenuItemSprite* menuHero = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(SingleGame::onMenu, this));
	menuHero->setTag(TAG_MENU_HERO);
	menuHero->setPosition(Point((winSize.width / 2) - 30, -100));

	menuNormal = Sprite::createWithSpriteFrameName("boss_open.png");
	MenuItemSprite* menuBoss = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(SingleGame::onMenu, this));
	menuBoss->setTag(TAG_MENU_BOSS);
	menuBoss->setPosition(Point(winSize.width / 2, 30));
	menuBoss->setAnchorPoint(Point(1, 0));

	Menu* menu = Menu::create(menuSoul, menuSkip, menuGamble, menuUpgrade, menuMix, menuCapsule, menuHero, menuBoss, NULL);
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

	inforBoard->setGold(50000);//
	inforBoard->setJewelry(0);
	inforBoard->setMonster(0);
	inforBoard->setStage(0);

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

	CapsuleBoard* capsuleBoard = CapsuleBoard::create();
	capsuleBoard->setTag(TAG_INTERFACE_CAPSULE);
	capsuleBoard->setPosition(Point(winSize.width / 2, winSize.height / 2));
	capsuleBoard->setVisible(false);
	spriteBatchNodeSurface->addChild(capsuleBoard);

	MixBoard* mixBoard = MixBoard::create();
	mixBoard->setTag(TAG_INTERFACE_MIX);
	mixBoard->setPosition(Point(winSize.width / 2, winSize.height / 2));
	mixBoard->setVisible(false);
	spriteBatchNodeHero->addChild(mixBoard);

	HeroBoard* heroBoard = HeroBoard::create();
	heroBoard->setTag(TAG_INTERFACE_HERO);
	heroBoard->setPosition(Point(winSize.width / 2, winSize.height / 2));
	heroBoard->setVisible(false);
	spriteBatchNodeHero->addChild(heroBoard);
	
	Sprite* sprite_background = Sprite::createWithSpriteFrameName("background.png");
	sprite_background->setAnchorPoint(Point(0, 0));
	addChild(sprite_background, ZORDER_BACKGROUND, TAG_BACKGROUND);

	Sprite* sprite_boss_background = Sprite::createWithSpriteFrameName("boss_background.png");
	sprite_boss_background->setTag(TAG_INTERFACE_BOSS);
	sprite_boss_background->setPosition(Point(sprite_boss_background->getContentSize().width / 2 + getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getContentSize().width, getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getContentSize().height / 2));
	getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->addChild(sprite_boss_background);

	int i = sprite_background->getTag();

	monster_location_init(sprite_background);

	if (winSize.height < sprite_background->getContentSize().height)
		anc_height = sprite_background->getContentSize().height - winSize.height;

	if (winSize.width < sprite_background->getContentSize().width)
		anc_width = sprite_background->getContentSize().width - winSize.width;

	schedule(schedule_selector(SingleGame::zorder_assort));
	schedule(schedule_selector(SingleGame::unit_atk_cooltime));
	schedule(schedule_selector(SingleGame::unit_atk_monster));
	schedule(schedule_selector(SingleGame::add_unit_queue));
	schedule(schedule_selector(SingleGame::atk_boss));

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(SingleGame::onTouchesBegan, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(SingleGame::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(SingleGame::onTouchesEnded, this);
	listener->onTouchesMoved = CC_CALLBACK_2(SingleGame::onTouchesMoved, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	return true;
}

void SingleGame::addmonster(float dt)
{
	if (arr_monster.size() >= 50)
	{
		GameOver();

		return;
	}

	int i = 0;
	Point move[4];
	Sprite* location = new Sprite();
	InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);

	if (inforBoard->getStage() > 100)
	{
		//GameClear();
		GameOver();

		return;
	}

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
	monster->setEnergy(monster_hp_def[0]);		//체력
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

	//2.5를 곱한 이유 = animate 한번당 0.4초가 걸림
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

	//add_monster 새로운 디비로
}

void SingleGame::selfRemover(Node* sender)
{
	sender->removeFromParentAndCleanup(true);
}

void SingleGame::monsterRemover(Node* sender)
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
		addmonster_death(sender->getPosition(), 0.43, false);
		monster->release();
		delete monster;
	}
}

void SingleGame::addmonster_death(Point pt, float scale, bool isboss)
{
	char szFile[64] = { 0, };

	Sprite* sprite_death = Sprite::createWithSpriteFrameName("death_0.png");
	sprite_death->setPosition(pt);
	sprite_death->setScale(scale);

	if (isboss == false)
		getChildByTag(TAG_BACKGROUND)->addChild(sprite_death, ZORDER_MONSTER_DEATH);
	else getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->addChild(sprite_death, ZORDER_MONSTER_DEATH);

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	for (int i = 0; i < 8; i++)
	{
		sprintf(szFile, "death_%d.png", i);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation->setDelayPerUnit(0.05f);

	Animate* animate = Animate::create(animation);
	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(SingleGame::selfRemover, this));
	Sequence* sequence = Sequence::create(animate, callfunc, NULL);

	sprite_death->runAction(sequence);
}

void SingleGame::unitRemover(Node* sender)
{
	//조합하여 사라지는것
	//팔아서 사라지는것
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

void SingleGame::addunit(char* sprite_name, char* name, char* type, int number, float speed, float range, float damage, bool help = false)
{
	Size winSize = Director::getInstance()->getWinSize();
	char szFile[64] = { 0, };

	Point pt = getChildByTag(TAG_BACKGROUND)->getContentSize();
	float xpos = pt.x / 2;
	float ypos = pt.y / 2;

	if (help == true)
	{
		xpos = pt.x * 0.2;
		ypos = pt.y * 0.8;
	}

	Unit* unit = new Unit();

	//name을 이용한 이미지
	unit->setBody(sprite_name);
	unit->setDamage(damage);
	unit->setRange(range);
	unit->setSpeed(speed);
	unit->setCount(number);
	unit->setType(type);
	unit->getBody()->setPosition(Point(xpos, ypos));

	Sprite* sprite = Sprite::createWithSpriteFrameName("range.png");
	sprite->setAnchorPoint(Point(0.5, 0.5));
	sprite->setPosition(Point(unit->getBody()->getContentSize().width / 2, unit->getBody()->getContentSize().height / 2));
	sprite->setOpacity(70);
	sprite->setVisible(false);
	sprite->setScale(unit->getRange() / 50.f);		//왜인지 모르겠는데 2배 차이

	getChildByTag(TAG_BACKGROUND)->addChild(unit->getBody(), ZORDER_CHARACTER, TAG_CHARACTER);
	unit->getBody()->addChild(sprite, ZORDER_RANGE, TAG_RANGE);

	arr_unit.push_back(unit);

	upgrade_update(type);
	update_hero_list();

	//add_unit 있던 자리
}

void SingleGame::addunit_mix(Point pt)
{

}

void SingleGame::addunit_sell(Point pt)
{

}

float SingleGame::calDistance(Point from, Point to)
{
	float x = from.x - to.x;
	float y = from.y - to.y;

	return sqrtf(pow(x, 2) + pow(y, 2));
}

void SingleGame::addattack(Monster* monster, float scale)
{
	char szFile[64] = { 0, };

	Sprite* sprite_bullet = Sprite::createWithSpriteFrameName("bullet_0.png");
	sprite_bullet->setPosition(Point(15, 15));

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	for (int i = 0; i < 5; i++)
	{
		sprintf(szFile, "bullet_%d.png", i);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation->setDelayPerUnit(0.08f);

	Animate* animate = Animate::create(animation);
	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(SingleGame::selfRemover, this));
	Sequence* sequence = Sequence::create(animate, callfunc, NULL);

	sprite_bullet->runAction(sequence);

	monster->getBody()->addChild(sprite_bullet, ZORDER_BULLET);
}

void SingleGame::unit_atk_monster(float dt)
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
					addattack(monster, 0.43);
					//addattack(unit->getBody()->getPosition(), monster->getBody()->getPosition(), unit);

					if (0 >= monster->subEnergy(unit->getDamage()))
					{
						char szFile[64] = { 0, };

						//delete_monster 부분

						addmonster_death(monster->getBody()->getPosition(), 0.43, false);
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

void SingleGame::unit_atk_cooltime(float dt)
{
	Unit* unit = NULL;

	for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
	{
		unit = (Unit*)*iterUnit;

		unit->setCurSpeed(unit->getCurSpeed() + dt);
		if (unit->getCurSpeed() >= unit->getMaxSpeed())
			unit->setCurSpeed(unit->getMaxSpeed());
	}

	for (std::vector<Unit*>::iterator iterUnit = arr_boss_room_unit.begin(); iterUnit != arr_boss_room_unit.end(); iterUnit++)
	{
		unit = (Unit*)*iterUnit;

		unit->setCurSpeed(unit->getCurSpeed() + dt);
		if (unit->getCurSpeed() >= unit->getMaxSpeed())
			unit->setCurSpeed(unit->getMaxSpeed());
	}
}

void SingleGame::unit_atk_motion(Unit* unit, bool right)
{
	char szFile[64] = { 0, };

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	if (right == true)
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(szFile, "%s_right_atk_%d.png", unit->getName(), i);
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
		}
		sprintf(szFile, "%s.png", unit->getName());
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(szFile, "%s_left_atk_%d.png", unit->getName(), i);
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
		}
		sprintf(szFile, "%s.png", unit->getName());
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
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

void SingleGame::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
	touch = true;
	touch_point = touches[0]->getLocation();

	if (touch_soul == true || touch_gamble == true || touch_upgrade == true || touch_mix == true || touch_capsule == true || touch_hero == true)
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

void SingleGame::onTouchesCancelled(const std::vector<Touch*>& touches, Event *event)
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

void SingleGame::allstop_motion(Node* sender)
{
	sender->stopAllActions();
}

void SingleGame::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	if (touch_unit == true)
	{
		SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
		Size winSize = Director::getInstance()->getWinSize();
		Point pt = getChildByTag(TAG_BACKGROUND)->getPosition();

		MoveTo* moveto = MoveTo::create(calDistance(now_unit->getBody()->getPosition(), touches[0]->getLocation() - pt) / 150.f, touches[0]->getLocation() - pt);
		CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(SingleGame::allstop_motion, this));
		auto animation = Animation::create();

		char szFile[64];

		sprintf(szFile, "%s.png", now_unit->getName());

		if (now_unit->getBody()->getPositionX() - (touches[0]->getLocation().x - pt.x) > 0)
		{
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
			now_unit->setRight(false);
		}
		else
		{
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
			now_unit->setRight(true);
		}
		animation->setDelayPerUnit(0.0001f);

		Animate* animate = Animate::create(animation);
		Sequence* sequence = Sequence::create(moveto, animate, callfunc, NULL);

		move_unit(now_unit, now_unit->getRight());
		now_unit->getBody()->runAction(sequence);
		now_unit->setCurSpeed(now_unit->getMaxSpeed() - (calDistance(now_unit->getBody()->getPosition(), touches[0]->getLocation() - pt) / 150.f));
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
		Point pt = Point(x, y);

		if (soulBoard->get_hero_down()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (soulBoard->getHero() > 0)
			{
				soulBoard->setHero(soulBoard->getHero() - 1);
				soulBoard->setSoul(soulBoard->getSoul() + 1);
				inforBoard->setSoul(inforBoard->getSoul() + 1);
			}
		}
		else if (soulBoard->get_hero_up()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (soulBoard->getSoul() > 0)
			{
				soulBoard->setHero(soulBoard->getHero() + 1);
				soulBoard->setSoul(soulBoard->getSoul() - 1);
				inforBoard->setSoul(inforBoard->getSoul() - 1);
			}
		}
		else if (soulBoard->get_gold_down()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (soulBoard->getGold() > 0)
			{
				soulBoard->setGold(soulBoard->getGold() - 1);
				soulBoard->setSoul(soulBoard->getSoul() + 1);
				inforBoard->setSoul(inforBoard->getSoul() + 1);
			}
		}
		else if (soulBoard->get_gold_up()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (soulBoard->getSoul() > 0)
			{
				soulBoard->setGold(soulBoard->getGold() + 1);
				soulBoard->setSoul(soulBoard->getSoul() - 1);
				inforBoard->setSoul(inforBoard->getSoul() - 1);
			}
		}
		else if (soulBoard->get_jewelry_down()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (soulBoard->getJewelry() > 0)
			{
				soulBoard->setJewelry(soulBoard->getJewelry() - 1);
				soulBoard->setSoul(soulBoard->getSoul() + 1);
				inforBoard->setSoul(inforBoard->getSoul() + 1);
			}
		}
		else if (soulBoard->get_jewelry_up()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (soulBoard->getSoul() > 0)
			{
				soulBoard->setJewelry(soulBoard->getJewelry() + 1);
				soulBoard->setSoul(soulBoard->getSoul() - 1);
				inforBoard->setSoul(inforBoard->getSoul() - 1);
			}
		}
		else if (soulBoard->get_esc()->boundingBox().containsPoint(touches[0]->getLocation() - pt) || !soulBoard->boundingBox().containsPoint(touches[0]->getLocation()))
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
		else if (soulBoard->get_ok()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			sound_ok_button();

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

				if (inforBoard->getStage() < 15)
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
				get_db_data(send_data, DEFENCEJS);
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
		Point pt = Point(x, y);

		if (gamble->get_low()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - 10 >= 0)
			{
				sound_gamble();

				temp = gamble->low_gamble();

				inforBoard->setGold(inforBoard->getGold() - 10);
				inforBoard->setGold(inforBoard->getGold() + temp);

				addlabel(NULL, temp, 5);
			}
			else addlabel(NULL, temp, 4);
		}
		else if (gamble->get_middle()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - 100 >= 0)
			{
				sound_gamble();

				temp = gamble->middle_gamble();

				inforBoard->setGold(inforBoard->getGold() - 100);
				inforBoard->setGold(inforBoard->getGold() + temp);

				addlabel(NULL, temp, 5);
			}
			else addlabel(NULL, temp, 4);
		}
		else if (gamble->get_high()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - 500 >= 0)
			{
				sound_gamble();

				temp = gamble->high_gamble();

				inforBoard->setGold(inforBoard->getGold() - 500);
				inforBoard->setGold(inforBoard->getGold() + temp);

				addlabel(NULL, temp, 5);
			}
			else addlabel(NULL, temp, 4);
		}
		else if (gamble->get_esc()->boundingBox().containsPoint(touches[0]->getLocation() - pt) || !gamble->boundingBox().containsPoint(touches[0]->getLocation()))
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
		Point pt = Point(x, y);

		if (upgrade->get_D_button()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - (10 + upgrade_count[0]) >= 0)
			{
				sound_upgrade();

				upgrade_count[0]++;
				upgrade->setD(upgrade_count[0]);
				upgrade_update("D");
				inforBoard->setGold(inforBoard->getGold() - (10 + upgrade_count[0]));

				addlabel("D", 0, 6);
				update_hero_list();
			}
			else addlabel(NULL, 0, 7);
		}
		else if (upgrade->get_C_button()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - (20 + upgrade_count[1] * 2) >= 0)
			{
				sound_upgrade();

				upgrade_count[1]++;
				upgrade->setC(upgrade_count[1]);
				upgrade_update("C");
				inforBoard->setGold(inforBoard->getGold() - (20 + upgrade_count[1] * 2));

				addlabel("C", 0, 6);
				update_hero_list();
			}
			else addlabel(NULL, 0, 7);
		}
		else if (upgrade->get_B_button()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - (50 + upgrade_count[2] * 5) >= 0)
			{
				sound_upgrade();

				upgrade_count[2]++;
				upgrade->setB(upgrade_count[2]);
				upgrade_update("B");
				inforBoard->setGold(inforBoard->getGold() - (50 + upgrade_count[2] * 5));

				addlabel("B", 0, 6);
				update_hero_list();
			}
			else addlabel(NULL, 0, 7);
		}
		else if (upgrade->get_A_button()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - (100 + upgrade_count[2] * 10) >= 0)
			{
				sound_upgrade();

				upgrade_count[3]++;
				upgrade->setA(upgrade_count[3]);
				upgrade_update("A");
				inforBoard->setGold(inforBoard->getGold() - (100 + upgrade_count[2] * 10));

				addlabel("A", 0, 6);
				update_hero_list();
			}
			else addlabel(NULL, 0, 7);
		}
		else if (upgrade->get_S_button()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - (300 + upgrade_count[2] * 30) >= 0)
			{
				sound_upgrade();

				upgrade_count[4]++;
				upgrade->setS(upgrade_count[4]);
				upgrade_update("S");
				inforBoard->setGold(inforBoard->getGold() - (300 + upgrade_count[2] * 30));

				addlabel("S", 0, 6);
				update_hero_list();
			}
			else addlabel(NULL, 0, 7);
		}
		else if (upgrade->get_SS_button()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() - (500 + upgrade_count[2] * 50) >= 0)
			{
				sound_upgrade();

				upgrade_count[5]++;
				upgrade->setSS(upgrade_count[5]);
				upgrade_update("SS");
				inforBoard->setGold(inforBoard->getGold() - (500 + upgrade_count[2] * 50));

				addlabel("SS", 0, 6);
				update_hero_list();
			}
			else addlabel(NULL, 0, 7);
		}
		else if (upgrade->get_esc_button()->boundingBox().containsPoint(touches[0]->getLocation() - pt) || !upgrade->boundingBox().containsPoint(touches[0]->getLocation()))
		{
			touch_upgrade = false;

			upgrade->setVisible(false);
		}
	}

	if (touch_mix == true)
	{
		MixBoard* mixBoard = (MixBoard*)getChildByTag(TAG_HERO)->getChildByTag(TAG_INTERFACE_MIX);

		float x = mixBoard->getPositionX() - (mixBoard->getContentSize().width / 2);
		float y = mixBoard->getPositionY() - (mixBoard->getContentSize().height / 2);
		Point pt = Point(x, y);

		Mix_hero* mix_hero = NULL;

		if (mixBoard->get_esc()->boundingBox().containsPoint(touches[0]->getLocation() - pt) || !mixBoard->boundingBox().containsPoint(touches[0]->getLocation()))
		{
			touch_mix = false;

			mixBoard->setVisible(false);
		}

		for (std::vector<Mix_hero*>::iterator iterMix = arr_mix_hero.begin(); iterMix != arr_mix_hero.end(); iterMix++)
		{
			mix_hero = (Mix_hero*)*iterMix;

			char szFile[64] = { 0, };
			//float x = getChildByTag(TAG_UNIT)->getChildByTag(TAG_MENU_MIX_LIST)->getPosition().x;
			//float y = getChildByTag(TAG_UNIT)->getChildByTag(TAG_MENU_MIX_LIST)->getPosition().y;
			Point pt = Point(140, 35);
			Point tp = touches[0]->getLocation();

			if (mix_hero->get_result_hero()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
			{
				int check_count = 0;
				bool check_unit[3] = { false, false, false };

				Unit* unit = NULL;
				for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
				{
					unit = (Unit*)*iterUnit;

					for (int i = 0; i < mix_hero->get_count(); i++)
					{
						if (check_unit[i] == true)
							continue;

						if (!strcmp(unit->getType(), mix_hero->get_mat_hero_type(i)) && unit->getCount() == mix_hero->get_mat_hero_count(i))
							check_unit[i] = true;
					}
				}

				for (int i = 0; i < mix_hero->get_count(); i++)
				{
					if (check_unit[i] == true)
						check_count++;
				}

				if (mix_hero->get_count() == check_count)
				{
					if (mix_hero->get_count() == arr_unit.size())
					{
						for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
						{
							unit = (Unit*)*iterUnit;

							unit->release();
							delete unit;
						}
						arr_unit.clear();
					}

					for (int i = 0; i < 3; i++)
						check_unit[i] = false;

					bool flag = false;

					for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
					{
						if (flag == true) iterUnit--;
						flag = false;

						unit = (Unit*)*iterUnit;

						for (int i = 0; i < mix_hero->get_count(); i++)
						{
							if (check_unit[i] == true)
								continue;

							if (!strcmp(unit->getType(), mix_hero->get_mat_hero_type(i)) && unit->getCount() == mix_hero->get_mat_hero_count(i))
							{
								check_unit[i] = true;

								unit->release();
								delete unit;
								iterUnit = arr_unit.erase(iterUnit);
								flag = true;
							}
						}
						if (iterUnit == arr_unit.end())
							break;
					}
					update_hero_list();

					sound_hero_mix();

					sprintf(szFile, "hero/%s/%d", mix_hero->get_result_hero_type(), mix_hero->get_result_hero_count());
					get_db_data(szFile, DEFENCEJS);
				}
				else
					addlabel(NULL, 0, 8);
			}
		}
	}

	if (touch_capsule == true)
	{
		InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);
		CapsuleBoard* capsule = (CapsuleBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_CAPSULE);

		char szFile[64] = { 0, };

		float x = capsule->getPositionX() - (capsule->getContentSize().width / 2);
		float y = capsule->getPositionY() - (capsule->getContentSize().height / 2);
		Point pt = Point(x, y);

		if (capsule->get_D_capsule()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() >= 10)
			{
				if (inforBoard->getJewelry() >= 1)
				{
					sound_capsule();

					inforBoard->setGold(inforBoard->getGold() - 10);
					inforBoard->setJewelry(inforBoard->getJewelry() - 1);

					if (rand_cal(0.8))
					{
						addlabel(NULL, 0, 11);
						sprintf(szFile, "hero/D/%d", capsule->D_gamble());
						get_db_data(szFile, DEFENCEJS);
					}
					else addlabel(NULL, 0, 10);
				}
				else addlabel(NULL, 0, 9);
			}
			else
			{
				addlabel(NULL, 0, 4);

				if (inforBoard->getJewelry() < 1)
					addlabel(NULL, 0, 9);
			}
		}
		else if (capsule->get_C_capsule()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() >= 50)
			{
				if (inforBoard->getJewelry() >= 2)
				{
					sound_capsule();

					inforBoard->setGold(inforBoard->getGold() - 50);
					inforBoard->setJewelry(inforBoard->getJewelry() - 2);

					if (rand_cal(0.6))
					{
						addlabel(NULL, 0, 11);
						sprintf(szFile, "hero/C/%d", capsule->C_gamble());
						get_db_data(szFile, DEFENCEJS);
					}
					else addlabel(NULL, 0, 10);
				}
				else addlabel(NULL, 0, 9);
			}
			else
			{
				addlabel(NULL, 0, 4);

				if (inforBoard->getJewelry() < 2)
					addlabel(NULL, 0, 9);
			}
		}
		else if (capsule->get_B_capsule()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() >= 150)
			{
				if (inforBoard->getJewelry() >= 3)
				{
					sound_capsule();

					inforBoard->setGold(inforBoard->getGold() - 150);
					inforBoard->setJewelry(inforBoard->getJewelry() - 3);

					if (rand_cal(0.4))
					{
						addlabel(NULL, 0, 11);
						sprintf(szFile, "hero/B/%d", capsule->B_gamble());
						get_db_data(szFile, DEFENCEJS);
					}
					else addlabel(NULL, 0, 10);
				}
				else addlabel(NULL, 0, 9);
			}
			else
			{
				addlabel(NULL, 0, 4);

				if (inforBoard->getJewelry() < 3)
					addlabel(NULL, 0, 9);
			}
		}
		else if (capsule->get_A_capsule()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() >= 500)
			{
				if (inforBoard->getJewelry() >= 5)
				{
					sound_capsule();

					inforBoard->setGold(inforBoard->getGold() - 500);
					inforBoard->setJewelry(inforBoard->getJewelry() - 5);

					if (rand_cal(0.2))
					{
						addlabel(NULL, 0, 11);
						sprintf(szFile, "hero/A/%d", capsule->A_gamble());
						get_db_data(szFile, DEFENCEJS);
					}
					else addlabel(NULL, 0, 10);
				}
				else addlabel(NULL, 0, 9);
			}
			else
			{
				addlabel(NULL, 0, 4);

				if (inforBoard->getJewelry() < 5)
					addlabel(NULL, 0, 9);
			}
		}
		else if (capsule->get_S_capsule()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() >= 1200)
			{
				if (inforBoard->getJewelry() >= 10)
				{
					sound_capsule();

					inforBoard->setGold(inforBoard->getGold() - 1200);
					inforBoard->setJewelry(inforBoard->getJewelry() - 10);

					if (rand_cal(0.1))
					{
						addlabel(NULL, 0, 11);
						sprintf(szFile, "hero/S/%d", capsule->S_gamble());
						get_db_data(szFile, DEFENCEJS);
					}
					else addlabel(NULL, 0, 10);
				}
				else addlabel(NULL, 0, 9);
			}
			else
			{
				addlabel(NULL, 0, 4);

				if (inforBoard->getJewelry() < 10)
					addlabel(NULL, 0, 9);
			}
		}
		else if (capsule->get_SS_capsule()->boundingBox().containsPoint(touches[0]->getLocation() - pt))
		{
			if (inforBoard->getGold() >= 2000)
			{
				if (inforBoard->getJewelry() >= 15)
				{
					sound_capsule();

					inforBoard->setGold(inforBoard->getGold() - 2000);
					inforBoard->setJewelry(inforBoard->getJewelry() - 15);

					if (rand_cal(0.04))
					{
						addlabel(NULL, 0, 11);
						sprintf(szFile, "hero/SS/%d", capsule->SS_gamble());
						get_db_data(szFile, DEFENCEJS);
					}
					else addlabel(NULL, 0, 10);
				}
				else addlabel(NULL, 0, 9);
			}
			else
			{
				addlabel(NULL, 0, 4);

				if (inforBoard->getJewelry() < 15)
					addlabel(NULL, 0, 9);
			}
		}
		else if (capsule->get_esc()->boundingBox().containsPoint(touches[0]->getLocation() - pt) || !capsule->boundingBox().containsPoint(touches[0]->getLocation()))
		{
			touch_capsule = false;

			capsule->setVisible(false);
		}
	}

	if (touch_hero == true)
	{
		HeroBoard* heroBoard = (HeroBoard*)getChildByTag(TAG_HERO)->getChildByTag(TAG_INTERFACE_HERO);

		float x = heroBoard->getPositionX() - (heroBoard->getContentSize().width / 2);
		float y = heroBoard->getPositionY() - (heroBoard->getContentSize().height / 2);
		Point pt = Point(x, y);
		bool end = false;

		//보내기 클릭하면 다른 위치로 보내기
		HeroList* heroList = NULL;
		Unit* unit = NULL;

		if (heroBoard->get_esc()->boundingBox().containsPoint(touches[0]->getLocation() - pt) || !heroBoard->boundingBox().containsPoint(touches[0]->getLocation()))
		{
			touch_hero = false;

			heroBoard->setVisible(false);
		}

		for (std::vector<HeroList*>::iterator iterHero = arr_hero_list.begin(); iterHero != arr_hero_list.end(); iterHero++)
		{
			if (hero_menu_move == true)
				break;

			heroList = (HeroList*)*iterHero;
			char szFile[32] = { 0, };
			//1 : 내맵 - 2 : 보스룸 - 3 : 보낸거

			if (heroList->getHero()->boundingBox().containsPoint(touches[0]->getLocation() - pt - Point(155, 0)))	//BOSS
			{
				if (heroList->getLocation() == 1)
				{
					for (std::vector<Unit*>::iterator iterUnit = arr_unit.begin(); iterUnit != arr_unit.end(); iterUnit++)
					{
						unit = (Unit*)*iterUnit;

						if (!strcmp(heroList->getType(), unit->getType()) && heroList->getCount() == unit->getCount())
						{
							sound_unit_move();

							Point pt = getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getChildByTag(TAG_INTERFACE_BOSS)->getContentSize();
							float xR = rand() % 10000;
							float yR = rand() % 10000;

							pt.x *= (0.7 + (xR / 100000 * 2));
							pt.y *= (0.2 + (yR / 100000 * 3));

							arr_unit.erase(iterUnit);
							arr_boss_room_unit.push_back(unit);

							CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(SingleGame::removeChild_background_boss, this));

							unit->getBody()->setPosition(pt);

							unit->getBody()->runAction(callfunc);
							///////////////////////////
							HeroList* new_heroList = new HeroList();

							new_heroList->setHero(unit->getName());
							new_heroList->setMap("null.png");
							new_heroList->setBoss("in.png");
							new_heroList->setHelp("null.png");
							new_heroList->getHelp()->setVisible(false);
							new_heroList->setType(unit->getType());
							new_heroList->setAtk(unit->getDamage());
							new_heroList->setCount(unit->getCount());
							new_heroList->init(0);
							new_heroList->setLocation(2);

							new_heroList->getHero()->setPosition(heroList->getHero()->getPosition());

							heroBoard->addChild(new_heroList->getHero());
							
							iterHero = arr_hero_list.erase(iterHero);
							heroList->release();
							delete heroList;

							arr_hero_list.push_back(new_heroList);

							end = true;
							/////////////////////
							break;
						}
					}
				}
				else break;
			}
			else if (heroList->getHero()->boundingBox().containsPoint(touches[0]->getLocation() - pt - Point(120, 0)))		//MAP
			{
				if (heroList->getLocation() == 2)
				{
					for (std::vector<Unit*>::iterator iterUnit = arr_boss_room_unit.begin(); iterUnit != arr_boss_room_unit.end(); iterUnit++)
					{
						unit = (Unit*)*iterUnit;

						if (!strcmp(heroList->getType(), unit->getType()) && heroList->getCount() == unit->getCount())
						{
							sound_unit_move();

							Point pt = getChildByTag(TAG_BACKGROUND)->getContentSize();

							arr_boss_room_unit.erase(iterUnit);
							arr_unit.push_back(unit);

							CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(SingleGame::removeChild_boss_background, this));

							unit->getBody()->setPosition(pt / 2);

							unit->getBody()->runAction(callfunc);
							///////////////////////////
							HeroList* new_heroList = new HeroList();

							new_heroList->setHero(unit->getName());
							new_heroList->setMap("in.png");
							new_heroList->setBoss("null.png");
							new_heroList->setHelp("null.png");
							new_heroList->getHelp()->setVisible(false);
							new_heroList->setType(unit->getType());
							new_heroList->setAtk(unit->getDamage());
							new_heroList->setCount(unit->getCount());
							new_heroList->init(0);
							new_heroList->setLocation(1);

							new_heroList->getHero()->setPosition(heroList->getHero()->getPosition());

							heroBoard->addChild(new_heroList->getHero());

							iterHero = arr_hero_list.erase(iterHero);
							heroList->release();
							delete heroList;

							arr_hero_list.push_back(new_heroList);

							end = true;
							/////////////////////
							break;
						}
					}
				}
				else break;
			}
			if (end == true)
				break;
		}
		if (hero_menu_move == true)
			hero_menu_move = false;
		//else update_hero_list();
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

void SingleGame::add_unit_queue(float dt)
{
	Use_String* queue = NULL;

	for (std::vector<Use_String*>::iterator iterQueue = arr_unit_queue.begin(); iterQueue != arr_unit_queue.end(); iterQueue++)
	{
		queue = (Use_String*)*iterQueue;

		char * rev = queue->getString();
		strtok(rev, "/");
		char * sprite = strtok(NULL, "/");
		char * name = strtok(NULL, "/");
		char * type = strtok(NULL, "/");
		char * count = strtok(NULL, "/");
		char * speed = strtok(NULL, "/");
		char * range = strtok(NULL, "/");
		char * damage = strtok(NULL, "/");

		addunit(sprite, name, type, atoi(count), atof(speed), atof(range), atof(damage));

		addlabel(name, 0, 0);

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

void SingleGame::onTouchesMoved(const std::vector<Touch*>& touches, Event *event)
{
	MixBoard* mixBoard = (MixBoard*)getChildByTag(TAG_HERO)->getChildByTag(TAG_INTERFACE_MIX);
	HeroList* heroList = (HeroList*)getChildByTag(TAG_HERO)->getChildByTag(TAG_INTERFACE_HERO);

	if (touch_unit == true || touch_soul == true || touch_gamble == true || touch_upgrade == true || touch_capsule == true)
		return;

	if (touch_mix == true && mixBoard->boundingBox().containsPoint(Point(touch_point)))
	{
		Mix_hero* mix_hero = NULL;
		Mix_hero* mix_out = (Mix_hero*)*arr_mix_hero.begin();

		for (std::vector<Mix_hero*>::iterator iterMix = arr_mix_hero.begin(); iterMix != arr_mix_hero.end(); iterMix++)
		{
			mix_hero = (Mix_hero*)*iterMix;
			Rect mix_rect = Rect(35, 35, 190, 150);
			float cur_y = touches[0]->getLocation().y;
			float dis_y = touch_point.y - cur_y;
			float use_x = mix_hero->get_result_hero()->getPositionX();

			if (mix_out->get_result_hero()->getPositionY() < 195)
				dis_y = -1;

			float des_y = mix_hero->get_result_hero()->getPositionY() - dis_y;

			MoveTo* moveto = MoveTo::create(0.f, Point(use_x, des_y));

			mix_hero->get_result_hero()->runAction(moveto);

			use_x = mix_hero->get_hero_type()->getPositionX();
			moveto = MoveTo::create(0.f, Point(use_x, des_y));
			mix_hero->get_hero_type()->runAction(moveto);

			for (int i = 0; i < mix_hero->get_count(); i++)
			{
				use_x = mix_hero->get_mat_hero(i)->getPositionX();

				moveto = MoveTo::create(0.f, Point(use_x, des_y));

				mix_hero->get_mat_hero(i)->runAction(moveto);
			}

			if (!mix_rect.intersectsRect(mix_hero->get_result_hero()->getBoundingBox()))
			{
				mix_hero->get_result_hero()->setVisible(false);
				mix_hero->get_hero_type()->setVisible(false);
				//getChildByTag(TAG_UNIT)->getChildByTag(TAG_MENU_MIX_LIST)->removeChild(mix_hero->get_result_hero());

				for (int i = 0; i < mix_hero->get_count(); i++)
				{
					mix_hero->get_mat_hero(i)->setVisible(false);
					//getChildByTag(TAG_UNIT)->getChildByTag(TAG_MENU_MIX_LIST)->removeChild(mix_hero->get_mat_hero(i));
				}
			}
			else
			{
				mix_hero->get_result_hero()->setVisible(true);
				mix_hero->get_hero_type()->setVisible(true);
				//getChildByTag(TAG_UNIT)->getChildByTag(TAG_MENU_MIX_LIST)->addChild(mix_hero->get_result_hero());

				for (int i = 0; i < mix_hero->get_count(); i++)
				{
					mix_hero->get_mat_hero(i)->setVisible(true);
					//getChildByTag(TAG_UNIT)->getChildByTag(TAG_MENU_MIX_LIST)->addChild(mix_hero->get_mat_hero(i));
				}
			}
		}
		touch_point = touches[0]->getLocation();
	}

	if (touch_hero == true && heroList->boundingBox().containsPoint(Point(touch_point)))
	{
		hero_menu_move = true;

		HeroList* heroList = NULL;
		HeroList* hero_out = (HeroList*)*arr_hero_list.begin();

		for (std::vector<HeroList*>::iterator iterHero = arr_hero_list.begin(); iterHero != arr_hero_list.end(); iterHero++)
		{
			heroList = (HeroList*)*iterHero;
			Rect hero_rect = Rect(0, 35, 480, 100);
			float cur_y = touches[0]->getLocation().y;
			float dis_y = touch_point.y - cur_y;
			float use_x = heroList->getHero()->getPositionX();

			if (hero_out->getHero()->getPositionY() < 135)
				dis_y = -1;

			float des_y = heroList->getHero()->getPositionY() - dis_y;

			MoveTo* moveto = MoveTo::create(0.f, Point(use_x, des_y));

			heroList->getHero()->runAction(moveto);

			if (!hero_rect.intersectsRect(heroList->getHero()->boundingBox()))
				heroList->getHero()->setVisible(false);
			else heroList->getHero()->setVisible(true);
		}
		touch_point = touches[0]->getLocation();
	}

	if (touch_mix == true || touch_hero == true)
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

Point SingleGame::map_out_check(Point point)
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

void SingleGame::monster_location_init(Sprite* sprite)
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

void SingleGame::zorder_assort(float dt)
{
	char szFile[32] = { 0, };
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

			sound_stage();

			inforBoard->setStage(inforBoard->getStage() + 1);
			sprintf(szFile, "monster/%d", inforBoard->getStage());
			get_db_data(szFile, DEFENCEJS);

			/////////// 보스 관련 수정중 -> 보스 생성 및 처치 실패
			if (inforBoard->getStage() % 10 == 5 && alive_boss == true)
			{
				sprintf(szFile, "alive_boss/%d", getID());
				get_db_data(szFile, DEFENCEJS);
				GameOver();
			}

			if (inforBoard->getStage() % 10 == 0 && inforBoard->getStage() > 0 && alive_boss == false)
			{
				boss_stage++;
				alive_boss = true;

				sprintf(szFile, "create_boss/%d/%s", boss_stage, getID());

				get_db_data(szFile, DEFENCEJS);
			}

			summon_monster = 10;
			inforBoard->setSoul(inforBoard->getSoul() + 3);
			soulBoard->setSoul(inforBoard->getSoul());
			soulBoard->updateNumber(soulBoard);
			addlabel(NULL, 3, 3);
		}
		schedule(schedule_selector(SingleGame::addmonster), 1.f);
		new_soul_1 = true;
		new_soul_2 = false;
	}
	else if (summon_monster == 0)
	{
		unschedule(schedule_selector(SingleGame::addmonster));
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

void SingleGame::touch_unit_check()
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

void SingleGame::screen_out()
{
	getChildByTag(TAG_BACKGROUND)->setPosition(getChildByTag(TAG_BACKGROUND)->getPosition());
}

void SingleGame::onMenu(Object* sender)
{
	InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);
	char szFile[32] = { 0, };

	switch (((Node*)sender)->getTag())
	{
	case TAG_MENU_SOUL:
		if (touch_gamble == true || touch_upgrade == true || touch_mix == true || touch_capsule == true || touch_hero == true || touch_boss == true) break;

		sound_interface_open();

		touch_soul = true;
		new_soul_1 = false;
		new_soul_2 = false;

		getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_SOUL)->setVisible(true);
		break;
	case TAG_MENU_GAMBLE:
		if (touch_soul == true || touch_upgrade == true || touch_mix == true || touch_capsule == true || touch_hero == true || touch_boss == true) break;

		sound_interface_open();

		touch_gamble = true;

		getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_GAMBLE)->setVisible(true);
		break;
	case TAG_MENU_SKIP:
		sound_interface_open();

		if (skip == false)
		{
			skip = true;
			getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SKIP)->getChildByTag(TAG_MENU_SKIP_BACK)->setVisible(true);

			if (inforBoard->getTime() < 51 && inforBoard->getTime() > 1)
				inforBoard->setTime(1);
		}
		else
		{
			skip = false;
			getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_SKIP)->getChildByTag(TAG_MENU_SKIP_BACK)->setVisible(false);
		}

		break;
	case TAG_MENU_UPGRADE:
		if (touch_soul == true || touch_gamble == true || touch_mix == true || touch_capsule == true || touch_hero == true || touch_boss == true) break;

		sound_interface_open();

		touch_upgrade = true;

		getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_UPGRADE)->setVisible(true);
		break;
	case TAG_MENU_MIX:
		if (touch_soul == true || touch_gamble == true || touch_upgrade == true || touch_capsule == true || touch_hero == true || touch_boss == true) break;

		sound_interface_open();

		touch_mix = true;

		getChildByTag(TAG_HERO)->getChildByTag(TAG_INTERFACE_MIX)->setVisible(true);
		break;
	case TAG_MENU_CAPSULE:
		if (touch_soul == true || touch_gamble == true || touch_mix == true || touch_upgrade == true || touch_hero == true || touch_boss == true) break;

		sound_interface_open();

		touch_capsule = true;

		getChildByTag(TAG_UNIT)->getChildByTag(TAG_INTERFACE_CAPSULE)->setVisible(true);
		break;
	case TAG_MENU_HERO:
		if (touch_soul == true || touch_gamble == true || touch_mix == true || touch_upgrade == true || touch_capsule == true || touch_boss == true) break;

		sound_interface_open();

		update_hero_list();

		touch_hero = true;

		getChildByTag(TAG_HERO)->getChildByTag(TAG_INTERFACE_HERO)->setVisible(true);
		break;
	case TAG_MENU_BOSS:
		if (touch_soul == true || touch_gamble == true || touch_mix == true || touch_upgrade == true || touch_capsule == true || touch_hero == true) break;

		sound_boss_room_open();

		Point pt;

		if (touch_boss == false)
		{
			pt = Point(getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getPosition()) - Point(getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getChildByTag(TAG_INTERFACE_BOSS)->getContentSize().width, 0);
			touch_boss = true;
		}
		else
		{
			pt = Point(getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getPosition()) + Point(getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getChildByTag(TAG_INTERFACE_BOSS)->getContentSize().width, 0);
			touch_boss = false;
		}

		MoveTo* moveTo = MoveTo::create(1.f, pt);
		getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->runAction(moveTo);

		break;
	}
}

void SingleGame::move_unit(Unit* unit, bool right)
{
	char szFile[64] = { 0, };

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	if (right == true)
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(szFile, "%s_right_%d.png", unit->getName(), i);
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
		}
		//sprintf(szFile, "%s.png", unit->getName());
		//animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(szFile, "%s_left_%d.png", unit->getName(), i);
			animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
		}
		//sprintf(szFile, "%s.png", unit->getName());
		//animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}

	animation->setDelayPerUnit(0.1f);

	Animate* animate = Animate::create(animation);
	RepeatForever* repeat = RepeatForever::create(animate);

	unit->getBody()->runAction(repeat);
	unit->getBody()->getChildByTag(TAG_RANGE)->setVisible(false);
}

void SingleGame::addlabel(char* name, int gold, int choice)
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
		sprintf(szFile, "%s 소환!", name);
		break;
	case 1:
		sprintf(szFile, "%d 골드 획득!", gold);
		break;
	case 2:
		sprintf(szFile, "1 보석 획득!");
		break;
	case 3:
		sprintf(szFile, "시민 %d 획득!", gold);
		break;
	case 4:
		sprintf(szFile, "골드 부족!");
		break;
	case 5:
		sprintf(szFile, "도박 성공! %d 골드 획득!", gold);
		break;
	case 6:
		sprintf(szFile, "%s 등급 업그레이드!", name);
		break;
	case 7:
		sprintf(szFile, "업그레이드 비용 부족!");
		break;
	case 8:
		sprintf(szFile, "조합 영웅 부족!");
		break;
	case 9:
		sprintf(szFile, "보석 부족!");
		break;
	case 10:
		sprintf(szFile, "뽑기 실패!");
		break;
	case 11:
		sprintf(szFile, "뽑기 성공!");
		break;
	case 12:
		sprintf(szFile, "보스 생성!");
		break;
	}
	label = Label::createWithSystemFont(szFile, "Arial", 10);

	sprite->setPosition(winSize.width / 2, winSize.height * 0.8 - height);
	sprite->setOpacity(100);

	label->setPosition(Point(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2));
	addChild(sprite, ZORDER_LABEL, TAG_LABEL);
	sprite->addChild(label, ZORDER_LABEL, TAG_LABEL);

	FadeOut* fadeout = FadeOut::create(1.5f);
	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(SingleGame::labelRemover, this));
	Sequence* sequence = Sequence::create(fadeout, callfunc, NULL);

	sprite->runAction(sequence);

	arr_label.push_back(sprite);
}

void SingleGame::labelRemover(Node* sender)
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

bool SingleGame::rand_cal(float per)
{
	float num = rand() % 10000;

	if (num / 10000 < per)
		return true;
	else return false;
}

void SingleGame::onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response)
{
	std::vector<char> * buffer = response->getResponseData();

	Use_String * use_string = new Use_String();
	char szFile[256] = { 0, };
	char temp[256] = { 0, };
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
	else if (!strcmp(compare, "mix_hero"))
	{
		MixBoard* mixBoard = (MixBoard*)getChildByTag(TAG_HERO)->getChildByTag(TAG_INTERFACE_MIX);

		strtok(szFile, "/");
		//mix_hero/result_sprite/result_type/result_count/list_count/mat_sprite/mat_type/mat_count ...
		Mix_hero * mix_hero = new Mix_hero();
		Rect mix_rect = Rect(35, 35, 190, 165);
		int height = arr_mix_hero.size() * 50;

		char * sprite = strtok(NULL, "/");
		char * type = strtok(NULL, "/");
		char * count = strtok(NULL, "/");
		char * name = strtok(NULL, "/");
		char * list_count = strtok(NULL, "/");

		mix_hero->set_result_hero(sprite);
		mix_hero->set_result_hero_type(type);
		mix_hero->set_result_hero_count(atoi(count));
		mix_hero->set_result_hero_name(name);
		mix_hero->set_count(atoi(list_count));

		mix_hero->set_hero_type(type);

		for (int i = 0; i < atoi(list_count); i++)
		{
			sprite = strtok(NULL, "/");
			type = strtok(NULL, "/");
			count = strtok(NULL, "/");
			name = strtok(NULL, "/");

			mix_hero->set_mat_hero(sprite, i);
			mix_hero->set_mat_hero_type(type, i);
			mix_hero->set_mat_hero_count(atoi(count), i);
			mix_hero->set_mat_hero_name(name, i);
		}

		mix_hero->get_result_hero()->setPosition(Point(40, 215 - (height)));
		mixBoard->addChild(mix_hero->get_result_hero());

		mix_hero->get_hero_type()->setPosition(Point(18, 215 - (height)));
		mixBoard->addChild(mix_hero->get_hero_type());

		for (int i = 0; i < atoi(list_count); i++)
		{
			mix_hero->get_mat_hero(i)->setPosition(Point(85 + (i * 40), 215 - (height)));
			mixBoard->addChild(mix_hero->get_mat_hero(i));
		}

		if (!mix_rect.intersectsRect(mix_hero->get_result_hero()->getBoundingBox()))
		{
			mix_hero->get_result_hero()->setVisible(false);
			mix_hero->get_hero_type()->setVisible(false);

			for (int i = 0; i < atoi(list_count); i++)
			{
				mix_hero->get_mat_hero(i)->setVisible(false);
			}
		}

		arr_mix_hero.push_back(mix_hero);
	}
	else if (!strcmp(compare, "boss"))
	{
		strtok(szFile, "/");
		char * name = strtok(NULL, "/");
		char * hp = strtok(NULL, "/");
		char * def = strtok(NULL, "/");

		create_boss(name, atof(hp), atof(def));

		addlabel(NULL, 0, 12);
	}

	/*
	printf("Response Code : %li \n", response->getResponseCode());

	if (200 == response->getResponseCode())
	printf("Successful");
	else
	printf("Failed");
	*/
}

void SingleGame::get_db_data(char * data, int port)
{
	__String * dataToSend = __String::create(data);
	char szFile[32] = { 0, };

	//모바일 버전
	//sprintf(szFile, "http://192.168.219.102:%d", port);
	sprintf(szFile, "localhost:%d", port);

	cocos2d::network::HttpRequest * request = new cocos2d::network::HttpRequest();
	request->setUrl(szFile);
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setRequestData(dataToSend->getCString(), dataToSend->length());
	request->setResponseCallback(CC_CALLBACK_2(SingleGame::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void SingleGame::upgrade_update(char* up)
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
			unit->setDamage(damage * rate * (upgrade_count[index]+1));
	}
}

void SingleGame::mix_hero_init()
{
	char szFile[64] = { 0, };
	int index = 1;

	while (mix_list-- > 0)
	{
		sprintf(szFile, "mix_hero/%d", index++);
		get_db_data(szFile, DEFENCEJS);
	}
}

void SingleGame::GameOver()
{
	char szFile[64] = { 0, };

	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	sound_gameover();

	unschedule(schedule_selector(SingleGame::zorder_assort));
	unschedule(schedule_selector(SingleGame::unit_atk_cooltime));
	unschedule(schedule_selector(SingleGame::unit_atk_monster));
	unschedule(schedule_selector(SingleGame::add_unit_queue));

	Monster* monster = NULL;

	for (std::vector<Monster*>::iterator iter = arr_monster.begin(); iter != arr_monster.end(); iter++)
	{
		monster = (Monster*)*iter;
		monster->release();
		delete monster;
	}
	arr_monster.clear();

	Unit* unit = NULL;

	for (std::vector<Unit*>::iterator iter = arr_unit.begin(); iter != arr_unit.end(); iter++)
	{
		unit = (Unit*)*iter;
		unit->release();
		delete unit;
	}
	arr_unit.clear();

	for (std::vector<Unit*>::iterator iter = arr_boss_room_unit.begin(); iter != arr_boss_room_unit.end(); iter++)
	{
		unit = (Unit*)*iter;
		unit->release();
		delete unit;
	}
	arr_boss_room_unit.clear();
	
	Use_String* use_string = NULL;

	for (std::vector<Use_String*>::iterator iter = arr_unit_queue.begin(); iter != arr_unit_queue.end(); iter++)
	{
		use_string = (Use_String*)*iter;
		delete use_string;
	}
	arr_unit_queue.clear();

	Sprite* arr = NULL;

	for (std::vector<Sprite*>::iterator iter = arr_location.begin(); iter != arr_location.end(); iter++)
	{
		arr = (Sprite*)*iter;
		arr->removeFromParentAndCleanup(true);
	}
	arr_location.clear();

	for (std::vector<Sprite*>::iterator iter = arr_label.begin(); iter != arr_label.end(); iter++)
	{
		arr = (Sprite*)*iter;
		arr->removeFromParentAndCleanup(true);
	}
	arr_label.clear();

	Mix_hero* mix = NULL;

	for (std::vector<Mix_hero*>::iterator iter = arr_mix_hero.begin(); iter != arr_mix_hero.end(); iter++)
	{
		mix = (Mix_hero*)*iter;
		mix->release();
		delete mix;
	}
	arr_mix_hero.clear();

	HeroList* heroList = NULL;

	for (std::vector<HeroList*>::iterator iter = arr_hero_list.begin(); iter != arr_hero_list.end(); iter++)
	{
		heroList = (HeroList*)*iter;
		heroList->release();
		delete heroList;
	}
	arr_hero_list.clear();

	sprintf(szFile, "gameover/%s", getID());

	get_db_data(szFile, DEFENCEJS);
	//게임오버 연출로 변경

	_eventDispatcher->autorelease();
	_eventDispatcher->removeAllEventListeners();

	Scene* gameover = GameOver::scene(getID());
	TransitionScene* transition = TransitionFade::create(1.5f, gameover);
	Director::getInstance()->replaceScene(transition);
}

void SingleGame::update_hero_list()
{
	HeroBoard* heroBoard = (HeroBoard*)getChildByTag(TAG_HERO)->getChildByTag(TAG_INTERFACE_HERO);

	Unit* unit = NULL;
	HeroList* heroList = NULL;

	Rect rt = Rect(0, 35, 480, 100);

	for (std::vector<HeroList*>::iterator iter = arr_hero_list.begin(); iter != arr_hero_list.end(); iter++)
	{
		heroList = (HeroList*)*iter;

		heroList->release();
		delete heroList;
	}
	arr_hero_list.clear();

	for (std::vector<Unit*>::iterator iter = arr_unit.begin(); iter != arr_unit.end(); iter++)
	{
		unit = (Unit*)*iter;

		heroList = new HeroList();

		heroList->setHero(unit->getName());
		heroList->setMap("in.png");
		heroList->setBoss("null.png");
		heroList->setHelp("null.png");
		heroList->getHelp()->setVisible(false);
		heroList->setType(unit->getType());
		heroList->setAtk(unit->getDamage());
		heroList->setCount(unit->getCount());
		heroList->init(arr_hero_list.size());
		heroList->setLocation(1);

		if (!rt.intersectsRect(heroList->getHero()->boundingBox()))
			heroList->getHero()->setVisible(false);

		heroBoard->addChild(heroList->getHero());

		arr_hero_list.push_back(heroList);
	}

	for (std::vector<Unit*>::iterator iter = arr_boss_room_unit.begin(); iter != arr_boss_room_unit.end(); iter++)
	{
		unit = (Unit*)*iter;

		heroList = new HeroList();

		heroList->setHero(unit->getName());
		heroList->setMap("null.png");
		heroList->setBoss("in.png");
		heroList->setHelp("null.png");
		heroList->getHelp()->setVisible(false);
		heroList->setType(unit->getType());
		heroList->setAtk(unit->getDamage());
		heroList->setCount(unit->getCount());
		heroList->init(arr_hero_list.size());
		heroList->setLocation(2);

		if (!rt.intersectsRect(heroList->getHero()->boundingBox()))
			heroList->getHero()->setVisible(false);

		heroBoard->addChild(heroList->getHero());

		arr_hero_list.push_back(heroList);
	}
	//새로운 클래스 생성(영웅 목록 저장할) 후 Mix_hero와 같이 추가
}

void SingleGame::create_boss(char* name, float hp, float def)
{
	char szFile[64] = { 0, };

	sound_boss_create();

	Point pt = getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getChildByTag(TAG_INTERFACE_BOSS)->getContentSize();

	sprintf(szFile, "boss_%s.png", name);

	boss = new Monster();

	boss->setBody(szFile);
	boss->setEnergy(hp);
	boss->setDefence(def);
	boss->setNum(boss_stage);

	boss->getBody()->setPosition(Point(pt.x * 0.4, pt.y * 0.2));

	getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->addChild(boss->getBody());

	atk_start_boss = true;
}

void SingleGame::atk_boss(float dt)
{
	InforBoard* inforBoard = (InforBoard*)getChildByTag(TAG_UNIT)->getChildByTag(TAG_INFORBOARD);

	Unit* unit = NULL;
	bool right;
	char szFile[32] = { 0, };

	for (std::vector<Unit*>::iterator iterUnit = arr_boss_room_unit.begin(); iterUnit != arr_boss_room_unit.end(); iterUnit++)
	{
		unit = (Unit*)*iterUnit;

		if (atk_start_boss == false) break;

		if (unit->getMaxSpeed() <= unit->getCurSpeed())
		{
			if (unit->getBody()->getPositionX() - boss->getBody()->getPositionX() > 0)
				right = false;
			else right = true;

			unit->setCurSpeed(0);
			unit_atk_motion(unit, right);
			addattack(boss, 1.0);
			//보스한테 데미지 주기(get_db_data) & lock 설정 

			if (0 >= boss->subEnergy(unit->getDamage()))
			{
				sound_boss_delete();

				addmonster_death(boss->getBody()->getPosition(), 1.0, true);

				alive_boss = false;

				inforBoard->setGold(inforBoard->getGold() + (boss_stage * 300));

				boss->release();
				//delete boss;
				// 보스 죽는 것(get_db_data)
				atk_start_boss = false;
			}
			else
				break;
		}
	}
}

void SingleGame::removeChild_background_boss(Node* sender)
{
	getChildByTag(TAG_BACKGROUND)->removeChild(sender, false);

	getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getChildByTag(TAG_INTERFACE_BOSS)->addChild(sender);
}

void SingleGame::removeChild_boss_background(Node* sender)
{
	getChildByTag(TAG_MENU)->getChildByTag(TAG_MENU_BOSS)->getChildByTag(TAG_INTERFACE_BOSS)->removeChild(sender, false);

	getChildByTag(TAG_BACKGROUND)->addChild(sender);
}

void SingleGame::setID(char* name)
{
	strcpy(id, name);
}

char* SingleGame::getID()
{
	return id;
}

void SingleGame::sound_interface_open()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("interface.wav");
}

void SingleGame::sound_ok_button()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ok_button.wav");
}

void SingleGame::sound_gamble()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gamble.wav");
}

void SingleGame::sound_upgrade()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("upgrade.wav");
}

void SingleGame::sound_capsule()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("capsule.wav");
}

void SingleGame::sound_gameover()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gameover.wav");
}

void SingleGame::sound_boss_create()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boss_create.wav");
}

void SingleGame::sound_boss_room_open()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boss_room_open.wav");
}

void SingleGame::sound_hero_mix()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hero_mix.wav");
}

void SingleGame::sound_boss_delete()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boss_delete.wav");
}

void SingleGame::sound_stage()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("stage.wav");
}

void SingleGame::sound_unit_move()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("unit_move.wav");
}
