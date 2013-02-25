
#include "GameObjects/Tiles/HealTile.h"
#include "GameObjects/Digger.h"
#include "GameObjects/DiggingPath.h"



namespace AlphaDig
{

HealTile::HealTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight )
: SoilTile( pHostPath, nColumn, nHeight )
, m_nHealthHealed( 50 )
{
}

HealTile::~HealTile()
{

}

void HealTile::extrude( float fHeightExtruded )
{
	if( fHeightExtruded >= 0.5f )
	{
		Digger *pDigger = m_pHostPath->getDigger();
		pDigger->modifyDurability( m_nHealthHealed );
		m_pHostPath->notifyRemoveTile( this );
	}
	else
		_extrudeImage( fHeightExtruded );
}

bool HealTile::isTouched( CCTouch *pTouch )
{
	return false;
}

void HealTile::touched()
{

}

const char* HealTile::_getTileRectName() const
{
	return "RECT_HEAL_TILE";
}




}

