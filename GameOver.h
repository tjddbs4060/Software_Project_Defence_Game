#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__

#include "cocos2d.h"
#include "Order.h"
#include "TitleScene.h"

class GameOver : public cocos2d::Layer
{
public:
	GameOver();

	virtual bool init();
	static cocos2d::Scene* scene(char* name);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;

	CREATE_FUNC(GameOver);

private:
	void setID(char* name);
	char* getID();

	void waitTime(float dt);

	bool touch_enable;
	char id[32];
};

#endif