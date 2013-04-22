

#include "GameObjects/Digger.h"
#include "GameObjects/DiggingWorld.h"
#include "GameObjects/DiggingPath.h"
#include "Script/LuaHelper.h"
#include "Layers/LayerGaming.h"
#include "CocosExt/ADAnimate.h"

#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"
#include "touch_dispatcher/CCTouch.h"
#include "support/CCPointExtension.h"
#include "actions/CCAction.h"
#include "actions/CCActionInterval.h"

#include <algorithm>



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
, m_pDiggerBodySprite( NULL )
, m_pDiggerDrill( NULL )
, m_pDiggerGear( NULL )
, m_pAnimDrill( NULL )
, m_pAnimGear( NULL )
, m_pActionAnimDrill( NULL )
, m_pActionAnimGear( NULL )
, m_touchRect( )
{
}

Digger::~Digger()
{
}

void Digger::create()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pBatchNode = pLayerGaming->getDiggerBatchNode();
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();

	m_fDynFriction = m_fMiu * m_fMassive * s_fGravityAcc;

	//create the digger body
	CCRect RECT_DIGGER = LuaHelper::s_getRectVar("RECT_DIGGER_BODY");
	m_pDiggerBodySprite = CCSprite::createWithTexture( pBatchNode->getTexture(), RECT_DIGGER );
	pBatchNode->addChild( m_pDiggerBodySprite );


	//计算digger sprite的位置
	unsigned int nColumn = m_pHostPath->getPathColumn();

	float fDiggerSpriteX = 0.0f;
	float fDiggerSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( m_fHeight, static_cast<float>(nColumn), fDiggerSpriteX, fDiggerSpriteY );

	float fTileSize = pDiggingWorld->getTileSize();
	m_pDiggerBodySprite->setAnchorPoint( CCPoint( 0.5f, 0.0f ) );
	m_pDiggerBodySprite->setScaleX( fTileSize / RECT_DIGGER.size.width );
	//m_pDiggerBodySprite->setScaleY( fTileSize * 4.0f / RECT_DIGGER.size.height );
	m_pDiggerBodySprite->setPosition( ccp( fDiggerSpriteX, fDiggerSpriteY ) );

	//create the digger drill
	CCArray *pAnimFrames = CCArray::createWithCapacity(16);
	for( unsigned int i = 0; i < 16; ++i )
	{
		unsigned int x = i / 8;
		unsigned int y = i % 8;
		CCSpriteFrame *pFrame =
				CCSpriteFrame::createWithTexture(pBatchNode->getTexture(), CCRectMake(64*x + 64, 64*y, 64, 64) );
		pAnimFrames->addObject( pFrame );
	}
	m_pAnimDrill = CCAnimation::createWithSpriteFrames(pAnimFrames, 0.2f);
	m_pDiggerDrill =
			CCSprite::createWithSpriteFrame( static_cast< CCSpriteFrame* >( pAnimFrames->objectAtIndex(0) ) );
	m_pDiggerBodySprite->addChild( m_pDiggerDrill );
	m_pDiggerDrill->setAnchorPoint( ccp( 0.0f, 0.0f ) );
	m_pActionAnimDrill = ADAnimate::create( m_pAnimDrill );
	m_pDiggerDrill->runAction( CCRepeatForever::create( m_pActionAnimDrill ) );

	//create the digger gear
	pAnimFrames = CCArray::createWithCapacity(16);
	for( unsigned int i = 0; i < 16; ++i )
	{
		unsigned int x = i / 8;
		unsigned int y = i % 8;
		CCSpriteFrame *pFrame =
				CCSpriteFrame::createWithTexture(pBatchNode->getTexture(), CCRectMake(64*x + 192, 64*y, 64, 64) );
		pAnimFrames->addObject( pFrame );
	}
	m_pAnimGear = CCAnimation::createWithSpriteFrames(pAnimFrames, 1.0f);
	m_pDiggerGear =
			CCSprite::createWithSpriteFrame( static_cast< CCSpriteFrame* >( pAnimFrames->objectAtIndex(0) ) );
	m_pDiggerBodySprite->addChild( m_pDiggerGear );
	m_pDiggerGear->setAnchorPoint( CCPoint( 0.0f, 0.0f ) );
	m_pDiggerGear->setPosition( ccp( 0.0f, fTileSize * 3.5f ) );
	m_pActionAnimGear = ADAnimate::create( m_pAnimGear );
	m_pDiggerGear->runAction( CCRepeatForever::create( m_pActionAnimGear ) );

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

	//计算速度
	m_fVelocity = m_fMomentum / m_fMassive;
	//计算高度
	m_fHeight += fElapsedTime * m_fVelocity;

	m_fMomentum = 0.0f;

	// do height clamp
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();
	float fBottomHeight = pDiggingWorld->getCurrentBottomHeight();
	float fClampedHeight = std::min<float>( fBottomHeight, m_pHostPath->getTopBlockHeight() );
	if( m_fHeight > fClampedHeight )
	{
		m_fHeight = fClampedHeight;
		m_fVelocity = 0.0f;
	}

	//set the sprite height
	float fDiggerSpriteX = 0.0f;
	float fDiggerSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( m_fHeight, 0, fDiggerSpriteX, fDiggerSpriteY );
	m_pDiggerBodySprite->setPositionY( fDiggerSpriteY );

	//update the touch rect
	fDiggerSpriteX = m_pDiggerBodySprite->getPositionX();
	float fTileSize = pDiggingWorld->getTileSize();
	m_touchRect.setRect( fDiggerSpriteX - fTileSize * 0.5f,  fDiggerSpriteY - fTileSize * 0.5f,
						 fTileSize, fTileSize );

	//update the anim speed
	m_pAnimDrill->setDelayPerUnit( 2.0f / m_fVelocity );
	m_pActionAnimDrill->reCaculateSplitTimes();
	m_pAnimGear->setDelayPerUnit( 2.0f / m_fVelocity );
	m_pActionAnimGear->reCaculateSplitTimes();
}

bool Digger::isTouched( CCTouch *pTouch )
{
	if( isBrokenup() )
		return false;

	CCRect RECT_DIGGER = LuaHelper::s_getRectVar("RECT_DIGGER_BODY");

	CCPoint touchPoint = m_pDiggerBodySprite->convertTouchToNodeSpace( pTouch );

	return RECT_DIGGER.containsPoint( touchPoint );
}

bool Digger::isBrokenup() const
{
	return ( m_fHeight < DiggingWorld::sharedDiggingWorld()->getCurrentTopHeight() - 1.0f ) &&
		   ( m_fMomentum == 0.0f );
}

}
