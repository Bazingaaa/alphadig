

#ifndef DIGGER_H__
#define DIGGER_H__


#include "platform/platform.h"
#include "cocoa/CCObject.h"
#include "cocoa/CCGeometry.h"



namespace cocos2d
{
	class CCSprite;
	class CCSpriteBatchNode;
}

namespace AlphaDig
{


using namespace cocos2d;
/**
 * 挖掘机
 */
class Digger : public CCObject
{
public:
	Digger();
	~Digger();

	//build a digger sprite( include build up drill and other parts )
	void buildDigger( CCSpriteBatchNode *pParentBatchNode, float fTileSize );

	//setPosition
	void setPosition( float fX, float fY );

	//set velocity
	void setVelocity( float fVel );

	//get the main node, should return a const point, but many methods in CCNode are non-const while they should be.
	CCSprite* getDiggerMainNode() const { return m_pDiggerMain; }


protected:

	///<CCSprite obj ptr of the digger
	CCSprite *m_pDiggerMain;
	CCSprite *m_pDiggerGearL;
	CCSprite *m_pDiggerGearM;

	//the scale of the digger drill to fill the tile width
	float m_fDrillScale;

};

};


#endif

