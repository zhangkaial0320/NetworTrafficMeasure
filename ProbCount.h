#ifndef PROBCOUNT_H
#define PROBCOUNT_H

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include "Utility.h"
#include "TrafficMeasureBase.h"
using namespace std;




class ProbCount:public TrafficMeasureBase
{
public:
	ProbCount(const size_t& bitmapSize);
	~ProbCount();

	void Measurement();
private:

	void UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize);

	unordered_map<unsigned int, unsigned int* > m_probCountTab;
	size_t m_bitmapSize;
	const size_t m_elementSize;
};

#endif