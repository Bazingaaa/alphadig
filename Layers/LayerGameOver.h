
#ifndef LAYER_GAME_OVER_H__
#define LAYER_GAME_OVER_H__


#include "layers_scenes_transitions_nodes/CCLayer.h"

namespace cocos2d
{
	class CCObject;
}

namespace AlphaDig
{

using namespace cocos2d;

class LayerGameOver : public CCLayer
{

public:
	LayerGameOver();
	~LayerGameOver();


	///<create / destroy
	bool initLayer();
	void deleteLayer();


	///< UI item callback
	void onPlayAgain( CCObject *pSender );
	void onToMainMenu( CCObject *pSender );

	///< override
	virtual void keyBackClicked( CCObject *pSender );


};


}

#endif

