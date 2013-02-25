
#ifndef HEAL_TILE_H__
#define HEAL_TILE_H__

#include "GameObjects/Tiles/SoilTile.h"

namespace AlphaDig
{

class HealTile : public SoilTile
{
public:
	HealTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight );
	~HealTile();

	///<override
	virtual void extrude( float fHeightExtruded );
	virtual bool isTouched( CCTouch *pTouch );
	virtual void touched();

protected:
	///<override
	virtual const char* _getTileRectName() const;

protected:

	///<health healed
	unsigned int m_nHealthHealed;
};


}



#endif

