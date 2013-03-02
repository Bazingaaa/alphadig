
#include "GameObjects/Tiles/GoldTile.h"
#include "GameObjects/DiggingPath.h"
#include "Layers/LayerGaming.h"
#include "StatisticsManager.h"
#include "Script/LuaHelper.h"

#include "support/CCPointExtension.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"


namespace AlphaDig
{

GoldTile::GoldTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned nHeight )
: SoilTile( pHostPath, nColumn, nHeight )
, m_nGoldCount( 1 )
, m_pSpriteGold( NULL )
{
	m_strTileType = "GOLD";
}

GoldTile::~GoldTile()
{

}

void GoldTile::create()
{
	SoilTile::create();

	m_pSpriteGold = _createTileSprite( _getTileRectName() );
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();
	pMainBatchNode->reorderChild( m_pSpriteGold, m_pSpriteSoil->getZOrder() + 1 );
}

void GoldTile::destroy()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();

	pMainBatchNode->removeChild( m_pSpriteGold, true );

	m_pSpriteGold = NULL;

	SoilTile::destroy();
}

bool GoldTile::isTouched( CCTouch *pTouch )
{
	return _isTouchedSprite( pTouch );
}

void GoldTile::touched()
{
	Digger *pDigger = m_pHostPath->getDigger();
	m_pHostPath->notifyRemoveTile( this );
	StatisticsManager::getSingleton().addStatisticsByName( "gold_obtained", m_nGoldCount );
	LayerGaming::sharedLayerGaming()->updateGoldHUD();
}

const char* GoldTile::_getTileRectName() const
{
	return "RECT_GOLD_TILE";
}

void GoldTile::_extrudeImage( float fHeightExtruded )
{
	SoilTile::_extrudeImage( fHeightExtruded );

	_extrudeImageImp( fHeightExtruded, _getTileRectName(), m_pSpriteGold );
}



}

