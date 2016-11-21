#include "GameOver.h"

#pragma execution_character_set("UTF-8")

USING_NS_CC;

GameOver::GameOver() : touch_enable(false)
{
}

Scene* GameOver::scene(char* name)
{
	Scene* scene = Scene::create();
	GameOver* game = GameOver::create();
	game->setID(name);
	scene->addChild(game);

	return scene;
}

bool GameOver::init()
{
	if (!Layer::init())
		return false;

	Size winSize = Director::getInstance()->getWinSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Thing.plist");
	SpriteBatchNode* spriteBatchNodeGameOver = SpriteBatchNode::create("Thing.png");
	addChild(spriteBatchNodeGameOver);

	Sprite* background = Sprite::createWithSpriteFrameName("gameover.png");
	background->setPosition(Point(winSize.width / 2, winSize.height / 2));
	background->setScale(winSize.width / background->getContentSize().width);
	spriteBatchNodeGameOver->addChild(background);

	schedule(schedule_selector(GameOver::waitTime), 3.f);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(GameOver::onTouchesBegan, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(GameOver::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(GameOver::onTouchesEnded, this);
	listener->onTouchesMoved = CC_CALLBACK_2(GameOver::onTouchesMoved, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	return true;
}

void GameOver::setID(char* name)
{
	strcpy(id, name);
}

char* GameOver::getID()
{
	return id;
}

void GameOver::waitTime(float dt)
{
	touch_enable = true;

	unschedule(schedule_selector(GameOver::waitTime));
}

void GameOver::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{

}

void GameOver::onTouchesCancelled(const std::vector<Touch*>& touches, Event *event)
{

}

void GameOver::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	if (touch_enable == true)
	{
		_eventDispatcher->autorelease();
		_eventDispatcher->removeAllEventListeners();

		Scene* title = TitleScene::scene(getID());
		TransitionScene* transition = TransitionFade::create(1.5f, title);
		Director::getInstance()->replaceScene(transition);
	}
}

void GameOver::onTouchesMoved(const std::vector<Touch*>& touches, Event *event)
{

}