

#include "CocosExt/ADAnimate.h"

#include <vector>


namespace AlphaDig
{


ADAnimate::ADAnimate()
{

}

ADAnimate::~ADAnimate()
{

}


void ADAnimate::reCaculateSplitTimes()
{
	if( getAnimation() == NULL )
		return;

	CCAnimation *pAnimation = getAnimation();

	float singleDuration = pAnimation->getDuration();

	m_fDuration = singleDuration * pAnimation->getLoops();

	m_pSplitTimes->clear();
	m_pSplitTimes->reserve(pAnimation->getFrames()->count());

	float accumUnitsOfTime = 0;
	float newUnitOfTimeValue = singleDuration / pAnimation->getTotalDelayUnits();

	CCArray* pFrames = pAnimation->getFrames();
	CCARRAY_VERIFY_TYPE(pFrames, CCAnimationFrame*);

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(pFrames, pObj)
	{
		CCAnimationFrame* frame = (CCAnimationFrame*)pObj;
		float value = (accumUnitsOfTime * newUnitOfTimeValue) / singleDuration;
		accumUnitsOfTime += frame->getDelayUnits();
		m_pSplitTimes->push_back(value);
	}
}

ADAnimate* ADAnimate::create( CCAnimation *pAnimation )
{
	ADAnimate *pAnimate = new ADAnimate();
	pAnimate->initWithAnimation(pAnimation);
	pAnimate->autorelease();

	return pAnimate;
}



}

