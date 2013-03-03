

#ifndef SOIL_TILE_H__
#define SOIL_TILE_H__

#include "Utilities/FiniteStateMachine.h"

#include <string>
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
class SoilTile : public CCObject, public FiniteStateMachine
{
public:
	SoilTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight );
	~SoilTile();

	///< create/ destroy methods
	virtual void create();
	virtual void destroy();

	///< get tile type
	const std::string& getTileType()const { return m_strTileType; }

	///<extrude tile
	void extrude( float fHeightExtruded );

	///touches callback
	virtual bool isTouched( CCTouch *pTouch ){ return false; }
	virtual void touched(){}

	///<get the top height of this tile
	float getTopHeight() const { return m_nHeight - 0.5f; }

protected:

	///<create a tile with a rect name
	CCSprite* _createTileSprite( const char *pRectName );

	///<destroy sprite
	void _destroySprite( CCSprite *pSprite );

	///< get the tile image rect define name in script
	virtual const char* _getTileRectName() const ;

	///<extrude image when digger dig into this tile
	virtual void _extrudeImage( float fHeightExtruded );
	void _extrudeImageImp( float fHeightExtruded, const char *pImageRectName, CCSprite *pSprite );

	bool _isTouchedSprite( CCTouch *pTouch );


	///<override from FiniteStateMachine
	virtual void _processInputEvent( int nEventID ){}
	virtual void _leaveCurrState( ){}
	virtual void _enterNewState( ){}

protected:

	///<the type of this tile
	std::string m_strTileType;

	unsigned int m_nColumn;

	///< the CCSprite obj of the tile
	CCSprite *m_pSpriteSoil;

	///<the initial sprite posY
	float m_fInitPosY;

	///<the host digging path
	DiggingPath *m_pHostPath;



	///<the tile height property
	CC_SYNTHESIZE_READONLY( unsigned int, m_nHeight, Height );

};

}



#endif


