
#ifndef ADPARTICLE_SYSTEM_CACHE_H__
#define ADPARTICLE_SYSTEM_CACHE_H__


#include <list>
#include <string>
#include <map>

#include "cocoa/CCGeometry.h"

namespace cocos2d
{
	class CCParticleSystem;
	class CCParticleBatchNode;
}

namespace AlphaDig
{

using namespace cocos2d;

class ADParticleSystemCache
{
public:
	ADParticleSystemCache();
	~ADParticleSystemCache();

	///<get the singleton
	static ADParticleSystemCache& getSingleton();

	///<add particle system
	CCParticleSystem* addParticleSystem( const char *pFileName );
	///<remove all particle system
	void removeAllParticleSystem( );

	///<recycle the dead particleSystems
	void recycle();

	///<play particle system
	void playParticleSystem( const char *pFileName, const CCPoint &pos );

	///<set batch root node
	void setBatchRootNode( CCNode *pRootNode ) { m_pRootNode = pRootNode; }


protected:

	///<create new particle system
	CCParticleSystem* _createNewParticleSystem( const char *pFileName, CCParticleBatchNode *pNode );


protected:

	typedef std::list< CCParticleSystem* > ParticleSystemList;
	struct ParticleSystemBatch
	{
		CCParticleBatchNode *_pBatchNode;
		ParticleSystemList _particleSystems;
	};
	typedef std::map< std::string, ParticleSystemBatch > ParticleSystemBatchMap;

	///<the idle particles
	ParticleSystemBatchMap m_particleBatchesIdle;

	///<the particles using now
	ParticleSystemBatchMap m_particleBatchesUsed;

	///<the root node
	CCNode *m_pRootNode;

};




}




#endif

