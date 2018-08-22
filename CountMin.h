#ifndef COUNT_MIN_H
#define COUNT_MIN_H


#include <set>
#include "TrafficMeasureBase.h"


class CountMin : public TrafficMeasureBase
{

public:
	CountMin(const size_t& bitsSize, const size_t& counterSize );
	~CountMin();
	void Measurement();

private:
	void UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize); 

	unsigned int*			m_counters;
	unsigned int 			m_size;

	unsigned int*			m_random;

	const unsigned int		m_elementCount;
	const unsigned int 		m_counterSize;
	const unsigned int 		m_elementSizeInBits;

	set<unsigned int> 		m_srcId;
};

#endif