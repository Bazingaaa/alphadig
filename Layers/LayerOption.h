

#ifndef LAYER_OPTION_H__
#define LAYER_OPTION_H__

namespace cocos2d
{
	class CCObject;
}


#include "layers_scenes_transitions_nodes/CCLayer.h"



namespace AlphaDig
{

using namespace cocos2d;

class LayerOption : public cocos2d::CCLayer
{
public:
	LayerOption();
	virtual ~LayerOption();

	/**初始化layer
	 *
	 */
	bool initLayer();

	void menuCallback();

	///< override
	virtual void keyBackClicked();

};


}



#endif
