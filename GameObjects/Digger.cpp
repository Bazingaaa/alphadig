

#include "GameObjects/Digger.h"
#include "Script/LuaHelper.h"

#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"
#include "actions/CCActionInterval.h"

namespace AlphaDig
{

Digger::Digger()
: m_pDiggerMain( NULL )
, m_pDiggerGearL( NULL )
, m_pDiggerGearM( NULL )
, m_pGearRotateL( NULL )
, m_pGearRotateM( NULL )
{
}

Digger::~Digger()
{
}

void Digger::buildDigger( CCSpriteBatchNode *pParentBatchNode, float fTileSize )
{
	//create digger main node
	CCRect RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_NONE" );
	m_pDiggerMain = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->setAnchorPoint( ccp( 0.0f, 0.0f ) );
	pParentBatchNode->addChild( m_pDiggerMain );

	float fOffset = 0.0f;
	float fMaxRectWidth = 0.0f;
	//create drill
	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_DRILL" );
	if( fMaxRectWidth < RECT.size.width )
		fMaxRectWidth = RECT.size.width;
	CCSprite *pDrill = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( pDrill, 2 );
	fOffset = LuaHelper::s_getNumberVar( "OFFSET_DRILL" ) + RECT.size.height * 0.5f;
	pDrill->setPosition( ccp(0.0f, fOffset ) );
	//create drill cuff
	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_CUFF" );
	if( fMaxRectWidth < RECT.size.width )
		fMaxRectWidth = RECT.size.width;
	CCSprite *pCuff = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( pCuff, 2 );
	fOffset = LuaHelper::s_getNumberVar( "OFFSET_CUFF" ) + RECT.size.height * 0.5f;
	pCuff->setPosition( CCPoint( 0.0f, fOffset ) );

	//create the display body
	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_DISPLAY_BODY" );
	if( fMaxRectWidth < RECT.size.width )
		fMaxRectWidth = RECT.size.width;
	CCSprite *pDisplayBody = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( pDisplayBody, 2 );
	fOffset = LuaHelper::s_getNumberVar( "OFFSET_DISPLAY_BODY" ) + RECT.size.height * 0.5f;
	pDisplayBody->setPosition( CCPoint( 0.0f, fOffset ) );

	//create gear
	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_GEAR_L" );
	if( fMaxRectWidth < RECT.size.width )
		fMaxRectWidth = RECT.size.width;
	m_pDiggerGearL = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( m_pDiggerGearL, 0 );
	fOffset = LuaHelper::s_getNumberVar( "OFFSET_GEAR_L" ) + RECT.size.height * 0.5f;
	m_pDiggerGearL->setPosition( CCPoint( 0.0f, fOffset ) );

	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_GEAR_M" );
	if( fMaxRectWidth < RECT.size.width )
		fMaxRectWidth = RECT.size.width;
	m_pDiggerGearM = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( m_pDiggerGearM, 2 );
	fOffset = LuaHelper::s_getNumberVar( "OFFSET_GEAR_M" ) + RECT.size.height * 0.5f;
	m_pDiggerGearM->setPosition( CCPoint( 0.0f, fOffset ) );

	//create digger body
	RECT = LuaHelper::s_getRectVar("RECT_DIGGER_BODY");
	if( fMaxRectWidth < RECT.size.width )
		fMaxRectWidth = RECT.size.width;
	CCSprite *pBody = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( pBody, 1 );
	fOffset = LuaHelper::s_getNumberVar( "OFFSET_BODY" ) + RECT.size.height * 0.5f;
	pBody->setPosition( ccp( 0.0f, fOffset ) );

	m_pDiggerMain->setScale( fTileSize / fMaxRectWidth );

	//create the gear action
	CCRotateBy *pRotate = CCRotateBy::create( 1.0f, 40.0f );
	m_pGearRotateL = CCRepeatForever::create( pRotate );

	pRotate = CCRotateBy::create( 1.0f, -60.0f );
	m_pGearRotateM = CCRepeatForever::create( pRotate );
	m_pDiggerGearL->runAction( m_pGearRotateL );
	m_pDiggerGearM->runAction( m_pGearRotateM );
	m_pDiggerGearL->pauseSchedulerAndActions();
	m_pDiggerGearM->pauseSchedulerAndActions();

}

void Digger::setPosition( float fX, float fY )
{
	m_pDiggerMain->setPosition( ccp( fX, fY ) );
}

void Digger::setVelocity( float fVel )
{
	if( fVel > 0.1f )
	{
		m_pDiggerGearL->resumeSchedulerAndActions();
		m_pDiggerGearM->resumeSchedulerAndActions();
	}
	else
	{
		m_pDiggerGearL->pauseSchedulerAndActions();
		m_pDiggerGearM->pauseSchedulerAndActions();
	}
}

};