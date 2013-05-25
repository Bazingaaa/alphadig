
#include "LayerLogo.h"
#include "Script/LuaHelper.h"
#include "SceneManager.h"

#include "CCDirector.h"
#include "sprite_nodes/CCSprite.h"



namespace AlphaDig
{

LayerLogo::LayerLogo()
: m_fTimeCount(0.0f)
, m_fDuration(0.5f)
{
}


LayerLogo::~LayerLogo()
{
}

bool LayerLogo::initLayer()
{
	CCRect logoRect = LuaHelper::s_getRectVar("RECT_LOGO");
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	CCSprite *pLogoSprite = CCSprite::create( LuaHelper::s_getStringVar("IMAGE_LOGO").c_str(), logoRect );

	pLogoSprite->setPosition( CCPoint( s.width/2, s.height/2) );
	pLogoSprite->setScaleX( s.width / logoRect.size.width );
	pLogoSprite->setScaleY( s.height / logoRect.size.height );

	addChild( pLogoSprite );

	schedule( schedule_selector( LayerLogo::_updateTimeCount ), 1.0f );

	return true;
}

void LayerLogo::deleteLayer()
{
}

void LayerLogo::_updateTimeCount( float fElapsedTime )
{
	m_fTimeCount += fElapsedTime;
	if( m_fTimeCount >= m_fDuration )
	{
		m_fTimeCount = 0.0f;
		SceneManager::replaceMainMenuScene();
	}
}

};
