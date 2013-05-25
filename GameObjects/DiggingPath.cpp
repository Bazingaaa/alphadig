
#include <algorithm>

#include "GameObjects/DiggingPath.h"
#include "GameObjects/GameDigger.h"
#include "GameObjects/Tiles/SoilTile.h"
#include "GameObjects/Tiles/BlockTile.h"
#include "GameObjects/Tiles/GoldTile.h"
#include "GameObjects/Tiles/SpawnDiggerTile.h"
#include "GameObjects/DiggingWorld.h"
#include "Script/LuaHelper.h"

#include "platform/CCPlatformMacros.h"
#include "cocoa/CCSet.h"
#include "touch_dispatcher/CCTouch.h"
#include "platform/CCCommon.h"
#include "include/ccMacros.h"

extern "C"
{
#include "lua.h"
}

namespace AlphaDig
{


DiggingPath::DiggingPath( unsigned int nPathColumn)
: m_pGameDigger( NULL )
, m_bCreated( false )
, m_nPathColumn( nPathColumn )
, m_nCurrMaxHeightLvl( 0 )
, m_bPathClosed( false )
, m_bDiggerTouched( false )
{

}

DiggingPath::~DiggingPath()
{
	CC_SAFE_RELEASE_NULL( m_pGameDigger );
	for( TileList::iterator iter = m_soilTiles.begin(); iter != m_soilTiles.end(); ++iter )
	{
		CC_SAFE_RELEASE_NULL( *iter );
	}

	m_soilTiles.clear();
}

void DiggingPath::create()
{
	//创建挖掘机
	m_pGameDigger = new GameDigger( this );
	m_pGameDigger->create();
	if( m_nPathColumn == 2 )
		m_pGameDigger->setMomentum( 100.0f );

	//创建土方
	unsigned int nTileCountPerColumn = DiggingWorld::sharedDiggingWorld()->getTileCountPerColumn();
	for( unsigned int i = 0; i < nTileCountPerColumn; ++i )
	{
		SoilTile *pTile = _createTile( false, i );
		pTile->create();
		m_soilTiles.push_back( pTile );
	}


	m_nCurrMaxHeightLvl = nTileCountPerColumn;

	m_bCreated = true;
}


void DiggingPath::update( float fElapsedTime )
{
	if( !m_bCreated )
		return;

	//根据可视最大高度，建立新的土方
	unsigned int nCachedHeightLvl = DiggingWorld::sharedDiggingWorld()->getCurrentBottomHeight() + 3; //缓冲一定数量的土方
	if( nCachedHeightLvl > m_nCurrMaxHeightLvl )
	{
		for( unsigned int i = m_nCurrMaxHeightLvl; i < nCachedHeightLvl; ++i )
		{
			SoilTile *pTile =_createTile( true, i );
			pTile->create();
			m_soilTiles.push_back( pTile );
		}

		m_nCurrMaxHeightLvl = nCachedHeightLvl;
	}

	m_pGameDigger->update( fElapsedTime );

	//根据挖掘机的高度，销毁土方
	CCAssert( !m_soilTiles.empty(), "Tiles can't be empty !" );
	SoilTile *pTopTile = m_soilTiles.front();
	float fHeightDiff = m_pGameDigger->getHeight() - pTopTile->getTopHeight();
	if( fHeightDiff > 0.0f )
		pTopTile->extrude( fHeightDiff );

	//see if the digger is out of range
	if( m_pGameDigger->isBrokenup() )
	{
		m_bPathClosed = true;
	}

	//remove the tiles
	for( TileList::iterator iter = m_tilesToBeRemoved.begin(); iter != m_tilesToBeRemoved.end(); ++iter )
		_removeTile( *iter );
	m_tilesToBeRemoved.clear();
}

void DiggingPath::touchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCSetIterator iter = pTouches->begin();
	for (; iter != pTouches->end(); iter++)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);

		//touches judgement for tiles
		for( TileList::iterator iter = m_soilTiles.begin(); iter != m_soilTiles.end(); ++iter )
		{
			if( (*iter)->isTouched( pTouch ) )
			{
				(*iter)->touched();
				break;
			}
		}

		m_bDiggerTouched = m_pGameDigger->isTouched( pTouch );
		if( m_bDiggerTouched )
			m_pGameDigger->startDragging();
	}
}

void DiggingPath::touchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	//判断是否在本挖掘机区域内
	CCSetIterator iter = pTouches->begin();
	for (; iter != pTouches->end(); iter++)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		if( m_bDiggerTouched )
		{
			CCPoint delta = pTouch->getDelta();
			m_pGameDigger->dig( -delta.y * 5.0f );
			break;
		}

	}
}

float DiggingPath::getGroundHeight( ) const
{
	CCAssert( !m_soilTiles.empty(), "Tiles can't be empty !" );
	SoilTile *pTile = m_soilTiles.front();

	return pTile->getTopHeight();
}

float DiggingPath::getTopBlockHeight() const
{
	TileList::const_iterator iter = m_soilTiles.begin();
	for( ; iter != m_soilTiles.end(); ++iter )
	{
		SoilTile *pTile = *iter;
		if( pTile->getTileType() == "BLOCK" && pTile->getCurrState() != BlockTile::E_BS_SOIL )
			return pTile->getTopHeight();
	}

	return m_soilTiles.back()->getTopHeight();
}

void DiggingPath::notifyRemoveTile( SoilTile *pTile )
{
	if( std::find( m_tilesToBeRemoved.begin(), m_tilesToBeRemoved.end(), pTile ) != m_tilesToBeRemoved.end() )
		return;

	m_tilesToBeRemoved.push_back( pTile );
}

void DiggingPath::respawnDigger()
{
	if( !m_pGameDigger->isBrokenup() )
		return;
	m_pGameDigger->setMomentum( 200.0f );
}

void DiggingPath::_removeTile( SoilTile *pTile )
{
	pTile->destroy();
	m_soilTiles.remove( pTile );

	CC_SAFE_RELEASE_NULL( pTile );
}

SoilTile* DiggingPath::_createTile( bool bCreateInDigging, unsigned int nRow )
{
	SoilTile *pRetTile = NULL;
	if( bCreateInDigging )
	{
		lua_State *pLuaState = LuaHelper::s_getLuaState();
		lua_getglobal( pLuaState, "GetNewTileTypeName" );
		LuaHelper::s_protectedCallFunc( 0, 1 );
		std::string strNewTileTypeName = lua_tostring( pLuaState, -1 );
		lua_pop( pLuaState, 1 );

		if( strNewTileTypeName == "BLOCK_LVL_1" )
			pRetTile = new BlockTile( this, m_nPathColumn, nRow, BlockTile::E_BS_BLOCK_LVL_1 );
		else if( strNewTileTypeName == "GOLD" )
			pRetTile = new GoldTile( this, m_nPathColumn, nRow );
		else if( strNewTileTypeName == "SPAWN_DIGGER" )
			pRetTile = new SpawnDiggerTile( this, m_nPathColumn, nRow );
		else
			pRetTile = new SoilTile( this, m_nPathColumn, nRow );
	}
	else
		pRetTile = new SoilTile( this, m_nPathColumn, nRow );

	return pRetTile;
}

}

