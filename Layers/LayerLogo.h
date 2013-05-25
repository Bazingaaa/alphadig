#ifndef LAYER_LOGO_H__
#define LAYER_LOGO_H__


#include "layers_scenes_transitions_nodes/CCLayer.h"

namespace AlphaDig
{

using namespace cocos2d;

class LayerLogo : public CCLayer
{

public:
	LayerLogo();
	~LayerLogo();

	///<create / destroy
	bool initLayer();
	void deleteLayer();

protected:

	///<update the time count to ending the logo layer
	void _updateTimeCount( float fElapsedTime );

protected:

	//logo layer time count
	float m_fTimeCount;

	///<logo layer duration
	float m_fDuration;

};


};


#endif