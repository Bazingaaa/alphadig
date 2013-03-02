

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
}

SpawnDiggerTile::~SpawnDiggerTile()
{
}


void SpawnDiggerTile::create()
{
	SoilTile::create();

	m_pSpriteSpawnDigger = _createTileSprite( _getTileRectName() );
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();
	pMainBatchNode->reorderChild( m_pSpriteSpawnDigger, m_pSpriteSoil->getZOrder() + 1 );
}

void SpawnDiggerTile::destroy()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();

	pMainBatchNode->removeChild( m_pSpriteSpawnDigger, true );

	m_pSpriteSpawnDigger = NULL;

	SoilTile::destroy();

}

bool SpawnDiggerTile::isTouched( CCTouch *pTouch )
{
	return _isTouchedSprite( pTouch );
}

void SpawnDiggerTile::touched()
{
	DiggingWorld::sharedDiggingWorld()->resetOneClosedPath();
	m_pHostPath->notifyRemoveTile( this );
}

const char* SpawnDiggerTile::_getTileRectName() const
{
	return "RECT_SPAWN_DIGGER_TILE";
}

void SpawnDiggerTile::_extrudeImage( float fHeightExtruded )
{
	SoilTile::_extrudeImage( fHeightExtruded );

	_extrudeImageImp( fHeightExtruded, _getTileRectName(), m_pSpriteSpawnDigger );
}

}
