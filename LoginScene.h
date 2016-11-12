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

	void onHttpRequestCompleted(cocos2d::network::HttpClient * sender, cocos2d::network::HttpResponse * response);	//http ����
	void get_db_data(char * data, int port);		//http�� data ����

	void onMenu(cocos2d::Object* sender);	//�������̽� �޴�
};

#endif