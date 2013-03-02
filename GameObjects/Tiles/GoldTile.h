
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
	virtual void create();
	virtual void destroy();
	virtual bool isTouched( CCTouch *pTouch );
	virtual void touched();

protected:
	///<override
	virtual const char* _getTileRectName() const;
	virtual void _extrudeImage( float fHeightExtruded );

protected:

	///<the gold count of this tile
	unsigned int m_nGoldCount;

	CCSprite *m_pSpriteGold;
};


}


#endif

