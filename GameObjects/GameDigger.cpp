

#include "GameObjects/GameDigger.h"
#include "GameObjects/DiggingWorld.h"
#include "GameObjects/DiggingPath.h"
#include "GameObjects/Digger.h"
#include "Script/LuaHelper.h"
#include "Layers/LayerGaming.h"

#include "sprite_nodes/CCSprite.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"
#include "actions/CCAction.h"
#include "actions/CCActionInterval.h"

#include <algorithm>



namespace AlphaDig
{

const float GameDigger::s_fGravityAcc = 9.8f;

GameDigger::GameDigger( DiggingPath *pHostPath )
: m_pHostPath( pHostPath )
, m_fMassive( 1.0f )
, m_fHeight( 0.0f )
, m_fVelocity( 0.0f )
, m_fMomentum( 0.0f )
, m_fMiu( 1.0f )
, m_fDynFriction( 0.0f )
, m_pDigger( NULL )
, m_touchRect( )
{
	m_timeStartDragging.tv_sec = 0;
	m_timeStartDragging.tv_usec = 0;
}

GameDigger::~GameDigger()
{
	CC_SAFE_RELEASE_NULL( m_pDigger );
}

void GameDigger::create()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pBatchNode = pLayerGaming->getDiggerBatchNode();
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();

	m_fDynFriction = m_fMiu * m_fMassive * s_fGravityAcc;

	//计算digger sprite的位置
	unsigned int nColumn = m_pHostPath->getPathColumn();

	float fDiggerSpriteX = 0.0f;
	float fDiggerSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( m_fHeight, static_cast<float>(nColumn), fDiggerSpriteX, fDiggerSpriteY );
	float fTileSize = pDiggingWorld->getTileSize();

	m_pDigger = new Digger();
	m_pDigger->buildDigger( pBatchNode, fTileSize );
	m_pDigger->setPosition( fDiggerSpriteX, fDiggerSpriteY );
	
}

void GameDigger::startDragging()
{
	CCTime::gettimeofdayCocos2d( &m_timeStartDragging, NULL );
}

void GameDigger::dig( float fForce )
{
	cc_timeval now;
	CCTime::gettimeofdayCocos2d( &now, NULL );
	float fTimeDif = static_cast<float>( CCTime::timersubCocos2d( &m_timeStartDragging, &now ) );
	fTimeDif /= 1000.0f;

	m_timeStartDragging = now;

	m_fMomentum += fForce * fTimeDif;
}

void GameDigger::update( float fElapsedTime )
{
	//do nothing if it is broken up
	if( isBrokenup() )
		return;

	
	float fReactionForce = 0.0f; //反作用力
	float fGroundHeight = m_pHostPath->getGroundHeight();
	if( m_fHeight < fGroundHeight )//重力下落
		m_fVelocity += fElapsedTime * s_fGravityAcc;
	else
		fReactionForce = m_fDynFriction; //钻地时，反作用力为动摩擦力

	//动量计算
	m_fMomentum -= fReactionForce * fElapsedTime;

	if( m_fMomentum < 0.0f )
		m_fMomentum = 0.0f;

	//计算速度
	m_fVelocity = m_fMomentum / m_fMassive;
	//计算高度
	m_fHeight += fElapsedTime * m_fVelocity;

	// do height clamp
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();
	float fBottomHeight = pDiggingWorld->getCurrentBottomHeight();
	float fClampedHeight = std::min<float>( fBottomHeight, m_pHostPath->getTopBlockHeight() );
	if( m_fHeight > fClampedHeight )
	{
		m_fHeight = fClampedHeight;
		m_fMomentum = 0.0f;
	}

	//set the sprite height
	float fDiggerSpriteX = 0.0f;
	float fDiggerSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( m_fHeight, m_pHostPath->getPathColumn(), fDiggerSpriteX, fDiggerSpriteY );
	m_pDigger->setPosition( fDiggerSpriteX, fDiggerSpriteY );
	m_pDigger->setVelocity( m_fVelocity );

	//update the touch rect
	float fTileSize = pDiggingWorld->getTileSize();
	m_touchRect.setRect( fDiggerSpriteX - fTileSize * 0.5f,  fDiggerSpriteY - fTileSize * 0.5f,
						 fTileSize, fTileSize );
}

bool GameDigger::isTouched( CCTouch *pTouch )
{
	if( isBrokenup() )
		return false;

	CCPoint touchPoint = m_pDigger->getDiggerMainNode()->convertTouchToNodeSpace( pTouch );

	return touchPoint.y > 0.0f;
}

bool GameDigger::isBrokenup() const
{
	return ( m_fHeight < DiggingWorld::sharedDiggingWorld()->getCurrentTopHeight() - 1.0f ) &&
		   ( m_fMomentum == 0.0f );
}


}
