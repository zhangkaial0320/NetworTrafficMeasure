#ifndef TrafficMeasure_h
#define TrafficMeasure_h

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class TrafficMeasureBase
{

public:
	virtual bool LoadTrafficFile(const string& path);
	virtual void StartProcess();
	virtual void Measurement() = 0;
	virtual void BenchMark(const unordered_map<unsigned int, unsigned int>& groundTruth, const string& resultFileName);
	virtual void MakePlot(const string& benchmarkFile, const string& resImageName);
protected:
	
	virtual void UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize) = 0;
	ifstream 		m_inputFile;
	ofstream 		m_outputFile;

	unordered_map<unsigned int, unsigned int> 	m_benchMark;
};


#endif