
#include "GameObjects/Tiles/GoldTile.h"
#include "GameObjects/DiggingPath.h"
#include "Layers/LayerGaming.h"
#include "StatisticsManager.h"
#include "Script/LuaHelper.h"

#include "support/CCPointExtension.h"


namespace AlphaDig
{

GoldTile::GoldTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned nHeight )
: SoilTile( pHostPath, nColumn, nHeight )
, m_nGoldCount( 1 )
{

}

GoldTile::~GoldTile()
{

}

void GoldTile::extrude( float fHeightExtruded )
{
	touched();
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

}

