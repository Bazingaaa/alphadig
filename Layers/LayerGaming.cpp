

#include "SceneManager.h"
#include "Layers/LayerGaming.h"
#include "GameObjects/DiggingWorld.h"
#include "Script/LuaHelper.h"
#include "StatisticsManager.h"

#include "label_nodes/CCLabelTTF.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"
#include "CCDirector.h"
#include "support/CCPointExtension.h"
#include "platform/CCPlatformMacros.h"
#include "actions/CCActionInterval.h"
#include "touch_dispatcher/CCTouchDispatcher.h"

namespace AlphaDig
{

LayerGaming* LayerGaming::s_pSharedLayerGaming = NULL;

LayerGaming::LayerGaming()
: m_pDiggingWorld( NULL )
, m_pMainBatchNode( NULL )
, m_pMainBatchNodeForHUD( NULL )
, m_pCurrentLvlLabel( NULL )
, m_nCurrentDisplayedLvlValue( 0 )
, m_pGoldObtainedLabel( NULL )
{
	s_pSharedLayerGaming = this;
}

LayerGaming::~LayerGaming()
{
	deleteLayer();
}



bool LayerGaming::initLayer()
{
	bool bRet = false;

	do
	{
		_createSpriteBatchNode();

		m_pDiggingWorld = new DiggingWorld( );

		m_pDiggingWorld->startNewWorld();

		_createHUD();

		schedule( schedule_selector( LayerGaming::_updateGame ) );

		setKeypadEnabled(true);
		setTouchEnabled(true);
		bRet = true;
	}
	while( false );

	return bRet;
}

void LayerGaming::deleteLayer()
{
	_destroyHUD();

	m_pDiggingWorld->clearWorld();

	CC_SAFE_RELEASE_NULL( m_pDiggingWorld );
}

void LayerGaming::keyBackClicked()
{
	SceneManager::replaceMainMenuScene();
}

void LayerGaming::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	m_pDiggingWorld->touchesBegan( pTouches, pEvent );
}
void LayerGaming::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	m_pDiggingWorld->touchesMoved( pTouches, pEvent );
}

void LayerGaming::registerWithTouchDispatcher( )
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}

void LayerGaming::_createSpriteBatchNode()
{
	m_pMainBatchNode = CCSpriteBatchNode::create( LuaHelper::s_getStringVar("IMAGE_MAIN").c_str(), 64);
	addChild( m_pMainBatchNode );
	CCActionInterval *pGoUp = CCMoveBy::create( 1.0f, ccp( 0, 50 ) );
	m_pMainBatchNode->runAction( (CCRepeatForever::create( pGoUp ) ) );

	m_pMainBatchNodeForHUD = CCSpriteBatchNode::create( LuaHelper::s_getStringVar("IMAGE_MAIN").c_str(), 4);
	addChild( m_pMainBatchNodeForHUD );
}

void LayerGaming::_updateGame( float fElapsedTime )
{
	_updateHUD();

	m_pDiggingWorld->update( fElapsedTime );
}


void LayerGaming::_createHUD()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//create gold hud
	CCRect goldRect = LuaHelper::s_getRectVar( "RECT_GOLD_HUD" );
	m_pGoldImg = CCSprite::createWithTexture( m_pMainBatchNodeForHUD->getTexture(), goldRect );
	m_pGoldImg->setAnchorPoint( ccp( 0.0f, 1.0f) );
	m_pGoldImg->setPosition( ccp( 0.0f, winSize.height ) );
	m_pMainBatchNodeForHUD->addChild( m_pGoldImg );

	m_pGoldObtainedLabel = CCLabelTTF::create("0", "Arial", 30 );
	m_pGoldObtainedLabel->setAnchorPoint( ccp( 0.0f, 1.0f) );
	m_pGoldObtainedLabel->setPosition( ccp( 32.0f, winSize.height ) );
	addChild( m_pGoldObtainedLabel );

	//create lvl label hud
	CCRect lvlRect = LuaHelper::s_getRectVar( "RECT_LVL_HUD" );
	m_pLvlImg = CCSprite::createWithTexture( m_pMainBatchNodeForHUD->getTexture(), lvlRect );
	m_pLvlImg->setAnchorPoint( ccp( 1.0f, 1.0f) );
	m_pLvlImg->setPosition( ccp( winSize.width, winSize.height ) );
	m_pMainBatchNodeForHUD->addChild( m_pLvlImg );

	m_pCurrentLvlLabel = CCLabelTTF::create("0", "Arial", 30 );
	m_pCurrentLvlLabel->setAnchorPoint( ccp( 1.0f, 1.0f) );
	m_pCurrentLvlLabel->setPosition( ccp( winSize.width - 32.0f, winSize.height ) );
	addChild( m_pCurrentLvlLabel );
}

void LayerGaming::_destroyHUD()
{
	//auto release
}


void LayerGaming::updateGoldHUD()
{
	int nGoldObtained = StatisticsManager::getSingleton().getStatisticsByName( "gold_obtained" );
	CCString *pCCStrGoldObtained = CCString::createWithFormat( "%u", nGoldObtained );
	m_pGoldObtainedLabel->setString( pCCStrGoldObtained->getCString() );
}

void LayerGaming::_updateHUD()
{
	unsigned int nCurrentLvl = m_pDiggingWorld->getCurrentBottomHeight();
	if( m_nCurrentDisplayedLvlValue != nCurrentLvl )
	{
		CCString *pCCStrCurrentLvl = CCString::createWithFormat( "%u", nCurrentLvl );
		m_pCurrentLvlLabel->setString( pCCStrCurrentLvl->getCString() );
	}
}




}
