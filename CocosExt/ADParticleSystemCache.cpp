

#include "CocosExt/ADParticleSystemCache.h"

#include "particle_nodes/CCParticleSystemQuad.h"
#include "particle_nodes/CCParticleBatchNode.h"




namespace AlphaDig
{

ADParticleSystemCache& ADParticleSystemCache::getSingleton()
{
	static ADParticleSystemCache s_singleton;
	return s_singleton;
}

ADParticleSystemCache::ADParticleSystemCache()
: m_pRootNode( NULL )
{

}

ADParticleSystemCache::~ADParticleSystemCache()
{

}

CCParticleSystem* ADParticleSystemCache::addParticleSystem( const char *pFileName )
{
	CCParticleSystem *pRetPart = NULL;
	ParticleSystemBatchMap::iterator iter = m_particleBatchesIdle.find( pFileName );
	if( iter == m_particleBatchesIdle.end() )
	{
		pRetPart = CCParticleSystemQuad::create( pFileName );
		CCParticleBatchNode *pNode = CCParticleBatchNode::createWithTexture( pRetPart->getTexture(), 100 );
		m_pRootNode->addChild( pNode );
		pNode->addChild( pRetPart );

		m_particleBatchesUsed[pFileName]._pBatchNode = pNode;

		m_particleBatchesIdle[pFileName]._pBatchNode = pNode;
	}
	else
	{
		if( iter->second._particleSystems.empty() )
		{
			pRetPart = _createNewParticleSystem( pFileName, iter->second._pBatchNode );
			iter->second._particleSystems.push_back( pRetPart );
		}
		else
		{
			pRetPart = iter->second._particleSystems.front();
			iter->second._particleSystems.pop_front();
		}
	}

	CCAssert( pRetPart != NULL, " add particle system error " );

	m_particleBatchesUsed[pFileName]._particleSystems.push_back( pRetPart );

	return pRetPart;
}


void ADParticleSystemCache::removeAllParticleSystem()
{
	ParticleSystemBatchMap::iterator iter = m_particleBatchesUsed.begin();
	for( ; iter != m_particleBatchesUsed.end(); ++iter )
	{
		iter->second._particleSystems.clear();
		iter->second._pBatchNode->removeAllChildrenWithCleanup(true);
		iter->second._pBatchNode = NULL;
	}
	m_particleBatchesUsed.clear();

	for( iter = m_particleBatchesIdle.begin(); iter != m_particleBatchesIdle.end(); ++iter )
	{
		iter->second._particleSystems.clear();
		iter->second._pBatchNode->removeAllChildrenWithCleanup(true);
		iter->second._pBatchNode = NULL;
	}
	m_particleBatchesIdle.clear();
}

void ADParticleSystemCache::recycle()
{
	ParticleSystemBatchMap::iterator iter = m_particleBatchesUsed.begin();
	for( ; iter != m_particleBatchesUsed.end(); ++iter )
	{
		ParticleSystemList &usedList = iter->second._particleSystems;
		for( ParticleSystemList::iterator iterList = usedList.begin(); iterList != usedList.end(); )
		{
			if( !(*iterList)->isActive() && (*iterList)->getParticleCount() == 0 )
			{
				//(*iterList)->resetSystem();
				//(*iterList)->stopSystem();
				m_particleBatchesIdle[iter->first]._particleSystems.push_back( *iterList );

				iterList = usedList.erase( iterList );
			}
			else
				++iterList;
		}
	}
}

void ADParticleSystemCache::playParticleSystem( const char *pFileName, const CCPoint &pos )
{
	CCParticleSystem *pPart = addParticleSystem( pFileName );
	pPart->setPosition( pos );
	pPart->resetSystem();
}


CCParticleSystem* ADParticleSystemCache::_createNewParticleSystem( const char *pFileName, CCParticleBatchNode *pNode )
{
	CCParticleSystem *pRetPart = CCParticleSystemQuad::create( pFileName );
	pNode->addChild( pRetPart );

	return pRetPart;
}


}

