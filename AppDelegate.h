#ifndef APP_DELEGATE_H__
#define APP_DELEGATE_H__

#include "cocos2d.h"

namespace AlphaDig
{

class AppDelegate: private cocos2d::CCApplication
{
public:

	/** 构造和析构
	 *
	 */
	AppDelegate();
	virtual ~AppDelegate();

	/**
	 @brief    Implement CCDirector and CCScene init code here.
	 @return true    Initialize success, app continue.
	 @return false   Initialize failed, app terminate.
	 */
	virtual bool applicationDidFinishLaunching();

	/**
	 @brief  The function be called when the application enter background
	 @param  the pointer of the application
	 */
	virtual void applicationDidEnterBackground();

	/**
	 @brief  The function be called when the application enter foreground
	 @param  the pointer of the application
	 */
	virtual void applicationWillEnterForeground();
};

}

#endif

