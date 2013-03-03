
#ifndef GOLD_TILE_H__
#define GOLD_TILE_H__

#include "GameObjects/Tiles/SoilTile.h"

namespace AlphaDig
{

class DiggingPath;

class GoldTile : public SoilTile
{
public:

	enum GoldEvent
	{
		E_GE_DEGENERATE,
	};

	enum GoldState
	{
		E_GS_SOIL,
		E_GS_GOLD,
	};

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

	///<override
	virtual void _processInputEvent( int nEventID );
	virtual void _leaveCurrState( );
	virtual void _enterNewState( );

protected:

	///<the gold count of this tile
	unsigned int m_nGoldCount;

	CCSprite *m_pSpriteGold;
};


}


#endif

