
#include "GameObjects/DiggingWorld.h"
#include "GameObjects/DiggingPath.h"
#include "Layers/LayerGaming.h"
#include "SceneManager.h"
#include "Script/LuaHelper.h"

#include "platform/CCPlatformMacros.h"
#include "cocoa/CCSet.h"
#include "CCDirector.h"
#include "touch_dispatcher/CCTouch.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "platform/CCCommon.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"

#include <vector>


namespace AlphaDig
{

DiggingWorld* DiggingWorld::s_pSharedDiggingWorld = NULL;

DiggingWorld::DiggingWorld()
: m_nMaxPathCount( 5 )
, m_fTileSize( 0.0f )
, m_nTileCountPerColumn( 0 )
, m_fTileCountPerColumn( 0.0f )
, m_fCurrentBottomHeight( 0.0f )
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_fTileSize = winSize.width / m_nMaxPathCount;
	m_fTileCountPerColumn = winSize.height / m_fTileSize;
	m_nTileCountPerColumn = m_fTileCountPerColumn;

	s_pSharedDiggingWorld = this;
}

DiggingWorld::~DiggingWorld()
{

}

void DiggingWorld::startNewWorld()
{
	clearWorld();

	_createOneDiggingPath();
}

void DiggingWorld::update( float fElapsedTime )
{
	// update the button height
	CCSpriteBatchNode *pMainBatchNode = LayerGaming::sharedLayerGaming()->getTilesBatchNode();
	float fBatchNodePosY = pMainBatchNode->getPositionY();
	m_fCurrentBottomHeight = fBatchNodePosY / m_fTileSize + m_fTileCountPerColumn;

	bool bGameOver = true;
	for( DiggingPathVector::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
	{
		(*iter)->update( fElapsedTime );

		if( !(*iter)->isPathClosed() )
			bGameOver = false;
	}

	if( bGameOver )
		SceneManager::replaceGameOverScene();
}

void DiggingWorld::_createOneDiggingPath()
{
	if( m_diggingPaths.size() >= m_nMaxPathCount )
		return;

	for( unsigned int i = 0; i < 5; ++i )
	{
		DiggingPath *pNewPath = new DiggingPath(i);
		pNewPath->create( );
		m_diggingPaths.push_back( pNewPath );
	}

}

void DiggingWorld::clearWorld()
{
	for( DiggingPathVector::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
	{
		CC_SAFE_RELEASE_NULL( *iter );
	}

	m_diggingPaths.clear();
}

void DiggingWorld::touchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	for( DiggingPathVector::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
	{
		(*iter)->touchesBegan( pTouches, pEvent );
	}
}

void DiggingWorld::touchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	for( DiggingPathVector::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
	{
		(*iter)->touchesMoved( pTouches, pEvent );
	}
}

void DiggingWorld::convertToGLCoordinate( float fRow, float fColumn, float &fOutX, float &fOutY )
{
	fOutX = m_fTileSize * fColumn + m_fTileSize * 0.5f;

	float fConvertedRow = -fRow + m_fTileCountPerColumn;

	fOutY = m_fTileSize * fConvertedRow + m_fTileSize * 0.5f;
}

void DiggingWorld::resetOneClosedPath( unsigned int nColumn )
{
	if( m_diggingPaths.size() <= nColumn )
		return;

	DiggingPath *pDiggingPath = m_diggingPaths[ nColumn ];
	if( !pDiggingPath->isPathClosed() )
		return;

	pDiggingPath->respawnDigger();
}

CCSprite* DiggingWorld::s_createTileSprite( const char *pRectName, unsigned int nColumn, unsigned int nHeight, float fScaleX, float fScaleY, int nZorder )
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pBatchNode = pLayerGaming->getTilesBatchNode();
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();

	CCSprite *pRetSprite = NULL;
	CCRect RECT_TILE = LuaHelper::s_getRectVar( pRectName );
	pRetSprite = CCSprite::createWithTexture( pBatchNode->getTexture(), RECT_TILE );
	pBatchNode->addChild( pRetSprite, nZorder );

	//¼ÆËãspriteµÄÎ»ÖÃ
	float fSoilSpriteX = 0.0f;
	float fSoilSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( nHeight, nColumn, fSoilSpriteX, fSoilSpriteY );

	float fTileSize = pDiggingWorld->getTileSize();
	pRetSprite->setScaleX( fTileSize / RECT_TILE.size.width * fScaleX );
	pRetSprite->setScaleY( fTileSize / RECT_TILE.size.height * fScaleY );

	pRetSprite->setPosition( ccp( fSoilSpriteX, fSoilSpriteY ) );

	return pRetSprite;
}


void DiggingWorld::s_destroyTileSprite( CCSprite *pSprite )
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pBatchNode = pLayerGaming->getTilesBatchNode();

	pBatchNode->removeChild( pSprite, true );
}


}

