#ifndef __DEFENCE_TITLESCENE_H__
#define __DEFENCE_TITLESCENE_H__

#include "cocos2d.h"
#include "Order.h"
#include "Use_String.h"
#include "TitleScene.h"
#include "CreateUser.h"

#include "network/HttpClient.h"
#include "network/HttpConnection-winrt.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/SocketIO.h"

class LoginScene : public cocos2d::Layer, public cocos2d::CCIMEDelegate
{
public:
	virtual bool init();
	static cocos2d::Scene* scene();
	CREATE_FUNC(LoginScene);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *event) override;

private:
	void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo &info);
	void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo & info);

	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);	//http 연결
	void get_db_data(char * data, int port);		//http에 data 전송

	void onMenu(cocos2d::Object* sender);	//인터페이스 메뉴
};

#endif