

#ifndef GAME_DIGGER_H__
#define GAME_DIGGER_H__


#include "platform/platform.h"
#include "cocoa/CCObject.h"
#include "cocoa/CCGeometry.h"


namespace cocos2d
{
	class CCTouch;
}

namespace AlphaDig
{

class DiggingPath;
class Digger;

using namespace cocos2d;
/**
 * 挖掘机
 */
class GameDigger : public CCObject
{
public:
	GameDigger( DiggingPath* pHostPath );
	~GameDigger();

	///<创建
	void create();

	/**
	 * 挖
	 * @param 挖的力的大小
	 */
	void dig( float fForce );

	///<更新
	virtual void update( float fElapsedTime );

	float getHeight() const { return m_fHeight; }

	const CCRect& getTouchRect() const { return m_touchRect; }

	///< see if the touch hit the digger
	bool isTouched( CCTouch *pTouch );

	///<set the momentum
	void setMomentum( float fMomentum ) { m_fMomentum = fMomentum; }

	bool isBrokenup() const;

	void startDragging();

public:
	///<重力加速度
	static const float s_fGravityAcc;

protected:
	///<这个挖掘机所在的挖掘道
	DiggingPath *m_pHostPath;

	///<挖掘机的质量
	float m_fMassive;

	///<挖掘机高度
	float m_fHeight;

	///<下落速度
	float m_fVelocity;

	///<draged or not
	bool m_bDragged;

	///<当前的动量
	float m_fMomentum;

	///<动摩擦因素
	float m_fMiu;

	///<动摩擦力
	float m_fDynFriction;

	///<the digger
	Digger *m_pDigger;

	///<the touch rect of this digger
	CCRect m_touchRect;

	///< time start dragging
	cc_timeval m_timeStartDragging;


};

}


#endif

