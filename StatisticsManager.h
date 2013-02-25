
#ifndef STATISTICS_MANAGER_H__
#define STATISTICS_MANAGER_H__

#include <string>
#include <map>


namespace AlphaDig
{

class StatisticsManager
{
public:

	static StatisticsManager& getSingleton();

	///<add a new value to specified statistics
	void addStatisticsByName( const std::string &strStatName, int nValue );
	int getStatisticsByName( const std::string &strStatName ){ return m_statisticsMap[strStatName]; }

protected:

	typedef std::map< std::string, int > StringIntMap;
	StringIntMap m_statisticsMap;


private:
	StatisticsManager();
	~StatisticsManager();
};


}



#endif

