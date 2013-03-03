
#ifndef SPAWN_DIGGER_H__
#define SPAWN_DIGGER_H__

#include "GameObjects/Tiles/SoilTile.h"

namespace AlphaDig
{

class SpawnDiggerTile : public SoilTile
{
public:

	enum SpawnDiggerEvent
	{
		E_SDE_DEGENERATE,
	};

	enum SpawnDiggerState
	{
		E_SDS_SOIL,
		E_SDS_NORMAL,
	};

	SpawnDiggerTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight );
	~SpawnDiggerTile();

	///<override
	virtual void create( );
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

	CCSprite *m_pSpriteSpawnDigger;

};


}



#endif

