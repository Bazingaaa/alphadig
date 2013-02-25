
#include "StatisticsManager.h"


namespace AlphaDig
{

StatisticsManager& StatisticsManager::getSingleton()
{
	static StatisticsManager s_Singleton;

	return s_Singleton;
}

StatisticsManager::StatisticsManager()
{
}

StatisticsManager::~StatisticsManager()
{
}

void StatisticsManager::addStatisticsByName( const std::string &strStatName, int nValue )
{
	m_statisticsMap[ strStatName ] += nValue;
}



}

