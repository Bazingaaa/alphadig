

#ifndef LAYER_GAMING_H__
#define LAYER_GAMING_H__

#include "layers_scenes_transitions_nodes/CCLayer.h"


namespace cocos2d
{
	class CCSpriteBatchNode;
	class CCLabelTTF;
	class CCSprite;
}

namespace AlphaDig
{

class DiggingWorld;

using namespace cocos2d;

class LayerGaming : public CCLayer
{
public:
	LayerGaming();
	~LayerGaming();


	/**初始化layer
	 *
	 */
	bool initLayer();

	/**销毁 */
	void deleteLayer();

	///< override
	virtual void keyBackClicked();
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	//virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();

	///<return the latest constructed layer gaming pointer
	static LayerGaming* sharedLayerGaming() { return s_pSharedLayerGaming; }

	///< get the main sprite batch node
	CCSpriteBatchNode* getMainSpriteBatchNode() { return m_pMainBatchNode; }

	///<update the gold hud
	void updateGoldHUD();

protected:

	///<create the sprite batch node
	void _createSpriteBatchNode( );

	///<update the game
	void _updateGame( float fElapsedTime );

	///<create/destroy/update the gaming HUD ui
	void _createHUD();
	void _destroyHUD();
	void _updateHUD();

protected:

	///<挖的世界对象
	DiggingWorld *m_pDiggingWorld;

	static LayerGaming *s_pSharedLayerGaming;

	///<the bathed sprite node
	CCSpriteBatchNode *m_pMainBatchNode;
	CCSpriteBatchNode *m_pMainBatchNodeForHUD;

	///<some hud UI elements
	CCLabelTTF *m_pCurrentLvlLabel;
	unsigned int m_nCurrentDisplayedLvlValue;
	CCLabelTTF *m_pGoldObtainedLabel;
	CCSprite *m_pGoldImg;
	CCSprite *m_pLvlImg;

};

}


#endif
