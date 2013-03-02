#include "Layers/LayerOption.h"
#include "Script/LuaHelper.h"
#include "SceneManager.h"

#include "menu_nodes/CCMenu.h"
#include "menu_nodes/CCMenuItem.h"
#include "platform/CCPlatformMacros.h"
#include "CCDirector.h"
#include "support/CCPointExtension.h"

namespace AlphaDig
{

LayerOption::LayerOption()
{
}

LayerOption::~LayerOption()
{
}

bool LayerOption::initLayer()
{
	bool bRet = false;

	do
	{
		CCMenuItemFont::setFontSize(18);
		CCMenuItemFont *pSoundTitle = CCMenuItemFont::create(
				LuaHelper::s_getStringVar("TEXT_SOUND").c_str());
		CC_BREAK_IF(! pSoundTitle);
		pSoundTitle->setEnabled(false);

		CCMenuItemFont::setFontSize(34);
		CCMenuItemToggle *pSoundToggle = CCMenuItemToggle::createWithTarget(
				this, menu_selector(LayerOption::menuCallback),
				CCMenuItemFont::create(
						LuaHelper::s_getStringVar("TEXT_ON").c_str()),
				CCMenuItemFont::create(
						LuaHelper::s_getStringVar("TEXT_OFF").c_str()), NULL);
		CC_BREAK_IF(! pSoundToggle);

		CCMenuItemFont::setFontSize(18);
		CCMenuItemFont *pMusicTitle = CCMenuItemFont::create(
				LuaHelper::s_getStringVar("TEXT_MUSIC").c_str());
		CC_BREAK_IF(! pMusicTitle);
		pMusicTitle->setEnabled(false);

		CCMenuItemFont::setFontSize(34);
		CCMenuItemToggle *pMusicToggle = CCMenuItemToggle::createWithTarget(
				this, menu_selector(LayerOption::menuCallback),
				CCMenuItemFont::create(
						LuaHelper::s_getStringVar("TEXT_ON").c_str()),
				CCMenuItemFont::create(
						LuaHelper::s_getStringVar("TEXT_OFF").c_str()), NULL);
		CC_BREAK_IF(! pMusicToggle);

		CCMenuItemFont::setFontSize(36);
		CCMenuItemFont *pBackButton = CCMenuItemFont::create("back", this,
				menu_selector(LayerOption::keyBackClicked));
		CC_BREAK_IF(! pBackButton);

		CCMenu *menu = CCMenu::create(pSoundTitle, pMusicTitle, pSoundToggle,
				pMusicToggle, pBackButton, NULL);
		CC_BREAK_IF(! menu);

		menu->alignItemsInColumns(2, 2, 1, NULL);

		addChild(menu);

		CCSize s = CCDirector::sharedDirector()->getWinSize();
		menu->setPosition(ccp(s.width / 2, s.height / 2));

	} while (false);
}

void LayerOption::menuCallback()
{

}

void LayerOption::keyBackClicked()
{
	SceneManager::replaceMainMenuScene();
}

}
