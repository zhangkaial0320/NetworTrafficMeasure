#include "CountMin.h"
#include <time.h>
#include <string.h>
#include "Utility.h"

CountMin::CountMin(const size_t& elementCount, const size_t& counterSize ):
					m_elementCount(elementCount), m_counterSize(counterSize), m_elementSizeInBits(sizeof(unsigned int) * 8)
{
	m_size = m_elementCount * m_counterSize;


	m_counters = new unsigned int[ m_size ];
	memset(m_counters, 0, sizeof(unsigned int)*m_size);

	m_random = new unsigned int[m_counterSize];
	srand(time(NULL));

	for(size_t i = 0; i < m_counterSize; i++)
	{
		m_random[i] = rand();
	}
}

CountMin::~CountMin()
{
	if(m_counters != NULL)
		delete[] m_counters;
}

void CountMin::UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize)
{
	size_t index;
	for(size_t row = 0; row < m_counterSize; row++)
	{
		index = std::hash<unsigned int>{}(srcId ^ m_random[row]) % m_elementCount;
		
		index = index + row * m_elementCount;

		m_counters[index] += flowSize;
	}

	m_srcId.insert(srcId);
}

void CountMin::Measurement()
{
	//m_outputFile.open(path);

	set<unsigned int>::iterator it;

	unsigned int srcId;
	size_t index;

	for(it = m_srcId.begin(); it != m_srcId.end(); it++)
	{
		unsigned int count = 0xFFFFFFFF;
		srcId = *it;

		for(size_t row = 0; row < m_counterSize; row++)
		{
			index = std::hash<unsigned int>{}(srcId ^ m_random[row]) % m_elementCount;
			index = index + row * m_elementCount;
	
			count = min(count, m_counters[index]);
		}
		m_benchMark[srcId] = count;
		//m_outputFile << Utility::Ip2Str(srcId).c_str() << "\t" << count << endl;
	}
	
	//m_outputFile.close();
}