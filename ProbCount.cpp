#include "ProbCount.h"
#include <string.h>

ProbCount::ProbCount(const size_t& bitmapSize):m_elementSize(sizeof(unsigned int) * 8)
{
	m_bitmapSize = bitmapSize;
	//m_elementSize = sizeof(unsigned int) * 8;
}
ProbCount::~ProbCount()
{
	unordered_map< unsigned int, unsigned int* >::iterator it;
	for (it = m_probCountTab.begin(); it != m_probCountTab.end(); ++it )
	{
		string ipStr = Utility::Ip2Str(it->first);
		unsigned int* bitmap = (it->second);
		delete[] bitmap;
	}
}

void ProbCount::UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize)
{
	size_t hashVal = std::hash<unsigned int>{}(dstId) % m_bitmapSize;
	size_t index = hashVal / m_elementSize;
	size_t bitPos = hashVal % m_elementSize;
	//printf("srcIp = %lu, hashVal = %lu, index = %lu, bitpos = %lu\n", srcId, hashVal, index, bitPos);
	if(m_probCountTab.find(srcId) != m_probCountTab.end())
	{
		unsigned int* bitmap = m_probCountTab[srcId];
		bitmap[index] = bitmap[index] | (0x01 << bitPos);
	}
	else
	{
		unsigned int* bitmap = new unsigned int[m_bitmapSize / m_elementSize + 1];
		memset(bitmap, 0, sizeof(unsigned int)*(m_bitmapSize / m_elementSize + 1));
		bitmap[index] = bitmap[index] | (0x01 << bitPos);
		m_probCountTab[srcId] = bitmap;
	}
}

void ProbCount::Measurement()
{
	unordered_map< unsigned int, unsigned int* >::iterator it;

	for (it = m_probCountTab.begin(); it != m_probCountTab.end(); ++it )
	{
		
		string ipStr = Utility::Ip2Str(it->first);
		unsigned int* bitmap = it->second;
		unsigned int count = 0;
		for(size_t i = 0; i < (m_bitmapSize / m_elementSize + 1); i++)
		{
			count += Utility::numberOfSetBits(bitmap[i]);
		}
		if(count == m_bitmapSize)
		{
			printf("%s overflow, so set to max value\n", ipStr.c_str());
			count--;
		}
		// v is the number of 1s in the bitmap
		float v = count * 1.0 / m_bitmapSize;
		int spread = round( -log(1 - v) * m_bitmapSize);

		// write measurement into hashtable for benchmark use.
		m_benchMark[it->first] = spread;
	}
	cout<<"ProbCount "<<m_benchMark.size()<<endl;
}






