


#include "SceneManager.h"
#include "Layers/LayerMainMenu.h"
#include "Script/LuaHelper.h"

#include "menu_nodes/CCMenu.h"
#include "menu_nodes/CCMenuItem.h"
#include "platform/CCPlatformMacros.h"
#include "CCDirector.h"
#include "support/CCPointExtension.h"


namespace AlphaDig
{

LayerMainMenu::LayerMainMenu()
{

}

LayerMainMenu::~LayerMainMenu()
{

}


bool LayerMainMenu::initLayer()
{
	bool bRet = false;

	do
	{
		CCMenuItemFont *pPlayItem = CCMenuItemFont::create( LuaHelper::s_getStringVar("TEXT_PLAY").c_str(), this, menu_selector(LayerMainMenu::onPlay));
		CC_BREAK_IF(! pPlayItem);

		CCMenuItemFont *pQuitItem = CCMenuItemFont::create( LuaHelper::s_getStringVar("TEXT_QUIT").c_str(), this, menu_selector(LayerMainMenu::onQuit));
		CC_BREAK_IF(! pQuitItem);

		CCMenu* pMenu = CCMenu::create( pPlayItem, pQuitItem, NULL );
		CC_BREAK_IF(! pMenu);
		pMenu->alignItemsVerticallyWithPadding( 100.0f );

		addChild( pMenu );

		CCSize s = CCDirector::sharedDirector()->getWinSize();
		pMenu->setPosition(ccp(s.width/2, s.height/2));

		setKeypadEnabled(true);

		bRet = true;
	}
	while( false );

	return bRet;
}


void LayerMainMenu::onPlay( CCObject *pSender )
{
	SceneManager::replaceGamingScene();
}

void LayerMainMenu::onQuit( CCObject *pSender )
{
	CCDirector::sharedDirector()->end();
}

void LayerMainMenu::keyBackClicked( CCObject *pSender )
{
	CCDirector::sharedDirector()->end();
}


};




