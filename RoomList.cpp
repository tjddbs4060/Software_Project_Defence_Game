#include "RoomList.h"

#pragma execution_character_set("UTF-8")

USING_NS_CC;

Scene* RoomList::scene(char* name)
{
	Scene* scene = Scene::create();
	RoomList* room = RoomList::create();
	room->setID(name);
	scene->addChild(room);

	return scene;
}

bool RoomList::init()
{
	if (!Layer::init())
		return false;

	Size winSize = Director::getInstance()->getWinSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	SpriteBatchNode* spriteBatchNodeMenu = SpriteBatchNode::create("Menu.png");
	addChild(spriteBatchNodeMenu);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Hero.plist");
	SpriteBatchNode* spriteBatchNodeHero = SpriteBatchNode::create("Hero.png");
	addChild(spriteBatchNodeHero);

	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

	Sprite* sprite_background = Sprite::createWithSpriteFrameName("title_background.png");
	sprite_background->setPosition(Point(winSize.width / 2, winSize.height / 2));
	sprite_background->setScale(winSize.width / sprite_background->getContentSize().width);
	spriteBatchNodeMenu->addChild(sprite_background);

	char szFile[64] = { 0, };

	Sprite* sprite_left = Sprite::createWithSpriteFrameName("jenos.png");
	Sprite* sprite_right = Sprite::createWithSpriteFrameName("saitama.png");

	sprite_left->setPosition(Point(winSize.width / 2 - 100, winSize.height / 2));
	sprite_right->setPosition(Point(winSize.width / 2 + 100, winSize.height / 2));

	auto animation_left = Animation::create();
	auto animation_right = Animation::create();

	for (int i = 0; i < 4; i++)
	{
		sprintf(szFile, "jenos_right_%d.png", i);
		animation_left->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	for (int i = 0; i < 4; i++)
	{
		sprintf(szFile, "jenos_right_atk_%d.png", i);
		animation_left->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	
	animation_left->setDelayPerUnit(0.05f);

	for (int i = 0; i < 4; i++)
	{
		sprintf(szFile, "saitama_left_%d.png", i);
		animation_right->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	for (int i = 0; i < 4; i++)
	{
		sprintf(szFile, "saitama_left_atk_%d.png", i);
		animation_right->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}

	animation_left->setDelayPerUnit(0.1f);
	animation_right->setDelayPerUnit(0.1f);

	Animate* animate_left = Animate::create(animation_left);
	Animate* animate_right = Animate::create(animation_right);

	RepeatForever* repeat_left = RepeatForever::create(animate_left);
	RepeatForever* repeat_right = RepeatForever::create(animate_right);

	sprite_left->runAction(repeat_left);
	sprite_right->runAction(repeat_right);

	this->addChild(sprite_left);
	this->addChild(sprite_right);

	CCTextFieldTTF *textfield = CCTextFieldTTF::textFieldWithPlaceHolder("친구들 부르는 중...", CCSize(480, 80), kCCTextAlignmentCenter, "Arial", 25);
	textfield->setPosition(Point(winSize.width / 2, winSize.height / 2));
	textfield->setColor(Color3B::YELLOW);
	textfield->enableBold();
	this->addChild(textfield);

	auto listener = EventListenerTouchAllAtOnce::create();

	listener->onTouchesBegan = CC_CALLBACK_2(RoomList::onTouchesBegan, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(RoomList::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(RoomList::onTouchesEnded, this);
	listener->onTouchesMoved = CC_CALLBACK_2(RoomList::onTouchesMoved, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	schedule(schedule_selector(RoomList::server_continue), 0.9f);

	return true;
}

void RoomList::onMenu(cocos2d::Object* sender)
{
	char szFile[32] = { 0, };

	switch (((Node*)sender)->getTag())
	{
		//방 번호 입장
	}
}

void RoomList::setID(char* name)
{
	strcpy(id, name);
}

char* RoomList::getID()
{
	return id;
}

void RoomList::get_db_data(char * data, int port)
{
	__String * dataToSend = __String::create(data);
	char szFile[32] = { 0, };

	sprintf(szFile, "http://192.168.219.102:%d", port);

	cocos2d::network::HttpRequest * request = new cocos2d::network::HttpRequest();
	request->setUrl(szFile);
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setRequestData(dataToSend->getCString(), dataToSend->length());
	request->setResponseCallback(CC_CALLBACK_2(RoomList::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void RoomList::onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response)
{
	std::vector<char> * buffer = response->getResponseData();

	char szFile[128] = { 0, };
	char temp[128] = { 0, };
	char * compare;
	for (unsigned int i = 0; i < buffer->size(); i++)
		szFile[i] = (*buffer)[i];

	if (buffer->size() == 0)
		return;

	strcpy(temp, szFile);
	compare = strtok(temp, "/");

	if (!strcmp(compare, "full_room"))
	{
		strtok(szFile, "/");
		char * num = strtok(NULL, "/");

		unschedule(schedule_selector(RoomList::server_continue));

		_eventDispatcher->autorelease();
		_eventDispatcher->removeAllEventListeners();

		if (atoi(num) == 1)
			Director::getInstance()->replaceScene(Game::scene(getID()));	//방장
		else
			Director::getInstance()->replaceScene(Client::scene(getID()));	//일반
	}
	else if (!strcmp(compare, "empty_room"))
	{
		//오류
	}
}

void RoomList::server_continue(float dt)
{
	char szFile[32] = { 0, };

	sprintf(szFile, "room_person/%s", getID());

	get_db_data(szFile, DEFENCEJS);
}