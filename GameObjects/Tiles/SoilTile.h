

#ifndef SOIL_TILE_H__
#define SOIL_TILE_H__

#include "cocoa/CCObject.h"

namespace cocos2d
{
	class CCSprite;
	class CCTouch;
}


namespace AlphaDig
{

using namespace cocos2d;

class DiggingPath;
/**
 * 土方
 */
class SoilTile : public CCObject
{
public:
	SoilTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight );
	~SoilTile();

	///< create/ destroy methods
	void create();
	void destroy();

	///<extrude tile
	virtual void extrude( float fHeightExtruded );

	///touches callback
	virtual bool isTouched( CCTouch *pTouch ){ return false; }
	virtual void touched(){}

	///<get the top height of this tile
	float getTopHeight() const { return m_nHeight - 0.5f; }

protected:

	///< get the tile image rect define name in script
	virtual const char* _getTileRectName() const ;

	void _extrudeImage( float fHeightExtruded );

	bool _isTouchedSprite( CCTouch *pTouch );

protected:

	unsigned int m_nColumn;

	///< the CCSprite obj of the tile
	CCSprite *m_pTileSprite;

	///<the initial sprite posY
	float m_fInitPosY;

	///<the host digging path
	DiggingPath *m_pHostPath;

	///<the tile height property
	CC_SYNTHESIZE_READONLY( unsigned int, m_nHeight, Height );

};

}



#endif


