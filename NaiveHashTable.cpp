
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "NaiveHashTable.h"


void NaiveHashTable::UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize)
{
	//printf("src = %lu, dst = %lu, flowSize = %lu\n", srcId, dstId, flowSize);
	if(m_2levelHash.find(srcId) != m_2levelHash.end())
	{
		unordered_map<unsigned int, unsigned int>* second = &(m_2levelHash[srcId]);
		if(second->find(dstId) != second->end())
		{
			(*second)[dstId] = (*second)[dstId] + flowSize;
		}
		else
		{
			(*second)[dstId] = flowSize;
		}
	}
	else
	{
		unordered_map<unsigned int, unsigned int> secondLev;
		secondLev[dstId] = flowSize;
		m_2levelHash[srcId] = secondLev;
	}
}

void NaiveHashTable::Measurement()
{
	//m_outputFile.open(path);
	unordered_map<unsigned int, unordered_map<unsigned int, unsigned int> >::iterator it;
	for (it = m_2levelHash.begin(); it != m_2levelHash.end(); ++it )
	{
		
		string ipStr = Utility::Ip2Str(it->first);
		unsigned int spread = (it->second).size();
		m_benchMark[it->first] = spread;
		m_groundtruthSpread[it->first] = spread;
		//sprintf(buf, "%s, %d\n", ipStr.c_str(), spread);
		//m_outputFile<<ipStr<<"\t"<<spread<<endl;
	}
	//m_outputFile.close();
}

void NaiveHashTable:: MeasureFlowSizePerSrc()
{
	//m_outputFile.open(path);
	unordered_map<unsigned int, unordered_map<unsigned int, unsigned int> >::iterator it;
	for (it = m_2levelHash.begin(); it != m_2levelHash.end(); ++it )
	{
		
		string ipStr = Utility::Ip2Str(it->first);

		unordered_map<unsigned int, unsigned int>::iterator lev2;
		size_t flowSize = 0;
		for(lev2 = (it->second).begin(); lev2 != (it->second).end(); lev2++)
		{
			flowSize += lev2->second;
		}
		//sprintf(buf, "%s, %d\n", ipStr.c_str(), spread);
		m_groundtruthFlowSize[it->first] = flowSize;
		//m_outputFile<<ipStr<<"\t"<<flowSize<<endl;
	}
	//m_outputFile.close();
}