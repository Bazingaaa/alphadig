

#ifndef ADANIMATE_H__
#define ADANIMATE_H__


#include "actions/CCActionInterval.h"


namespace AlphaDig
{

using namespace cocos2d;

class ADAnimate : public CCAnimate
{
public:
	ADAnimate();
	~ADAnimate();

	///<recaculate the frame split times value
	void reCaculateSplitTimes( );


	///<create
	static ADAnimate* create( CCAnimation *pAnimation );

};


}






#endif

