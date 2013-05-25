

#include "GameObjects/Digger.h"
#include "Script/LuaHelper.h"

#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"

namespace AlphaDig
{

Digger::Digger()
: m_pDiggerMain( NULL )
, m_pDiggerGearL( NULL )
, m_pDiggerGearM( NULL )
, m_fDrillScale( 1.0f )
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
	//create drill
	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_DRILL" );
	CCSprite *pDrill = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( pDrill );
	m_fDrillScale = fTileSize / RECT.size.width;
	pDrill->setScale( m_fDrillScale );
	fOffset += RECT.size.height * 0.5f;
	pDrill->setPosition( ccp(0.0f, fOffset ) );
	fOffset += RECT.size.height * 0.5f;
	//create drill cuff
	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_CUFF" );
	CCSprite *pCuff = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( pCuff );
	pCuff->setScale( fTileSize / RECT.size.width );
	fOffset += RECT.size.height * 0.5f;
	pCuff->setPosition( CCPoint( 0.0f, fOffset ) );
	fOffset += RECT.size.height * 0.5f;

	//create the display body
	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_DISPLAY_BODY" );
	CCSprite *pDisplayBody = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( pDisplayBody );
	pDisplayBody->setScale( fTileSize / RECT.size.width );
	fOffset += RECT.size.height * 0.5f;
	pDisplayBody->setPosition( CCPoint( 0.0f, fOffset ) );
	fOffset += RECT.size.height * 0.5f;

	//create gear
	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_GEAR_L" );
	m_pDiggerGearL = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( m_pDiggerGearL );
	m_pDiggerGearL->setScale( fTileSize / RECT.size.width );
	fOffset += RECT.size.height * 0.5f;
	m_pDiggerGearL->setPosition( CCPoint( 0.0f, fOffset ) );
	fOffset += RECT.size.height * 0.5f;

	RECT = LuaHelper::s_getRectVar( "RECT_DIGGER_GEAR_M" );
	m_pDiggerGearM = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( m_pDiggerGearM );
	m_pDiggerGearM->setScale( fTileSize / RECT.size.width );
	fOffset += RECT.size.height * 0.5f;
	m_pDiggerGearM->setPosition( CCPoint( 0.0f, fOffset ) );
	fOffset += RECT.size.height * 0.5f;

	//create digger body
	RECT = LuaHelper::s_getRectVar("RECT_DIGGER_BODY");
	CCSprite *pBody = CCSprite::createWithTexture( pParentBatchNode->getTexture(), RECT );
	m_pDiggerMain->addChild( pBody );
	pBody->setScale( fTileSize / RECT.size.width );
	fOffset += RECT.size.height * 0.5f;
	pBody->setPosition( ccp( 0.0f, fOffset ) );
}

void Digger::setPosition( float fX, float fY )
{
	m_pDiggerMain->setPosition( ccp( fX, fY ) );
}

void Digger::setVelocity( float fVel )
{

}

};