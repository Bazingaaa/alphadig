
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
: m_strTileType( "SOIL" )
, m_nColumn( nColumn )
, m_nHeight( nHeight )
, m_pSpriteSoil( NULL )
, m_fInitPosY( 0 )
, m_pHostPath( pHostPath )
{
}


SoilTile::~SoilTile()
{

}

void SoilTile::create()
{
	m_pSpriteSoil = _createTileSprite( SoilTile::_getTileRectName() );
}

void SoilTile::destroy()
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();

	pMainBatchNode->removeChild( m_pSpriteSoil, true );

	m_pSpriteSoil = NULL;
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
	_extrudeImageImp( fHeightExtruded, SoilTile::_getTileRectName(), m_pSpriteSoil );
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

CCSprite* SoilTile::_createTileSprite( const char* pRectName )
{
	LayerGaming *pLayerGaming = LayerGaming::sharedLayerGaming();
	CCSpriteBatchNode *pMainBatchNode = pLayerGaming->getMainSpriteBatchNode();
	DiggingWorld *pDiggingWorld = DiggingWorld::sharedDiggingWorld();

	CCSprite *pRetSprite = NULL;
	CCRect RECT_TILE = LuaHelper::s_getRectVar( pRectName );
	pRetSprite = CCSprite::createWithTexture( pMainBatchNode->getTexture(), RECT_TILE );
	pMainBatchNode->addChild( pRetSprite );

	//计算sprite的位置
	float fSoilSpriteX = 0.0f;
	float fSoilSpriteY = 0.0f;
	pDiggingWorld->convertToGLCoordinate( m_nHeight, m_nColumn, fSoilSpriteX, fSoilSpriteY );

	float fTileSize = pDiggingWorld->getTileSize();
	pRetSprite->setScaleX( fTileSize / RECT_TILE.size.width );
	pRetSprite->setScaleY( fTileSize / RECT_TILE.size.height );

	pRetSprite->setPosition( ccp( fSoilSpriteX, fSoilSpriteY ) );
	m_fInitPosY = fSoilSpriteY;

	return pRetSprite;
}

}

