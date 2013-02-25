

#include "AppDelegate.h"
#include "Script/LuaHelper.h"

#include "SceneManager.h"
#include "CCLuaEngine.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"


namespace AlphaDig
{

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
	SimpleAudioEngine::sharedEngine()->end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    TargetPlatform target = getTargetPlatform();

    if (target == kTargetIpad)
    {
        // ipad

        if (pDirector->enableRetinaDisplay(true))
        {
            // ipad hd
            CCFileUtils::sharedFileUtils()->setResourceDirectory("ipadhd");
        }
        else
        {
            CCFileUtils::sharedFileUtils()->setResourceDirectory("ipad");
        }
    }
    else if (target == kTargetIphone)
    {
        // iphone

        if (pDirector->enableRetinaDisplay(true))
        {
            // iphone hd
            CCFileUtils::sharedFileUtils()->setResourceDirectory("hd");
        }
    }

    //script config
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    std::string dirPath = "Scripts";
    LuaHelper::s_executeFile( "resources_define.lua");
    LuaHelper::s_executeFile( "ui_texts_chs.lua");

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    SceneManager::startMainMenuScene();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

}
