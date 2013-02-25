
#ifndef BLOCK_TILE_H__
#define BLOCK_TILE_H__


#include "GameObjects/Tiles/SoilTile.h"

namespace AlphaDig
{

class BlockTile : public SoilTile
{
public:
	BlockTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight );
	~BlockTile();

	///<override
	virtual void extrude( float fHeightExtruded );
	virtual bool isTouched( CCTouch *pTouch );
	virtual void touched();

protected:
	///<override
	virtual const char* _getTileRectName() const;

protected:

	///< the damage to digger durability
	float m_fDamage;
};

}




#endif

