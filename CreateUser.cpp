#include "CreateUser.h"

#pragma execution_character_set("UTF-8")

USING_NS_CC;

Scene* CreateUser::scene()
{
	Scene* scene = Scene::create();
	CreateUser* createUser = CreateUser::create();
	scene->addChild(createUser);

	return scene;
}

bool CreateUser::init()
{
	if (!Layer::init())
		return false;

	Size winSize = Director::getInstance()->getWinSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Menu.plist");
	SpriteBatchNode* spriteBatchNodeLogin = SpriteBatchNode::create("Menu.png");
	spriteBatchNodeLogin->setTag(TAG_TITLE_MENU);
	addChild(spriteBatchNodeLogin);

	Sprite* sprite_background = Sprite::createWithSpriteFrameName("title_background.png");
	sprite_background->setPosition(Point(winSize.width / 2, winSize.height / 2));
	sprite_background->setScale(winSize.width / sprite_background->getContentSize().width);
	spriteBatchNodeLogin->addChild(sprite_background);

	Sprite* menuNormal = Sprite::createWithSpriteFrameName("create.png");
	MenuItemSprite* menuMember = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(CreateUser::onMenu, this));
	menuMember->setTag(TAG_MEMBER);
	menuMember->setPosition(Point(30, -100));

	menuNormal = Sprite::createWithSpriteFrameName("back.png");
	MenuItemSprite* menuLogin = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(CreateUser::onMenu, this));
	menuLogin->setTag(TAG_LOGIN);
	menuLogin->setPosition(Point(-40, -100));

	Menu* menu = Menu::create(menuLogin, menuMember, NULL);
	addChild(menu, ZORDER_MENU, TAG_MENU);

	CCTextFieldTTF *textfield_id = CCTextFieldTTF::textFieldWithPlaceHolder("Please, input your ID.", CCSize(480, 30), kCCTextAlignmentCenter, "Arial", 20);
	textfield_id->setAnchorPoint(Point(0, 0));
	textfield_id->setPosition(Point(0, 150));
	textfield_id->setTag(TAG_ID);
	this->addChild(textfield_id);

	CCTextFieldTTF *textfield_pw = CCTextFieldTTF::textFieldWithPlaceHolder("Please, input your PW.", CCSize(480, 30), kCCTextAlignmentCenter, "Arial", 20);
	textfield_pw->setAnchorPoint(Point(0, 0));
	textfield_pw->setPosition(Point(0, 100));
	textfield_pw->setTag(TAG_PW);
	this->addChild(textfield_pw);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(CreateUser::onTouchesBegan, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(CreateUser::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(CreateUser::onTouchesEnded, this);
	listener->onTouchesMoved = CC_CALLBACK_2(CreateUser::onTouchesMoved, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	return true;
}

void CreateUser::keyboardWillShow(CCIMEKeyboardNotificationInfo &info)
{
	CCTextFieldTTF *textfield = (CCTextFieldTTF*)this->getChildByTag(TAG_ID);
	textfield->setString("");
}

void CreateUser::keyboardWillHide(CCIMEKeyboardNotificationInfo &info)
{
	/*
	CCTextFieldTTF *textfield = (CCTextFieldTTF*)this->getChildByTag(200);
	CCLabelTTF *label = (CCLabelTTF*)this->getChildByTag(200);

	label->setString(textfield->getString());
	*/
}

void CreateUser::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{
	CCPoint pt = touches[0]->getLocation();
	//pt = CCDirector::sharedDirector()->convertToGL(pt);

	CCTextFieldTTF *textfield_id = (CCTextFieldTTF*)this->getChildByTag(TAG_ID);
	CCTextFieldTTF *textfield_pw = (CCTextFieldTTF*)this->getChildByTag(TAG_PW);
	CCRect rt_id = textfield_id->boundingBox();
	CCRect rt_pw = textfield_pw->boundingBox();

	if (rt_id.containsPoint(pt))
		textfield_id->attachWithIME();
	else if (rt_pw.containsPoint(pt))
		textfield_pw->attachWithIME();
}

void CreateUser::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{

}

void CreateUser::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{

}

void CreateUser::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{

}

void CreateUser::get_db_data(char * data, int port)
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
	request->setResponseCallback(CC_CALLBACK_2(CreateUser::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void CreateUser::onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response)
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

	if (!strcmp(compare, "success"))
	{
		sound_title_menu();

		_eventDispatcher->autorelease();
		_eventDispatcher->removeAllEventListeners();

		Director::getInstance()->replaceScene(LoginScene::scene());
	}
	else if (!strcmp(compare, "fail_member"))
	{
		//이미 존재하는 ID라는 표시
		sound_title_fail();
	}
}

void CreateUser::onMenu(cocos2d::Object* sender)
{
	char szFile[64] = { 0, };

	switch (((Node*)sender)->getTag())
	{
	case TAG_LOGIN:
		sound_title_menu();

		_eventDispatcher->autorelease();
		_eventDispatcher->removeAllEventListeners();

		Director::getInstance()->replaceScene(LoginScene::scene());
		break;
	case TAG_MEMBER:
		CCTextFieldTTF *textfield_id = (CCTextFieldTTF*)this->getChildByTag(TAG_ID);
		CCTextFieldTTF *textfield_pw = (CCTextFieldTTF*)this->getChildByTag(TAG_PW);

		sprintf(szFile, "member/%s/%s", textfield_id->getString().data(), textfield_pw->getString().data());
		
		get_db_data(szFile, DEFENCEJS);

		break;
	}
}

void CreateUser::sound_title_menu()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("title_menu.wav");
}

void CreateUser::sound_title_fail()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("title_fail.wav");
}