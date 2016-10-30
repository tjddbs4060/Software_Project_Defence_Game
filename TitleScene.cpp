#include "TitleScene.h"

USING_NS_CC;

Scene* TitleScene::scene()
{
	Scene* scene = Scene::create();
	TitleScene* title = TitleScene::create();
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
	menuSingle->setPosition(Point(0, 40));

	menuNormal = Sprite::createWithSpriteFrameName("multi_menu_normal.png");
	menuSelect = Sprite::createWithSpriteFrameName("multi_menu_select.png");
	MenuItemSprite* menuMulti = MenuItemSprite::create(menuNormal, menuSelect, CC_CALLBACK_1(TitleScene::onMenu, this));
	menuMulti->setTag(TAG_TITLE_MENU_MULTI);
	menuMulti->setPosition(Point(0, -10));

	menuNormal = Sprite::createWithSpriteFrameName("option_menu_normal.png");
	menuSelect = Sprite::createWithSpriteFrameName("option_menu_select.png");
	MenuItemSprite* menuOption = MenuItemSprite::create(menuNormal, menuSelect, CC_CALLBACK_1(TitleScene::onMenu, this));
	menuOption->setTag(TAG_TITLE_MENU_OPTION);
	menuOption->setPosition(Point(0, -60));

	menuNormal = Sprite::createWithSpriteFrameName("exit_menu_normal.png");
	menuSelect = Sprite::createWithSpriteFrameName("exit_menu_select.png");
	MenuItemSprite* menuExit = MenuItemSprite::create(menuNormal, menuSelect, CC_CALLBACK_1(TitleScene::onMenu, this));
	menuExit->setTag(TAG_TITLE_MENU_EXIT);
	menuExit->setPosition(Point(0, -110));

	Menu* menu = Menu::create(menuSingle, menuMulti, menuOption, menuExit, NULL);
	addChild(menu, ZORDER_MENU, TAG_MENU);

	return true;
}

void TitleScene::onMenu(cocos2d::Object* sender)
{
	switch (((Node*)sender)->getTag())
	{
	case TAG_TITLE_MENU_SINGLE:
		//게임화면 연출

		Director::getInstance()->replaceScene(Game::scene());
		break;
	case TAG_TITLE_MENU_MULTI:
		break;
	case TAG_TITLE_MENU_OPTION:
		break;
	case TAG_TITLE_MENU_EXIT:
		break;
	}
}