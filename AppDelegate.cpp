#include "AppDelegate.h"
#include "GameScene.h"
#include "LoginScene.h"
#include "TitleScene.h"
#include "CreateUser.h"
#include "RoomList.h"
#include "GameOver.h"
#include "ClientGameScene.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Defence", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Defence");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
	/* /////////
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
	*/
    register_all_packages();

	CocosDenshion::SimpleAudioEngine* audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	audioEngine->setBackgroundMusicVolume(0.1);
	audioEngine->preloadBackgroundMusic("title_bgm.mp3");
	audioEngine->preloadBackgroundMusic("game_bgm.mp3");

	audioEngine->setEffectsVolume(0.7);
	audioEngine->preloadEffect("upgrade.wav");
	audioEngine->preloadEffect("gamble.wav");
	audioEngine->preloadEffect("capsule.wav");
	audioEngine->preloadEffect("gameover.wav");
	audioEngine->preloadEffect("ok_button.wav");
	audioEngine->preloadEffect("boss_create.wav");
	audioEngine->preloadEffect("menu_button.wav");
	audioEngine->preloadEffect("w_off.wav");
	audioEngine->preloadEffect("w_on.wav");
	audioEngine->preloadEffect("interface_open.wav");
	audioEngine->preloadEffect("title_menu.wav");
	audioEngine->preloadEffect("title_fail.wav");
	audioEngine->preloadEffect("hero_mix.wav");
	audioEngine->preloadEffect("stage.wav");
	audioEngine->preloadEffect("unit_move.wav");
	audioEngine->preloadEffect("boss_room_open.wav");
	audioEngine->preloadEffect("boss_delete.wav");
	
    // create a scene. it's an autorelease object
	/////////////////////
    //여기 밑 수정함
	//auto scene = Game::scene("asdf");
	//auto scene = GameOver::scene("asdf");
	auto scene = LoginScene::scene();
	//auto scene = TitleScene::scene("asdf");
	//auto scene = Client::scene("asdf");
	//auto scene = CreateUser::scene();
	//auto scene = RoomList::scene("asdf");
    // run
    director->runWithScene(scene);
	/////////////////////
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
