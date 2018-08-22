#ifndef VIRTUAL_FM_H
#define VIRTUAL_FM_H

#include <set>
#include "TrafficMeasureBase.h"

using namespace std;

class VirtualFM : public TrafficMeasureBase
{
public:
	VirtualFM(const size_t& sketchCount, const size_t& virtualSketchCount);
	~VirtualFM();

	void Measurement();

private:
	unsigned int LeadingOnes(const unsigned int& x);
	void UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize); 
	unsigned int GeoHash(const unsigned int x);

	unsigned int*			m_sketchs;
	unsigned int* 			m_random;
	
	set<unsigned int> 		m_srcId;

	const size_t 			m_sketchCount;
	const size_t 			m_virtualSketchCount;
	const unsigned char		m_sketchSize;

	
};

#endif