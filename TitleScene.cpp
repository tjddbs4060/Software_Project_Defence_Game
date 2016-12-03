#include "TitleScene.h"

#pragma execution_character_set("UTF-8")

USING_NS_CC;

Scene* TitleScene::scene(char* name)
{
	Scene* scene = Scene::create();
	TitleScene* title = TitleScene::create();
	title->setID(name);
	scene->addChild(title);

	return scene;
}

bool TitleScene::init()
{
	if (!Layer::init())
		return false;

	Size winSize = Director::getInstance()->getWinSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	SpriteBatchNode* spriteBatchNodeMenu = SpriteBatchNode::create("Menu.png");
	spriteBatchNodeMenu->setTag(TAG_TITLE_MENU);
	addChild(spriteBatchNodeMenu);

	Sprite* sprite_background = Sprite::createWithSpriteFrameName("title_background.png");
	sprite_background->setPosition(Point(winSize.width / 2, winSize.height / 2));
	sprite_background->setScale(winSize.width / sprite_background->getContentSize().width);
	spriteBatchNodeMenu->addChild(sprite_background);

	Sprite* menuNormal = Sprite::createWithSpriteFrameName("single_menu_normal.png");
	Sprite* menuSelect = Sprite::createWithSpriteFrameName("single_menu_select.png");
	MenuItemSprite* menuSingle = MenuItemSprite::create(menuNormal, menuSelect, CC_CALLBACK_1(TitleScene::onMenu, this));
	menuSingle->setTag(TAG_TITLE_MENU_SINGLE);
	menuSingle->setPosition(Point(0, 15));
	menuSingle->setScale(0.5);

	menuNormal = Sprite::createWithSpriteFrameName("multi_menu_normal.png");
	menuSelect = Sprite::createWithSpriteFrameName("multi_menu_select.png");
	MenuItemSprite* menuMulti = MenuItemSprite::create(menuNormal, menuSelect, CC_CALLBACK_1(TitleScene::onMenu, this));
	menuMulti->setTag(TAG_TITLE_MENU_MULTI);
	menuMulti->setPosition(Point(0, -30));
	menuMulti->setScale(0.5);

	menuNormal = Sprite::createWithSpriteFrameName("option_menu_normal.png");
	menuSelect = Sprite::createWithSpriteFrameName("option_menu_select.png");
	MenuItemSprite* menuOption = MenuItemSprite::create(menuNormal, menuSelect, CC_CALLBACK_1(TitleScene::onMenu, this));
	menuOption->setTag(TAG_TITLE_MENU_OPTION);
	menuOption->setPosition(Point(0, -75));
	menuOption->setScale(0.5);

	menuNormal = Sprite::createWithSpriteFrameName("exit_menu_normal.png");
	menuSelect = Sprite::createWithSpriteFrameName("exit_menu_select.png");
	MenuItemSprite* menuExit = MenuItemSprite::create(menuNormal, menuSelect, CC_CALLBACK_1(TitleScene::onMenu, this));
	menuExit->setTag(TAG_TITLE_MENU_EXIT);
	menuExit->setPosition(Point(0, -120));
	menuExit->setScale(0.5);

	Menu* menu = Menu::create(menuSingle, menuMulti, menuOption, menuExit, NULL);
	addChild(menu, ZORDER_MENU, TAG_MENU);

	return true;
}

void TitleScene::onMenu(cocos2d::Object* sender)
{
	char szFile[32] = { 0, };

	sound_menu_button();

	switch (((Node*)sender)->getTag())
	{
	case TAG_TITLE_MENU_SINGLE:
		//게임화면 연출
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

		_eventDispatcher->autorelease();
		_eventDispatcher->removeAllEventListeners();

		sprintf(szFile, "single/%s", getID());

		get_db_data(szFile, DEFENCEJS);

		//싱글 플레이어로
		Director::getInstance()->replaceScene(SingleGame::scene(getID()));
		break;
	case TAG_TITLE_MENU_MULTI:
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();

		_eventDispatcher->autorelease();
		_eventDispatcher->removeAllEventListeners();

		sprintf(szFile, "join_room/%s", getID());

		get_db_data(szFile, DEFENCEJS);

		Director::getInstance()->replaceScene(RoomList::scene(getID()));
		break;
	case TAG_TITLE_MENU_OPTION:
		break;
	case TAG_TITLE_MENU_EXIT:
		break;
	}
}

void TitleScene::setID(char* name)
{
	strcpy(id, name);
}

char* TitleScene::getID()
{
	return id;
}

void TitleScene::get_db_data(char * data, int port)
{
	__String * dataToSend = __String::create(data);
	char szFile[32] = { 0, };

	sprintf(szFile, "http://192.168.219.102:%d", port);

	cocos2d::network::HttpRequest * request = new cocos2d::network::HttpRequest();
	request->setUrl(szFile);
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setRequestData(dataToSend->getCString(), dataToSend->length());
	request->setResponseCallback(CC_CALLBACK_2(TitleScene::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void TitleScene::onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response)
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
}

void TitleScene::sound_menu_button()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu_button.wav");
}

void TitleScene::sound_w_off()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("w_off.wav");
}

void TitleScene::sound_w_on()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("w_on.wav");
}