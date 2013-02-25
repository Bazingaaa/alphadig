
#include "GameObjects/DiggingWorld.h"
#include "GameObjects/DiggingPath.h"
#include "Layers/LayerGaming.h"
#include "SceneManager.h"

#include "platform/CCPlatformMacros.h"
#include "cocoa/CCSet.h"
#include "CCDirector.h"
#include "touch_dispatcher/CCTouch.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "platform/CCCommon.h"

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
	CCSpriteBatchNode *pMainBatchNode = LayerGaming::sharedLayerGaming()->getMainSpriteBatchNode();
	float fBatchNodePosY = pMainBatchNode->getPositionY();
	m_fCurrentBottomHeight = fBatchNodePosY / m_fTileSize + m_fTileCountPerColumn;

	bool bGameOver = true;
	for( DiggingPathList::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
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
	for( DiggingPathList::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
	{
		CC_SAFE_RELEASE_NULL( *iter );
	}

	m_diggingPaths.clear();
}

void DiggingWorld::touchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	for( DiggingPathList::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
	{
		(*iter)->touchesBegan( pTouches, pEvent );
	}
}

void DiggingWorld::touchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	for( DiggingPathList::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
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

void DiggingWorld::resetOneClosedPath()
{
	std::vector< DiggingPath* > closedPaths;
	for( DiggingPathList::iterator iter = m_diggingPaths.begin(); iter != m_diggingPaths.end(); ++iter )
	{
		if( (*iter)->isPathClosed() )
			closedPaths.push_back( *iter );
	}

	if( closedPaths.empty() )
		return;

	std::size_t closedPathsNum = closedPaths.size();
	DiggingPath *pDiggingPath = closedPaths[ rand() % closedPathsNum ];

	pDiggingPath->respawnDigger();
}

}

