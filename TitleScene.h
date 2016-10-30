#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "Order.h"
#include "GameScene.h"

class TitleScene : public cocos2d::Layer
{
public:
	virtual bool init();
	static cocos2d::Scene* scene();

	void onMenu(cocos2d::Object* sender);

	CREATE_FUNC(TitleScene);
};

#endif