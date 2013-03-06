

#include "GameObjects/Tiles/SpawnDiggerTile.h"
#include "GameObjects/DiggingWorld.h"
#include "GameObjects/DiggingPath.h"
#include "Layers/LayerGaming.h"

#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"


namespace AlphaDig
{

SpawnDiggerTile::SpawnDiggerTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight )
: SoilTile( pHostPath, nColumn, nHeight )
, m_pSpriteSpawnDigger( NULL )
{
	m_strTileType = "SPAWN_DIGGER";
	m_nCurrentState = E_SDS_NORMAL;
}

SpawnDiggerTile::~SpawnDiggerTile()
{
}


void SpawnDiggerTile::create()
{
	SoilTile::create();

	m_pSpriteSpawnDigger = _createTileSprite( _getTileRectName() );
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pBatchNode = pLayerGaming->getTilesBatchNode();
	pBatchNode->reorderChild( m_pSpriteSpawnDigger, m_pSpriteSoil->getZOrder() + 1 );
}

void SpawnDiggerTile::destroy()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pBatchNode = pLayerGaming->getTilesBatchNode();

	pBatchNode->removeChild( m_pSpriteSpawnDigger, true );

	m_pSpriteSpawnDigger = NULL;

	SoilTile::destroy();

}

bool SpawnDiggerTile::isTouched( CCTouch *pTouch )
{
	return _isTouchedSprite( pTouch );
}

void SpawnDiggerTile::touched()
{
	input( E_SDE_DEGENERATE );
}

const char* SpawnDiggerTile::_getTileRectName() const
{
	return "RECT_TILE_SPAWN_TILE";
}

void SpawnDiggerTile::_extrudeImage( float fHeightExtruded )
{
	SoilTile::_extrudeImage( fHeightExtruded );

	if( m_nCurrentState == E_SDS_NORMAL )
		_extrudeImageImp( fHeightExtruded, _getTileRectName(), m_pSpriteSpawnDigger );
}

void SpawnDiggerTile::_processInputEvent( int nEventID )
{
	if( nEventID == E_SDE_DEGENERATE )
		_changeState( E_SDS_SOIL );
}

void SpawnDiggerTile::_leaveCurrState()
{
	_destroySprite( m_pSpriteSpawnDigger );
	m_pSpriteSpawnDigger = NULL;
}

void SpawnDiggerTile::_enterNewState()
{
	DiggingWorld::sharedDiggingWorld()->resetOneClosedPath( m_nColumn );
}

}
