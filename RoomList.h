#ifndef __ROOM_LIST_H__
#define __ROOM_LIST_H__

#include "cocos2d.h"
#include "Order.h"
#include "GameScene.h"
#include "ClientGameScene.h"

#include "network/HttpClient.h"
#include "network/HttpConnection-winrt.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/SocketIO.h"

class RoomList : public cocos2d::Layer
{
public:
	virtual bool init();
	static cocos2d::Scene* scene(char* name);

	void onMenu(cocos2d::Object* sender);

	CREATE_FUNC(RoomList);

private:
	void setID(char* name);
	char* getID();

	void get_db_data(char * data, int port);
	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);

	void server_continue(float dt);	//서버와 통신

	char id[32];
};

#endif