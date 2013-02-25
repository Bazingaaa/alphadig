
#ifndef DIGGING_PATH_H__
#define DIGGING_PATH_H__

#include "cocoa/CCObject.h"

#include <list>

namespace cocos2d
{
	class CCSet;
	class CCEvent;
}

namespace AlphaDig
{

using namespace cocos2d;

class Digger;
class SoilTile;
/**
 * 挖掘道
 */
class DiggingPath : public CCObject
{
public:
	DiggingPath( unsigned int nPathColumn );
	~DiggingPath();

	///<创建
	void create();

	/**
	 * 更新
	 * @param 一帧的时间
	 * @param 当前可视的最大高度
	 */
	virtual void update( float fElapsedTime );

	///<toches callback
	void touchesBegan( CCSet *pTouches, CCEvent *pEvent );
	void touchesMoved( CCSet *pTouches, CCEvent *pEvent );

	///<get tht ground height ,ie. the top tile's height
	float getGroundHeight() const;

	///<get the column of this path
	unsigned int getPathColumn( ) const { return m_nPathColumn; }

	///<get the path closed property
	bool isPathClosed( ) const { return m_bPathClosed; }

	///<notify the path to remove specified tile
	void notifyRemoveTile( SoilTile *pTile );

	///<get the digger obj
	Digger* getDigger() const { return m_pDigger; }

	///<respawn the digger
	void respawnDigger();

protected:

	///<移除某个指定土方
	void _removeTile( SoilTile *pTile );

	///<create new tile
	SoilTile* _createTile( bool bCreateInDigging, unsigned int nRow );

protected:

	///<挖掘机对象
	Digger *m_pDigger;

	///<挖掘的土方
	typedef std::list< SoilTile* > TileList;
	TileList m_soilTiles;

	///<是否已经创建完毕
	bool m_bCreated;

	///<挖掘道的位置(第几列）
	unsigned int m_nPathColumn;

	///<当前土方的最大高度
	unsigned int m_nCurrMaxHeightLvl;

	///< the path is closed
	bool m_bPathClosed;

	///<the tile will be delete in next loop
	TileList m_tilesToBeRemoved;

	///<if the touch began in this path
	bool m_bTouchBegan;




};



}

#endif

