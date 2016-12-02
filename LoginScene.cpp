#include "LoginScene.h"

#pragma execution_character_set("UTF-8")

USING_NS_CC;

Scene* LoginScene::scene()
{
	Scene* scene = Scene::create();
	LoginScene* loginScene = LoginScene::create();
	scene->addChild(loginScene);

	return scene;
}

bool LoginScene::init()
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

	Sprite* menuNormal = Sprite::createWithSpriteFrameName("login.png");
	MenuItemSprite* menuLogin = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(LoginScene::onMenu, this));
	menuLogin->setTag(TAG_LOGIN);
	menuLogin->setPosition(Point(-40, -40));

	menuNormal = Sprite::createWithSpriteFrameName("member.png");
	MenuItemSprite* menuMember = MenuItemSprite::create(menuNormal, menuNormal, CC_CALLBACK_1(LoginScene::onMenu, this));
	menuMember->setTag(TAG_MEMBER);
	menuMember->setPosition(Point(30, -40));

	Menu* menu = Menu::create(menuLogin, menuMember, NULL);
	addChild(menu, ZORDER_MENU, TAG_MENU);

	CCTextFieldTTF *textfield_id = CCTextFieldTTF::textFieldWithPlaceHolder("Please, input your ID.", CCSize(480, 30), kCCTextAlignmentCenter, "Arial", 20);
	textfield_id->setAnchorPoint(Point(0, 0));
	textfield_id->setPosition(Point(0, 200));
	textfield_id->setTag(TAG_ID);
	this->addChild(textfield_id);

	CCTextFieldTTF *textfield_pw = CCTextFieldTTF::textFieldWithPlaceHolder("Please, input your PW.", CCSize(480, 30), kCCTextAlignmentCenter, "Arial", 20);
	textfield_pw->setAnchorPoint(Point(0, 0));
	textfield_pw->setPosition(Point(0, 150));
	textfield_pw->setTag(TAG_PW);
	this->addChild(textfield_pw);

	auto listener = EventListenerTouchAllAtOnce::create();

	listener->onTouchesBegan = CC_CALLBACK_2(LoginScene::onTouchesBegan, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(LoginScene::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(LoginScene::onTouchesEnded, this);
	listener->onTouchesMoved = CC_CALLBACK_2(LoginScene::onTouchesMoved, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	return true;
}

void LoginScene::keyboardWillShow(CCIMEKeyboardNotificationInfo &info)
{
	CCTextFieldTTF *textfield = (CCTextFieldTTF*)this->getChildByTag(TAG_ID);
	textfield->setString("");
}

void LoginScene::keyboardWillHide(CCIMEKeyboardNotificationInfo &info)
{
	/*
	CCTextFieldTTF *textfield = (CCTextFieldTTF*)this->getChildByTag(200);
	CCLabelTTF *label = (CCLabelTTF*)this->getChildByTag(200);

	label->setString(textfield->getString());
	*/
}

void LoginScene::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
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

void LoginScene::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{

}

void LoginScene::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{

}

void LoginScene::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event)
{

}

void LoginScene::get_db_data(char * data, int port)
{
	__String * dataToSend = __String::create(data);
	char szFile[32] = { 0, };

	sprintf(szFile, "localhost:%d", port);

	cocos2d::network::HttpRequest * request = new cocos2d::network::HttpRequest();
	request->setUrl(szFile);
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setRequestData(dataToSend->getCString(), dataToSend->length());
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpRequestCompleted, this));
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void LoginScene::onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response)
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
		strtok(szFile, "/");
		char * id = strtok(NULL, "/");

		_eventDispatcher->autorelease();
		_eventDispatcher->removeAllEventListeners();

		Director::getInstance()->replaceScene(TitleScene::scene(id));
	}
	else if (!strcmp(compare, "fail_login"))
	{
		//로그인 다시하달라는 MoveTo 라벨 출력(생겼다 사라지는)
	}
}

void LoginScene::onMenu(cocos2d::Object* sender)
{	
	CCTextFieldTTF *textfield_id = (CCTextFieldTTF*)this->getChildByTag(TAG_ID);
	CCTextFieldTTF *textfield_pw = (CCTextFieldTTF*)this->getChildByTag(TAG_PW);
	char szFile[64] = { 0, };

	switch (((Node*)sender)->getTag())
	{
	case TAG_LOGIN:
		sprintf(szFile, "login/%s/%s", textfield_id->getString().data(), textfield_pw->getString().data());
		
		get_db_data(szFile, DEFENCEJS);
		break;
	case TAG_MEMBER:
		_eventDispatcher->autorelease();
		_eventDispatcher->removeAllEventListeners();

		Director::getInstance()->replaceScene(CreateUser::scene());

		break;
	}
}