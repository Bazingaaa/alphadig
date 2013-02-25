


#include "GameObjects/Tiles/BlockTile.h"
#include "GameObjects/Digger.h"
#include "GameObjects/DiggingPath.h"
#include "Script/LuaHelper.h"

#include "touch_dispatcher/CCTouch.h"
#include "sprite_nodes/CCSprite.h"

namespace AlphaDig
{

BlockTile::BlockTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned nHeight )
: SoilTile( pHostPath, nColumn, nHeight )
, m_fDamage( 0.0f )
{
	m_fDamage = LuaHelper::s_getNumberVar( "DAMAGE_BLOCK_TILE" );
}

BlockTile::~BlockTile()
{

}

void BlockTile::extrude( float fHeightExtruded )
{
	if( fHeightExtruded > 0.2f )
	{
		Digger *pDigger = m_pHostPath->getDigger();
		pDigger->modifyDurability( -m_fDamage );
		m_pHostPath->notifyRemoveTile( this );
	}
	else
		_extrudeImage( fHeightExtruded );
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


}

