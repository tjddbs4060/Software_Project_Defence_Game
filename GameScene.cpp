#include "GameScene.h"

USING_NS_CC;

Game::Game() :cur_leave(0), cur_level(0), cur_monster(0) {}

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

	schedule(schedule_selector(Game::addmonster), 0.8f);
	schedule(schedule_selector(Game::unit_atk_cooltime));
	schedule(schedule_selector(Game::unit_atk_monster));
	addunit(0.1f);

	return true;
}

void Game::addmonster(float dt)
{
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


/////////////////////////////////////////////////////////////////
/*
#include "StartScene.h"

USING_NS_CC;

Scene* Start::scene()
{
	auto scene = Scene::create();
	auto layer = Start::create();

	scene->addChild(layer);

	return scene;
}

bool Start::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size winSize = Director::getInstance()->getWinSize();
	auto spriteEX = Sprite::create("exam.png");
	auto spriteCH = Sprite::create("ch.png");
	//spriteEX->setAnchorPoint(Point(0.5, 0.5));
	spriteEX->setPosition(Point(winSize.width / 2, winSize.height / 2));
	spriteCH->setPosition(Point(winSize.width / 2, winSize.height * 0.2));

	addChild(spriteCH, 1);
	addChild(spriteEX, 0);
	schedule(schedule_selector(Start::addenm), 1.0f);
	schedule(schedule_selector(Start::shoot), 0.6f);
	schedule(schedule_selector(Start::dropBomb), 4.0f);
	//unschedule(schedule_selector(Start::addenm));

	Sprite* spriteRotateRadar = Sprite::create("radar_01.png");

	spriteRotateRadar->setAnchorPoint(Point(1, 0));
	spriteRotateRadar->setPosition(Point(winSize.width * 0.5, winSize.height * 0.25));

	float scale = winSize.height / spriteRotateRadar->getContentSize().height;
	spriteRotateRadar->setScale(scale);
	addChild(spriteRotateRadar, 2);

	RotateBy* rotateBy = RotateBy::create(10, 360);
	RepeatForever* repeatForever = RepeatForever::create(rotateBy);
	spriteRotateRadar->runAction(repeatForever);

	Sprite* spriteScaleRadar = Sprite::create("radar_00.png");
	spriteScaleRadar->setPosition(Point(winSize.width * 0.5, winSize.height * 0.25));
	spriteScaleRadar->setScale(0);
	addChild(spriteScaleRadar, 3);

	scale = winSize.height / spriteScaleRadar->getContentSize().height;
	ScaleTo* scaleTo = ScaleTo::create(4, scale);
	ScaleTo* scaleFrom = ScaleTo::create(0, 0);
	Sequence* sequenceScale = Sequence::create(scaleTo, scaleFrom, NULL);
	RepeatForever* scaleForever = RepeatForever::create(sequenceScale);
	spriteScaleRadar->runAction(scaleForever);

	Sprite* spriteFadeRadar = Sprite::create("radar_00.png");
	spriteFadeRadar->setPosition(Point(winSize.width * 0.5, winSize.height * 0.25));
	spriteFadeRadar->setScale(scale);
	addChild(spriteFadeRadar, 4);

	FadeOut* fadeOut = FadeOut::create(4);
	FadeIn* fadeIn = FadeIn::create(0);
	Sequence* sequenceFade = Sequence::create(fadeOut, fadeIn, NULL);
	RepeatForever* fadeForever = RepeatForever::create(sequenceFade);
	spriteFadeRadar->runAction(fadeForever);

	return true;
}

void Start::addenm(float dt)
{
	Size winSize = Director::getInstance()->getWinSize();

	auto spriteEn = Sprite::create("en.png");
	float Xpos = rand() % (int)winSize.width;
	float Ypos = winSize.height;
	spriteEn->setPosition(Point(Xpos, Ypos));
	spriteEn->setAnchorPoint(Point(0.5, 0));

	addChild(spriteEn, 2);

	//MoveTo* moveTo = MoveTo::create(2.f, Point(winSize.width * 0.5, winSize.height * 0.25));
	//spriteEn->runAction(moveTo);

	Point position = spriteEn->getPosition();

	float xDistance = (winSize.width / 2) - position.x;

	ccBezierConfig bezierConfig;

	bezierConfig.controlPoint_1 = Point(position.x, winSize.height * 0.4f);
	bezierConfig.controlPoint_2 = Point(position.x + (xDistance / 3), winSize.height * 0.2f);
	bezierConfig.endPosition = Point(winSize.width * 0.5, winSize.height * 0.25);

	BezierTo* bezierTo = BezierTo::create(10.f, bezierConfig);

	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(Start::selfRemover, this));

	Sequence* sequence = Sequence::create(bezierTo, callfunc, NULL);
	spriteEn->runAction(sequence);

	/*
	Size winSize = Director::getInstance()->getWinSize();

	auto spriteEn = Sprite::create("en.png");
	float Xpos = winSize.width * 0.1;
	float Ypos = winSize.height * 0.9;
	spriteEn->setPosition(Point(Xpos, Ypos));

	addChild(spriteEn, 9);

	//MoveTo* moveTo = MoveTo::create(2.f, Point(winSize.width * 0.5, winSize.height * 0.25));
	//spriteEn->runAction(moveTo);

	auto actionTo1 = MoveTo::create(winSize.height / 200, Point(Xpos, winSize.height * 0.1));
	auto actionTo2 = MoveTo::create(winSize.width / 200, Point(winSize.width * 0.9, winSize.height * 0.1));
	auto actionTo3 = MoveTo::create(winSize.height / 200, Point(winSize.width * 0.9, Ypos));
	auto actionTo4 = MoveTo::create(winSize.width / 200, Point(Xpos, Ypos));
	auto seq = Sequence::create(actionTo1, actionTo2, actionTo3, actionTo4, NULL);
	auto rep1 = Repeat::create(seq, 3);

	//pauseSchedulerAndActions(); 액션 일시정지
	//resulmeSchedulerAndActions(); 액션 재시작

	spriteEn->runAction(rep1);
	
}

void Start::shoot(float dt)
{
	Size winSize = Director::getInstance()->getWinSize();

	Sprite* spriteBullet = Sprite::create("sh.png");

	float scale = (winSize.height / 30) / spriteBullet->getContentSize().height;
	spriteBullet->setScale(scale);

	spriteBullet->setPosition(Point(winSize.width * 0.5, winSize.height * 0.25));

	addChild(spriteBullet, 10);

	MoveBy* moveBy = MoveBy::create(1.f, Point(0, winSize.height / 2));

	ActionInterval* actionInterval = EaseOut::create(moveBy, 1.1f);

	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(Start::selfRemover, this));

	Sequence* sequence = Sequence::create(actionInterval, callfunc, NULL);
	spriteBullet->runAction(sequence);
}

void Start::dropBomb(float dt)
{
	Size winSize = Director::getInstance()->getWinSize();

	Sprite* spriteBomb = Sprite::create("MISSILE.png");

	float bombHeight = spriteBomb->getContentSize().height;

	float beginScale = (winSize.height / 6) / bombHeight;
	float finishScale = (winSize.height / 40) / bombHeight;

	spriteBomb->setScale(beginScale);
	float visibleHeight = spriteBomb->getBoundingBox().size.height;
	spriteBomb->setPosition(Point(winSize.width / 2, 0 - (visibleHeight / 2)));

	addChild(spriteBomb, 5);

	MoveBy* moveBy = MoveBy::create(1.5, Point(0, winSize.height * 0.66));
	ActionInterval* actionInterval = EaseOut::create(moveBy, 1.8);
	ScaleTo* scaleTo = ScaleTo::create(1.5, finishScale);
	Spawn* spawn = Spawn::create(actionInterval, scaleTo, NULL);

	CallFuncN* callfunc = CallFuncN::create(CC_CALLBACK_1(Start::selfRemover, this));

	Sequence* sequence = Sequence::create(spawn, callfunc, NULL);
	spriteBomb->runAction(sequence);
}

void Start::selfRemover(Node* sender)
{
	sender->removeFromParentAndCleanup(true);
}
*/