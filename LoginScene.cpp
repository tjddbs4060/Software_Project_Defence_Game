#include "LoginScene.h"

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

	return true;
}
