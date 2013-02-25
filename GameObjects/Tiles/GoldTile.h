
#ifndef GOLD_TILE_H__
#define GOLD_TILE_H__

#include "GameObjects/Tiles/SoilTile.h"

namespace AlphaDig
{

class DiggingPath;

class GoldTile : public SoilTile
{
public:
	GoldTile(  DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight );
	~GoldTile();

	///<override
	virtual void extrude( float fHeightExtruded );
	virtual bool isTouched( CCTouch *pTouch );
	virtual void touched();

protected:
	///<override
	virtual const char* _getTileRectName() const;

protected:

	///<the gold count of this tile
	unsigned int m_nGoldCount;
};


}


#endif

