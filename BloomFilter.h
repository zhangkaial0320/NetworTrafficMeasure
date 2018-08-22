#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H


#include <set>
#include <string>
#include <fstream>
#include "TrafficMeasureBase.h"
using namespace std;

class BloomFilter : public TrafficMeasureBase
{
public:
	BloomFilter(const size_t& sizeBloomFilter, const size_t& sizeHashFunc);
	~BloomFilter();

	virtual bool LoadTrafficFile(const string& path);
	virtual void Measurement(){;}

	virtual bool Measurement(const unsigned int& memberID);
	virtual void StartProcess();
	double GetFalsePositive();

protected:
	virtual void UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize){;}

	void RecordMember(const unsigned int& memberID);

private:
	
	set<unsigned int> 		m_groundTruth;

	ifstream 				m_inputFile;
	ofstream 				m_outputFile;
	size_t 					m_m;
	size_t					m_k;
	size_t 					m_n;

	unsigned int*			m_random;
	unsigned int* 			m_memberTable;
};

#endif