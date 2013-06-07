


#include "GameObjects/Tiles/BlockTile.h"
#include "GameObjects/DiggingPath.h"
#include "Script/LuaHelper.h"
#include "Layers/LayerGaming.h"
#include "CocosExt/ADParticleSystemCache.h"
#include "GameObjects/DiggingWorld.h"

#include "touch_dispatcher/CCTouch.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"

namespace AlphaDig
{

BlockTile::BlockTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned nHeight, BlockState eBlockState )
: SoilTile( pHostPath, nColumn, nHeight )
, m_pSpriteBlock( NULL )
{
	m_strTileType = "BLOCK";

	m_nCurrentState = eBlockState;
}

BlockTile::~BlockTile()
{

}

void BlockTile::create( )
{
	SoilTile::create();

	_createBlockSprite();
}

void BlockTile::destroy()
{

	DiggingWorld::s_destroyTileSprite( m_pSpriteBlock );
	m_pSpriteBlock = NULL;

	SoilTile::destroy();
}

bool BlockTile::isTouched( CCTouch *pTouch )
{
	return _isTouchedSprite( pTouch );
}

void BlockTile::touched()
{
	input( E_BE_DEGENERATE );
}

void BlockTile::_createBlockSprite()
{
	m_pSpriteBlock = _createTileSprite( _getTileRectName() );
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pBatchNode = pLayerGaming->getTilesBatchNode();
	pBatchNode->reorderChild( m_pSpriteBlock, m_pSpriteSoil->getZOrder() + 1 );

	/*ccBlendFunc blendFunc = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
	m_pSpriteBlock->setBlendFunc( blendFunc );*/
}

const char* BlockTile::_getTileRectName() const
{
	if( m_nCurrentState ==  E_BS_BLOCK_LVL_1 )
		return "RECT_TILE_BLOCK_LVL_1";
	else if( m_nCurrentState == E_BS_BLOCK_LVL_2 )
		return "RECT_TILE_BLOCK_LVL_2";
	else
		return "RECT_TILE_BLOCK_LVL_3";
}

void BlockTile::_extrudeImage( float fHeightExtruded )
{
	SoilTile::_extrudeImage( fHeightExtruded );

	if( m_nCurrentState != E_BS_SOIL )
		_extrudeImageImp( fHeightExtruded, _getTileRectName(), m_pSpriteBlock );
}


void BlockTile::_processInputEvent( int nEventID )
{
	if( nEventID == E_BE_DEGENERATE )
	{
		if( m_nCurrentState > E_BS_SOIL )
			_changeState( m_nCurrentState - 1 );
	}
}

void BlockTile::_leaveCurrState()
{
	const CCPoint point = m_pSpriteBlock->getPosition();
	ADParticleSystemCache::getSingleton().playParticleSystem(
			LuaHelper::s_getStringVar("PARTS_BLOCK_EXP").c_str(), m_pSpriteBlock->getParent()->convertToWorldSpace( point ) );

	DiggingWorld::s_destroyTileSprite( m_pSpriteBlock );
	m_pSpriteBlock = NULL;


}

void BlockTile::_enterNewState()
{
	if( m_nCurrentState != E_BS_SOIL )
		_createBlockSprite();
}


}

