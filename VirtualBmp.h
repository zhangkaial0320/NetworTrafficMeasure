#ifndef VIRTUALBMP_H
#define VIRTUALBMP_H

#include <vector>
#include <set>
#include "TrafficMeasureBase.h"

using namespace std;



class VirtualBmp : public TrafficMeasureBase
{
public:
	VirtualBmp(const size_t& bitmapSize, const size_t& virtualBmpSize);
	~VirtualBmp();

	void Measurement();
private:

	void UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize);


	unsigned int* 					m_bitmap;
	unsigned int* 					m_random;

	set<unsigned int> 				m_srcId;

	const size_t 					m_bitmapSize;

	const size_t 					m_virtualBmpSize;

	const size_t 					m_elementSize;
};

#endif