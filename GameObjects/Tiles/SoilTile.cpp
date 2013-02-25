
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
: m_nColumn( nColumn )
, m_nHeight( nHeight )
, m_pTileSprite( NULL )
, m_fInitPosY( 0 )
, m_pHostPath( pHostPath )
{
}


SoilTile::~SoilTile()
{

}

void SoilTile::create()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();

	CCRect RECT_SOIL_TILE = LuaHelper::s_getRectVar( _getTileRectName() );
	m_pTileSprite = CCSprite::createWithTexture( pMainBatchNode->getTexture(), RECT_SOIL_TILE );
	pMainBatchNode->addChild( m_pTileSprite );

	//计算sprite的位置
	float fSoilSpriteX = 0.0f;
	float fSoilSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( m_nHeight, m_nColumn, fSoilSpriteX, fSoilSpriteY );

	float fTileSize = pDiggingWorld->getTileSize();
	m_pTileSprite->setScaleX( fTileSize / RECT_SOIL_TILE.size.width );
	m_pTileSprite->setScaleY( fTileSize / RECT_SOIL_TILE.size.height );

	m_pTileSprite->setPosition( ccp( fSoilSpriteX, fSoilSpriteY ) );
	m_fInitPosY = fSoilSpriteY;


}

void SoilTile::destroy()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();

	pMainBatchNode->removeChild( m_pTileSprite, true );

	m_pTileSprite = NULL;
}

void SoilTile::extrude( float fHeightExtruded )
{
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
	return "RECT_SOIL_TILE";
}

void SoilTile::_extrudeImage( float fHeightExtruded )
{
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();
	float fTileSize = pDiggingWorld->getTileSize();

	CCRect RECT_TILE = LuaHelper::s_getRectVar( _getTileRectName() );
	RECT_TILE.origin.y += RECT_TILE.size.height * fHeightExtruded;
	RECT_TILE.size.height = RECT_TILE.size.height * ( 1.0f - fHeightExtruded );
	m_pTileSprite->setTextureRect( RECT_TILE );
	m_pTileSprite->setPositionY( m_fInitPosY - fTileSize * fHeightExtruded * 0.5f );
}

bool SoilTile::_isTouchedSprite( CCTouch *pTouch )
{
	/*this quad was not transformed by parents
	ccV3F_C4B_T2F_Quad tileQuad = m_pTileSprite->getQuad();
	CCPoint touchP = pTouch->getLocation();

	if( tileQuad.tl.vertices.x < touchP.x && tileQuad.tl.vertices.y < touchP.y &&
		tileQuad.br.vertices.x >= touchP.x && tileQuad.br.vertices.y >= touchP.y )
	{
		return true;
	}

	return false;*/

	CCRect RECT = LuaHelper::s_getRectVar( _getTileRectName() );
	RECT.origin.x = 0.0f;
	RECT.origin.y = 0.0f;

	CCPoint touchPoint = m_pTileSprite->convertTouchToNodeSpace( pTouch );

	return RECT.containsPoint( touchPoint );
}

}

