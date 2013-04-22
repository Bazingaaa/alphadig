
#include "Layers/LayerGameOver.h"
#include "SceneManager.h"
#include "Script/LuaHelper.h"

#include "menu_nodes/CCMenu.h"
#include "menu_nodes/CCMenuItem.h"
#include "label_nodes/CCLabelTTF.h"
#include "CCDirector.h"
#include "support/CCPointExtension.h"
#include "platform/CCPlatformMacros.h"


namespace AlphaDig
{

using namespace cocos2d;

LayerGameOver::LayerGameOver()
{

}

LayerGameOver::~LayerGameOver()
{

}

bool LayerGameOver::initLayer()
{
	bool bRet = false;

	do
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		CCLabelTTF *pGameOverLabel = CCLabelTTF::create(LuaHelper::s_getStringVar("TEXT_GAME_OVER").c_str(), "Arial", 50);
		pGameOverLabel->setPosition( ccp( winSize.width*0.5f, winSize.height*0.5f) );
		addChild( pGameOverLabel );

		CCMenuItemFont *pPlayAgainItem = CCMenuItemFont::create( LuaHelper::s_getStringVar("TEXT_PLAY_AGAIN").c_str(), this, menu_selector(LayerGameOver::onPlayAgain));
		CC_BREAK_IF(! pPlayAgainItem);

		CCMenuItemFont *pToMainMenu = CCMenuItemFont::create( LuaHelper::s_getStringVar("TEXT_TO_MAIN_MENU").c_str(), this, menu_selector(LayerGameOver::onToMainMenu));
		CC_BREAK_IF(! pToMainMenu);

		CCMenu* pMenu = CCMenu::create( pPlayAgainItem, pToMainMenu, NULL );
		CC_BREAK_IF(! pMenu);
		pMenu->alignItemsHorizontallyWithPadding( 300.0f );

		addChild( pMenu );

		pMenu->setPosition(ccp(winSize.width*0.5f, winSize.height*0.25f));

		setKeypadEnabled(true);

		bRet = true;
	}
	while( false );

	return bRet;
}

void LayerGameOver::deleteLayer()
{
	// auto release obj
}

void LayerGameOver::onPlayAgain( CCObject *pSender )
{
	SceneManager::replaceGamingScene();
}

void LayerGameOver::onToMainMenu( CCObject *pSender )
{
	SceneManager::replaceMainMenuScene();
}

void LayerGameOver::keyBackClicked( CCObject *pSender )
{
	onToMainMenu( pSender );
}

}

