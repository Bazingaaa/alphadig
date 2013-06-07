
#include "GameObjects/Tiles/SoilTile.h"
#include "GameObjects/DiggingWorld.h"
#include "GameObjects/DiggingPath.h"
#include "Script/LuaHelper.h"
#include "Layers/LayerGaming.h"

#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSprite.h"
#include "support/CCPointExtension.h"

namespace AlphaDig
{

SoilTile::SoilTile( DiggingPath *pHostPath, unsigned int nColumn, unsigned int nHeight )
: FiniteStateMachine( 0 )
, m_strTileType( "SOIL" )
, m_nColumn( nColumn )
, m_nHeight( nHeight )
, m_pSpriteSoil( NULL )
, m_fInitPosY( 0 )
, m_pHostPath( pHostPath )
, m_pEdgeSprite( NULL )
, m_fHeightExtruded( 0.0f )
{
}


SoilTile::~SoilTile()
{

}

void SoilTile::create()
{
	m_pSpriteSoil = _createTileSprite( SoilTile::_getTileRectName() );

	m_pEdgeSprite = _createTileSprite( getTileEdgeName(), -1 );
	m_pEdgeSprite->setScaleX( m_pSpriteSoil->getScaleX() );
	m_pEdgeSprite->setScaleY( m_pSpriteSoil->getScaleY() );
}

void SoilTile::destroy()
{
	DiggingWorld::s_destroyTileSprite( m_pSpriteSoil );
	m_pSpriteSoil = NULL;

	DiggingWorld::s_destroyTileSprite( m_pEdgeSprite );
	m_pEdgeSprite = NULL;
}

void SoilTile::extrude( float fHeightExtruded )
{
	m_fHeightExtruded = fHeightExtruded;

	if( fHeightExtruded > 0.95f )
	{
		m_pHostPath->notifyRemoveTile( this );
	}
	else
	{
		_extrudeImage( fHeightExtruded );
	}

}

const char* SoilTile::_getTileRectName() const
{
	return "RECT_TILE_SOIL_STYLE_1";
}

void SoilTile::_extrudeImage( float fHeightExtruded )
{
	_extrudeImageImp( fHeightExtruded, SoilTile::_getTileRectName(), m_pSpriteSoil );
	_extrudeImageImp( fHeightExtruded, getTileEdgeName(), m_pEdgeSprite );
}

void SoilTile::_extrudeImageImp( float fHeightExtruded, const char *pImageRectName, CCSprite *pSprite )
{
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();
	float fTileSize = pDiggingWorld->getTileSize();

	CCRect RECT_TILE = LuaHelper::s_getRectVar( pImageRectName );
	RECT_TILE.origin.y += RECT_TILE.size.height * fHeightExtruded;
	RECT_TILE.size.height = RECT_TILE.size.height * ( 1.0f - fHeightExtruded );
	pSprite->setTextureRect( RECT_TILE );
	pSprite->setPositionY( m_fInitPosY - fTileSize * fHeightExtruded * 0.5f );
}

bool SoilTile::_isTouchedSprite( CCTouch *pTouch )
{
	/*this quad was not transformed by parents
	ccV3F_C4B_T2F_Quad tileQuad = m_pSpriteSoil->getQuad();
	CCPoint touchP = pTouch->getLocation();

	if( tileQuad.tl.vertices.x < touchP.x && tileQuad.tl.vertices.y < touchP.y &&
		tileQuad.br.vertices.x >= touchP.x && tileQuad.br.vertices.y >= touchP.y )
	{
		return true;
	}

	return false;*/

	CCRect RECT = LuaHelper::s_getRectVar( SoilTile::_getTileRectName() );
	RECT.origin.x = 0.0f;
	RECT.origin.y = 0.0f;

	CCPoint touchPoint = m_pSpriteSoil->convertTouchToNodeSpace( pTouch );

	return RECT.containsPoint( touchPoint );
}

CCSprite* SoilTile::_createTileSprite( const char* pRectName, int nZorder )
{
	CCSprite *pRetSprite = DiggingWorld::s_createTileSprite( pRectName, m_nColumn, m_nHeight, 1.0f, 1.0f, nZorder );
	
	m_fInitPosY = pRetSprite->getPositionY();

	return pRetSprite;
}

}

