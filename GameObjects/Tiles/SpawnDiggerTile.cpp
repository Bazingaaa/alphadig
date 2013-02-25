

#include "GameObjects/Tiles/SpawnDiggerTile.h"
#include "GameObjects/DiggingWorld.h"
#include "GameObjects/DiggingPath.h"


namespace AlphaDig
{

SpawnDiggerTile::SpawnDiggerTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight )
: SoilTile( pHostPath, nColumn, nHeight )
{

}

SpawnDiggerTile::~SpawnDiggerTile()
{
}

void SpawnDiggerTile::extrude( float fHeightExtruded )
{
	if( fHeightExtruded > 0.5f )
	{
		touched();
	}
	else
		_extrudeImage( fHeightExtruded );
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

}
