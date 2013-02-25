

#include "GameObjects/Digger.h"
#include "GameObjects/DiggingWorld.h"
#include "GameObjects/DiggingPath.h"
#include "Script/LuaHelper.h"
#include "Layers/LayerGaming.h"

#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"
#include "actions/CCAction.h"
#include "actions/CCActionInterval.h"



namespace AlphaDig
{

const float Digger::s_fGravityAcc = 9.8f;

Digger::Digger( DiggingPath *pHostPath )
: m_pHostPath( pHostPath )
, m_fMassive( 1.0f )
, m_fHeight( 0.0f )
, m_fVelocity( 0.0f )
, m_fCurrentDriveForce( 0.0f )
, m_fMomentum( 0.0f )
, m_fMiu( 1.0f )
, m_fDynFriction( 0.0f )
, m_pDiggerSprite( NULL )
, m_touchRect( )
, m_fDurability( 100.0f )
, m_fInvincibleTime( 2.0f )
, m_fInvincibleTimeCount( 0.0f )
, m_bInvincibleFlash( false )
, m_pInvincibleFlash( NULL )
{
}

Digger::~Digger()
{
	CC_SAFE_RELEASE_NULL( m_pInvincibleFlash );
}

void Digger::create()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();


	CCRect RECT_DIGGER = LuaHelper::s_getRectVar("RECT_DIGGER");

	m_pDiggerSprite = CCSprite::createWithTexture( pMainBatchNode->getTexture(), RECT_DIGGER );
	pMainBatchNode->addChild( m_pDiggerSprite );


	//计算digger sprite的位置
	unsigned int nColumn = m_pHostPath->getPathColumn();

	float fDiggerSpriteX = 0.0f;
	float fDiggerSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( m_fHeight, nColumn, fDiggerSpriteX, fDiggerSpriteY );

	float fTileSize = pDiggingWorld->getTileSize();
	m_pDiggerSprite->setAnchorPoint( CCPoint( 0.5f, 0.0f ) );
	m_pDiggerSprite->setScaleX( fTileSize / RECT_DIGGER.size.width );
	m_pDiggerSprite->setScaleY( fTileSize * 4.0f / RECT_DIGGER.size.height );
	m_pDiggerSprite->setPosition( ccp( fDiggerSpriteX, fDiggerSpriteY ) );

	m_fDynFriction = m_fMiu * m_fMassive * s_fGravityAcc;

	//invincible flash obj
	CCActionInterval* pTintYellow = CCTintBy::create(0.5f, 0, 0, -255);
	CCActionInterval* pTintYellowBack = pTintYellow->reverse();
	m_pInvincibleFlash = CCRepeatForever::create( (CCActionInterval*)(CCSequence::create( pTintYellow, pTintYellowBack, NULL)) );
	m_pInvincibleFlash->retain();
}

void Digger::dig( float fForce )
{
	m_fCurrentDriveForce = fForce;
}

void Digger::update( float fElapsedTime )
{
	//do nothing if it is broken up
	if( isBrokenup() )
		return;

	_updateInvincible( fElapsedTime );


	float fReactionForce = 0.0f; //反作用力
	float fGroundHeight = m_pHostPath->getGroundHeight();
	if( m_fHeight < fGroundHeight )//重力下落
		m_fVelocity += fElapsedTime * s_fGravityAcc;
	else
		fReactionForce = m_fDynFriction; //钻地时，反作用力为动摩擦力

	//动量计算
	m_fMomentum += m_fVelocity * m_fMassive;
	m_fMomentum += m_fCurrentDriveForce * fElapsedTime;
	m_fCurrentDriveForce = 0.0f;

	m_fMomentum -= fReactionForce * fElapsedTime;

	if( m_fMomentum < 0.0f )
		m_fMomentum = 0.0f;

	// do height clamp
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();
	float fBottomHeight = pDiggingWorld->getCurrentBottomHeight();
	if( m_fHeight > fBottomHeight )
	{
		m_fHeight = fBottomHeight;
		m_fMomentum = 0.0f;
	}

	//计算速度
	m_fVelocity = m_fMomentum / m_fMassive;

	//计算高度
	m_fHeight += fElapsedTime * m_fVelocity;

	m_fMomentum = 0.0f;

	//set the sprite height
	float fDiggerSpriteX = 0.0f;
	float fDiggerSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( m_fHeight, 0, fDiggerSpriteX, fDiggerSpriteY );
	m_pDiggerSprite->setPositionY( fDiggerSpriteY );

	//update the touch rect
	fDiggerSpriteX = m_pDiggerSprite->getPositionX();
	float fTileSize = pDiggingWorld->getTileSize();
	m_touchRect.setRect( fDiggerSpriteX - fTileSize * 0.5f,  fDiggerSpriteY - fTileSize * 0.5f,
						 fTileSize, fTileSize );
}

bool Digger::isTouched( CCTouch *pTouch )
{
	if( isBrokenup() )
		return false;

	CCRect RECT_DIGGER = LuaHelper::s_getRectVar("RECT_DIGGER");

	CCPoint touchPoint = m_pDiggerSprite->convertTouchToNodeSpace( pTouch );

	return RECT_DIGGER.containsPoint( touchPoint );
}

bool Digger::isBrokenup() const
{
	if( _isInvincible() )
		return false;

	return m_fDurability <= 0.0f ||
		   m_fHeight < DiggingWorld::sharedDiggingWorld()->getCurrentTopHeight();
}

void Digger::modifyDurability( float fChangeValue )
{
	if( fChangeValue < 0.0f && _isInvincible() )
		return;

	m_fDurability -= fChangeValue;
}

void Digger::_updateInvincible( float fElapsedTime )
{
	//update invincible time count
	if( _isInvincible() )
	{
		m_fInvincibleTimeCount += fElapsedTime;
		if( !m_bInvincibleFlash )
		{
			m_pDiggerSprite->runAction( m_pInvincibleFlash );
			m_bInvincibleFlash = true;
		}
	}
	else
	{
		if( m_bInvincibleFlash )
		{
			m_pDiggerSprite->stopAction( m_pInvincibleFlash );
			m_pDiggerSprite->setColor( ccc3( 255, 255, 255 ) );
			m_bInvincibleFlash = false;
		}
	}
}

}