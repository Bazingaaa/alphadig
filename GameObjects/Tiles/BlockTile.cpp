


#include "GameObjects/Tiles/BlockTile.h"
#include "GameObjects/Digger.h"
#include "GameObjects/DiggingPath.h"
#include "Script/LuaHelper.h"
#include "Layers/LayerGaming.h"

#include "touch_dispatcher/CCTouch.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"

namespace AlphaDig
{

BlockTile::BlockTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned nHeight )
: SoilTile( pHostPath, nColumn, nHeight )
//, FiniteStateMachine( )
, m_pSpriteBlock( NULL )
{
	m_strTileType = "BLOCK";
}

BlockTile::~BlockTile()
{

}

void BlockTile::create( )
{
	SoilTile::create();

	m_pSpriteBlock = _createTileSprite( _getTileRectName() );
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();
	pMainBatchNode->reorderChild( m_pSpriteBlock, m_pSpriteSoil->getZOrder() + 1 );
}

void BlockTile::destroy()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();

	pMainBatchNode->removeChild( m_pSpriteBlock, true );

	m_pSpriteBlock = NULL;

	SoilTile::destroy();
}

bool BlockTile::isTouched( CCTouch *pTouch )
{
	return _isTouchedSprite( pTouch );
}

void BlockTile::touched()
{
	m_pHostPath->notifyRemoveTile( this );
}

const char* BlockTile::_getTileRectName() const
{
	return "RECT_BLOCK_TILE";
}

void BlockTile::_extrudeImage( float fHeightExtruded )
{
	SoilTile::_extrudeImage( fHeightExtruded );

	_extrudeImageImp( fHeightExtruded, _getTileRectName(), m_pSpriteBlock );
}

/*void BlockTile::_processInputEvent( int nEventID )
{
	if( nEventID == E_BE_DEGENERATE )
	{
		m_nState -= 1;
	}
}*/


}

