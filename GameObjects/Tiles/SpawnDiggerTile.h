
#ifndef SPAWN_DIGGER_H__
#define SPAWN_DIGGER_H__

#include "GameObjects/Tiles/SoilTile.h"

namespace AlphaDig
{

class SpawnDiggerTile : public SoilTile
{
public:
	SpawnDiggerTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight );
	~SpawnDiggerTile();

	///<override
	virtual void extrude( float fHeightExtruded );
	virtual bool isTouched( CCTouch *pTouch );
	virtual void touched();

protected:
	///<override
	virtual const char* _getTileRectName() const;

};


}



#endif

