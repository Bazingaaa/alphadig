
#include "SceneManager.h"
#include "Layers/LayerLogo.h"
#include "Layers/LayerMainMenu.h"
#include "Layers/LayerGaming.h"
#include "Layers/LayerGameOver.h"
#include "Layers/LayerOption.h"


#include "layers_scenes_transitions_nodes/CCScene.h"
#include "CCDirector.h"
#include "layers_scenes_transitions_nodes/CCTransition.h"
#include "include/ccTypes.h"

namespace AlphaDig
{
using namespace cocos2d;

void SceneManager::startLogoScene()
{
	CCScene *pScene = CCScene::create();

	LayerLogo *pLayer = new LayerLogo();
	pLayer->init();
	pLayer->initLayer();
	pLayer->autorelease();

	pScene->addChild( pLayer );

	CCDirector::sharedDirector()->runWithScene(pScene);
}

void SceneManager::replaceGamingScene()
{
	CCScene *pScene = CCScene::create();

	LayerGaming *pLayer = new LayerGaming();
	pLayer->init();
	pLayer->initLayer();
	pLayer->autorelease();

	pScene->addChild( pLayer );

	CCDirector::sharedDirector()->replaceScene(pScene);
}


void SceneManager::replaceMainMenuScene()
{
	CCScene *pScene = CCScene::create();

	LayerMainMenu *pLayer = new LayerMainMenu();
	pLayer->init();
	pLayer->initLayer();
	pLayer->autorelease();

	pScene->addChild( pLayer );

	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create( 1.0f, pScene, ccBLACK ) );
}

void SceneManager::replaceGameOverScene()
{
	CCScene *pScene = CCScene::create();

	LayerGameOver *pLayer = new LayerGameOver();
	pLayer->init();
	pLayer->initLayer();
	pLayer->autorelease();

	pScene->addChild( pLayer );

	CCDirector::sharedDirector()->replaceScene(pScene);
}

}

