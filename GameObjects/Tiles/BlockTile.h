
#ifndef BLOCK_TILE_H__
#define BLOCK_TILE_H__


#include "GameObjects/Tiles/SoilTile.h"
//#include "Utilities/FiniteStateMachine.h"

namespace AlphaDig
{

class BlockTile : public SoilTile//, FiniteStateMachine
{
public:

	enum BlockEvent
	{
		E_BE_DEGENERATE = 0
	};

	enum BlockState
	{
		E_BS_SOIL = 0,
		E_BS_BLOCK_LVL_1,
		E_BS_BLOCK_LVL_2,
		E_BS_BLOCK_LVL_3
	};

	BlockTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight );
	~BlockTile();

	///<override
	virtual void create();
	virtual void destroy();
	virtual bool isTouched( CCTouch *pTouch );
	virtual void touched();

protected:

	///<override from SoilTile
	virtual const char* _getTileRectName() const ;
	virtual void _extrudeImage( float fHeightExtruded );

	/*//<override from FiniteStateMachine
	virtual void _processInputEvent( int nEventID );
	virtual void _leaveCurrState( );
	virtual void _enterNewState( );*/


protected:

	CCSprite *m_pSpriteBlock;
};

}




#endif

