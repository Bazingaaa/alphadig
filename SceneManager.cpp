
#include "SceneManager.h"
#include "Layers/LayerMainMenu.h"
#include "Layers/LayerGaming.h"
#include "Layers/LayerGameOver.h"


#include "layers_scenes_transitions_nodes/CCScene.h"
#include "CCDirector.h"

namespace AlphaDig
{
using namespace cocos2d;


void SceneManager::startMainMenuScene()
{
	CCScene *pScene = CCScene::create();

	LayerMainMenu *pLayer = new LayerMainMenu();
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

	CCDirector::sharedDirector()->replaceScene(pScene);
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

