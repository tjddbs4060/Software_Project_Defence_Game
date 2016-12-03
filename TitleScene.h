#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "Order.h"
#include "GameScene.h"
#include "RoomList.h"
#include "SingleGameScene.h"

#include "network/HttpClient.h"
#include "network/HttpConnection-winrt.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/SocketIO.h"

class TitleScene : public cocos2d::Layer
{
public:
	virtual bool init();
	static cocos2d::Scene* scene(char* name);

	void onMenu(cocos2d::Object* sender);

	CREATE_FUNC(TitleScene);

private:
	void setID(char* name);
	char* getID();

	void get_db_data(char * data, int port);
	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);

	char id[32];

	void sound_menu_button();
	void sound_w_off();
	void sound_w_on();
};

#endif