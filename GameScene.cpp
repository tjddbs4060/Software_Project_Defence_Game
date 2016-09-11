#include "GameScene.h"

USING_NS_CC;

Game::Game() :cur_leave(0), cur_level(0), cur_monster(0), Game_Start(false), touch(false), anc_height(0), anc_width(0) {}

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

	Sprite* sprite_background = Sprite::createWithSpriteFrameName("background.png");
	sprite_background->setAnchorPoint(Point(0, 0));
	addChild(sprite_background, ZORDER_BACKGROUND, TAG_BACKGROUND);

	monster_location_init(sprite_background);

	if (winSize.height < sprite_background->getContentSize().height)
		anc_height = sprite_background->getContentSize().height - winSize.height;

	if (winSize.width < sprite_background->getContentSize().width)
		anc_width = sprite_background->getContentSize().width - winSize.width;

	schedule(schedule_selector(Game::addmonster), 0.8f);
	//schedule(schedule_selector(Game::unit_atk_cooltime));
	//schedule(schedule_selector(Game::unit_atk_monster));
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
	for (std::vector<Sprite*>::iterator iter = arr_location.begin(); iter != arr_location.end(); iter++)
	{
		location = (Sprite*)*iter;
		move[i++] = location->getPosition();
		location = NULL;
	}

	int monster_type = rand() % 3 + 1;
	char szFile[64] = { 0, };

	Size winSize = Director::getInstance()->getWinSize();
	Monster* monster = new Monster;

	sprintf(szFile, "monster_0%d_02.png", monster_type);
	monster->setBody(szFile);
	monster->setEnergy(100.f);		//체력
	monster->setDefence(0.f);
	monster->getBody()->setPosition(move[0]);

	getChildByTag(TAG_UNIT)->addChild(monster->getBody(), ZORDER_MONSTER, TAG_MONSTER);

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
	/*
	int monster_type = rand() % 3 + 1;
	char szFile[64] = { 0, };

	Size winSize = Director::getInstance()->getWinSize();
	Monster* monster = new Monster;

	sprintf(szFile, "monster_0%d_02.png", monster_type);
	monster->setBody(szFile);
	monster->setEnergy(100.f);		//체력
	monster->setDefence(0.f);
	monster->getBody()->setPosition(Point(winSize.width * 0.1, winSize.height * 0.9));

	getChildByTag(TAG_UNIT)->addChild(monster->getBody(), ZORDER_MONSTER, TAG_MONSTER);

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
	
	MoveTo* move_1 = MoveTo::create(h_speed, Point(winSize.width * 0.1, winSize.height * 0.1));
	MoveTo* move_2 = MoveTo::create(w_speed, Point(winSize.width * 0.9, winSize.height * 0.1));
	MoveTo* move_3 = MoveTo::create(h_speed, Point(winSize.width * 0.9, winSize.height * 0.9));
	MoveTo* move_4 = MoveTo::create(w_speed, Point(winSize.width * 0.1, winSize.height * 0.9));
	
	auto Spawn_down = Spawn::create(move_1, rep_down, NULL);
	auto Spawn_right = Spawn::create(move_2, rep_right, NULL);
	auto Spawn_up = Spawn::create(move_3, rep_up, NULL);
	auto Spawn_left = Spawn::create(move_4, rep_left, NULL);

	auto sequence = Sequence::create(Spawn_down, Spawn_right, Spawn_up, Spawn_left, NULL);
	auto rep = RepeatForever::create(sequence);

	monster->getBody()->runAction(rep);

	arr_monster.push_back(monster);
	*/
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
	
	getChildByTag(TAG_UNIT)->addChild(sprite_death, ZORDER_MONSTER_DEATH);

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

void Game::addunit(float dt)
{
	char szFile[64] = { 0, };

	Size winSize = Director::getInstance()->getWinSize();
	Unit* unit = new Unit();

	unit->setBody("unit_01.png");
	unit->setDamage(70.f);
	unit->setRange(100.f);
	unit->setSpeed(2.f);
	unit->getBody()->setPosition(Point(winSize.width * 0.2, winSize.height * 0.5));
	
	getChildByTag(TAG_UNIT)->addChild(unit->getBody(), ZORDER_CHARACTER, TAG_CHARACTER);

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	arr_unit.push_back(unit);
}

void Game::addunit_mix(cocos2d::Point pt)
{

}

void Game::addunit_sell(cocos2d::Point pt)
{

}

float Game::calDistance(cocos2d::Point from, cocos2d::Point to)
{
	float x = from.x - to.x;
	float y = from.y - to.y;

	return sqrtf(pow(x, 2) + pow(y, 2));
}

void Game::addattack(Monster* monster)
{
	/*
	Size winSize = Director::getInstance()->getWinSize();

	float distance = calDistance(from, to);
	
	Sprite* sprite_bullet = Sprite::createWithSpriteFrameName("bullet.png");
	sprite_bullet->setPosition(from);

	MoveTo* moveTo = MoveTo::create(0.03f, to);
	MoveTo* stop = MoveTo::create(unit->getMaxSpeed() * 0.2, from);
	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(Game::selfRemover, this));
	Sequence* sequence = Sequence::create(stop, moveTo, callfunc, NULL);

	sprite_bullet->runAction(sequence);
	
	getChildByTag(TAG_UNIT)->addChild(sprite_bullet, ZORDER_ATK);
	*/
	//cocos2d::Point from, cocos2d::Point to, Unit* unit
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
	Size winSize = Director::getInstance()->getWinSize();
	Unit* unit = NULL;
	Monster* monster = NULL;

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
					unit->setCurSpeed(0);
					unit_atk_motion(unit);
					addattack(monster);
					//addattack(unit->getBody()->getPosition(), monster->getBody()->getPosition(), unit);

					if (0 >= monster->subEnergy(unit->getDamage()))
					{
						addmonster_death(monster->getBody()->getPosition());
						monster->release();
						delete monster;
						iterMonster = arr_monster.erase(iterMonster);

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

void Game::unit_atk_motion(Unit* unit)
{
	char szFile[64] = { 0, };

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	for (int i = 2; i < 4; i++)
	{
		sprintf(szFile, "unit_%02d.png", i);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation->addSpriteFrame(frameCache->getSpriteFrameByName("unit_01.png"));
	animation->setDelayPerUnit(unit->getMaxSpeed() * 0.1f);

	Animate* animate = Animate::create(animation);
	
	unit->getBody()->runAction(animate);
}

void Game::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
	touch = true;
	touch_point = touches[0]->getLocation();
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

void Game::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	touch = false;
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
	Point movePoint = touches[0]->getLocation();
	Point distance = touch_point - movePoint;
	Point destination = this->getPosition() - distance;

	if (destination.x <= -anc_width)
		destination.setPoint(-anc_width, destination.y);
	if (destination.y <= -anc_height)
		destination.setPoint(destination.x, -anc_height);
	if (destination.x >= 0)
		destination.setPoint(0, destination.y);
	if (destination.y >= 0)
		destination.setPoint(destination.x, 0);

	touch_point = movePoint;

	if (destination.x <= -anc_width && destination.y <= -anc_height)
		return;

	if (destination.x >= 0 && destination.y >= 0)
		return;

	MoveTo* moveto = MoveTo::create(0.f, destination);

	this->runAction(moveto);
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