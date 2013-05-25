

#ifndef MAIN_MENU_SCENE_H__
#define MAIN_MENU_SCENE_H__

namespace cocos2d
{
	class CCObject;
}


#include "layers_scenes_transitions_nodes/CCLayer.h"



namespace AlphaDig
{

using namespace cocos2d;


class LayerMainMenu : public cocos2d::CCLayer
{
public:
	LayerMainMenu();
	virtual ~LayerMainMenu();

	/**初始化layer
	 *
	 */
	bool initLayer();

	/**界面item的回调函数 */
	void onPlay( CCObject *pSender );
	void onQuit( CCObject *pSender );

	///< override
	virtual void keyBackClicked( CCObject *pSender );

protected:

	///<update main menu
	void _updateMainMenu( float fElapsedTime );

protected:

};


}
#endif