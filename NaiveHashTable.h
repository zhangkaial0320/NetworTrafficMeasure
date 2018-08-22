#ifndef NAIVE_HASH_TABLE_H
#define NAIVE_HASH_TABLE_H


#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include "Utility.h"
#include "TrafficMeasureBase.h"
using namespace std;


class NaiveHashTable:public TrafficMeasureBase
{
public:
	virtual void Measurement();
	void MeasureFlowSizePerSrc();
	const unordered_map<unsigned int, unsigned int>& GetGroundtruthSpread(){return m_groundtruthSpread;}
	const unordered_map<unsigned int, unsigned int>& GetGroundtruthFlowSize(){return m_groundtruthFlowSize;}
private:
	virtual void UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize);

	unordered_map<unsigned int, unordered_map<unsigned int, unsigned int> > m_2levelHash;

	unordered_map<unsigned int, unsigned int> 	m_groundtruthSpread;
	unordered_map<unsigned int, unsigned int> 	m_groundtruthFlowSize;
};

#endif