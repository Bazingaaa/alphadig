
#ifndef DIGGING_WORLD_H__
#define DIGGING_WORLD_H__

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

class DiggingPath;
/**
 * digging世界的各种对象的管理类
 */
class DiggingWorld : public CCObject
{
public:

	DiggingWorld();
	~DiggingWorld();

	void startNewWorld();

	void clearWorld();

	virtual void update( float fElapsedTime );

	///<toches callback
	void touchesBegan( CCSet *pTouches, CCEvent *pEvent );
	void touchesMoved( CCSet *pTouches, CCEvent *pEvent );

	///<get the max path count
	unsigned int getMaxPathCount() const { return m_nMaxPathCount; }
	///<get the tile size
	float getTileSize() const { return  m_fTileSize; }
	///<get the tile count per column
	float getTileCountPerColumn() const { return m_nTileCountPerColumn; }

	///<get the shared world
	static DiggingWorld* sharedDiggingWorld() { return s_pSharedDiggingWorld; }

	///< convert the digging coordinate to openGL coordinate
	void convertToGLCoordinate( float fRow, float fColumn, float &fOutX, float &fOutY );

	///<get current bottom height
	float getCurrentBottomHeight() const { return m_fCurrentBottomHeight; }
	///<get current top height
	float getCurrentTopHeight() const { return m_fCurrentBottomHeight - m_fTileCountPerColumn; }

	///<reset one closed path
	void resetOneClosedPath();

protected:

	void _createOneDiggingPath();

protected:

	typedef std::list< DiggingPath* > DiggingPathList;
	DiggingPathList m_diggingPaths;

	const unsigned int m_nMaxPathCount; ///< the max path count
	float m_fTileSize; ///<size per soil tile , cal by window size
	unsigned int m_nTileCountPerColumn; ///< number of tiles per column
	float m_fTileCountPerColumn; ///< number of tiles per column in float
	float m_fCurrentBottomHeight; ///< current button height


	///< the latest constructed DiggingWorld obj ptr
	static DiggingWorld *s_pSharedDiggingWorld;


};


}



#endif

