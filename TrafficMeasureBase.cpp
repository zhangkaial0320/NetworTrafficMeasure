#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "matplotlib-cpp/matplotlibcpp.h"
#include "Utility.h"
#include "TrafficMeasureBase.h"

namespace plt = matplotlibcpp;

bool TrafficMeasureBase::LoadTrafficFile(const string& path)
{
	m_inputFile.open(path);
	if(!m_inputFile.is_open())
	{
		printf("The file does not exist!\n");
		return false;
	}
	return true;
}

void TrafficMeasureBase::StartProcess()
{
	string line;
	unsigned int srcId;
	unsigned int dstId;
	unsigned int flowSize;

	std::stringstream buffer;
    buffer << m_inputFile.rdbuf();
	
	vector<string> tokens;
	copy(istream_iterator<string>(buffer), istream_iterator<string>(), back_inserter(tokens));
	//std::cout << tokens.size() << std::endl;

	size_t index = 6;					// skip the title
	while(index < tokens.size())
	{
		srcId = Utility::Str2Ip(tokens[index++]);
		dstId = Utility::Str2Ip(tokens[index++]);
		flowSize = atoi(tokens[index++].c_str());
		UpdateTable(srcId, dstId, flowSize);
	}
	m_inputFile.close();
}

void TrafficMeasureBase::BenchMark(const unordered_map<unsigned int, unsigned int>& groundTruth, const string& resultFileName)
{
	m_outputFile.open(resultFileName);

	unordered_map<unsigned int, unsigned int>::iterator it = m_benchMark.begin();

	for(; it != m_benchMark.end(); it++)
	{
		unsigned int ip = it->first;
		string ipStr = Utility::Ip2Str(ip);

		unsigned int spreadMeasure = it->second;
		if(groundTruth.find(ip) == groundTruth.end())
		{
			printf("BenchMark: Error, the ip(:%s) is not in groundTruth\n", ipStr.c_str());
			exit(0);
		}
		unordered_map<unsigned int, unsigned int>::const_iterator iter = groundTruth.find(ip);
		unsigned int spreadGroundtruth = iter->second;

		m_outputFile<<ipStr<<"\t"<<spreadMeasure<<"\t"<<spreadGroundtruth<<endl;
	}
	m_outputFile.close();
}

void TrafficMeasureBase::MakePlot(const string& benchmarkFile, const string& resImageName)
{
	string line;
	vector<float> measures;
	vector<float> groundTruths;

	std::stringstream buffer;
	m_inputFile.open(benchmarkFile);
    buffer << m_inputFile.rdbuf();
	
	vector<string> tokens;
	copy(istream_iterator<string>(buffer), istream_iterator<string>(), back_inserter(tokens));
	//std::cout << tokens.size() << std::endl;

	size_t index = 0;					// skip the title
	while(index < tokens.size())
	{
		unsigned int srcId = Utility::Str2Ip(tokens[index++]);
		float measure = 1.0 * atoi(tokens[index++].c_str());
		float groundTruth  = 1.0 * atoi(tokens[index++].c_str());
		measures.push_back(measure);
		groundTruths.push_back(groundTruth);
	}
	m_inputFile.close();

	// m_outputFile.open(resImageName);
	// vector<float> data(2*measures.size(), 0.0);
	// for(size_t i = 0; i < measures.size(); i++)
	// {
	// 	data[2*i] = measures[i];
	// 	data[2*i+1] = groundTruths[i];
	// }
	// m_outputFile.write((char*)(data.data()), data.size() * sizeof(unsigned int));
	// m_outputFile.close();
	plt::clf();
	plt::plot(measures, groundTruths, "*");
	plt::plot(groundTruths, groundTruths, "r-");
	cout << "Saving result to " << resImageName << endl;
	plt::save(resImageName);
}