#ifndef __DEFENCE_TITLESCENE_H__
#define __DEFENCE_TITLESCENE_H__

#include "cocos2d.h"
#include "Order.h"

class LoginScene : public cocos2d::Layer
{
public:
	virtual bool init();
	static cocos2d::Scene* scene();
	CREATE_FUNC(LoginScene);
};

#endif