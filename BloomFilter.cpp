#include <time.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "BloomFilter.h"
#include "Utility.h"

BloomFilter::BloomFilter(const size_t& sizeBloomFilter, const size_t& sizeHashFunc)
{
	m_m = sizeBloomFilter * sizeof(unsigned int) * 8;
	m_k = sizeHashFunc;
	m_memberTable = new unsigned int[sizeBloomFilter];
	memset(m_memberTable, 0, sizeof(unsigned int) * sizeBloomFilter);

	m_random = new unsigned int[m_k];
	srand(time(NULL));

	for(size_t i = 0; i < m_k; i++)
	{
		m_random[i] = rand();
	}
}

BloomFilter::~BloomFilter()
{
	delete[] m_memberTable;
}

bool BloomFilter::LoadTrafficFile(const string& path)
{
	m_inputFile.open(path);
	if(!m_inputFile.is_open())
	{
		printf("The file does not exist!\n");
		return false;
	}
	return true;
}

void BloomFilter::StartProcess()
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
		RecordMember(srcId);
		RecordMember(dstId);
		m_groundTruth.insert(srcId);
		m_groundTruth.insert(dstId);
	}
	m_inputFile.close();

	m_n = m_groundTruth.size();

	m_k = ceil(m_m / m_n * log(2.0));
	printf("optimal k = %lu\n", m_k);
	printf("size of member = %lu\n", m_n);
	printf("size of bits = %lu\n", m_m);
}

void BloomFilter::RecordMember(const unsigned int& memberID)
{
	size_t bitIndex;
	size_t index;
	size_t bit;
	for(size_t i = 0; i < m_k; i++)
	{
		bitIndex = std::hash<unsigned int>{}(memberID ^ m_random[i]) % m_m;
		
		index = bitIndex / (sizeof(unsigned int)*8);
		bit   = bitIndex % (sizeof(unsigned int)*8);

		m_memberTable[index] = 0x01 << bit;
	}
}

bool BloomFilter::Measurement(const unsigned int& memberID)
{
	size_t bitIndex;
	size_t index;
	size_t bit;

	bool val = true;
	for(size_t i = 0; i < m_k; i++)
	{
		bitIndex = std::hash<unsigned int>{}(memberID ^ m_random[i]) % m_m;
		
		index = bitIndex / (sizeof(unsigned int)*8);
		bit   = bitIndex % (sizeof(unsigned int)*8);

		val = val && (m_memberTable[index]>>bit);
	}
	// this is false positive, a non-member is claimed as member
	if((val == true) && (m_groundTruth.find(memberID) == m_groundTruth.end())) 
		return true;
	else
		return false;
}

double BloomFilter::GetFalsePositive()
{
	double falsePostive = pow(1.0 - exp(-1.0 * m_k * m_n / m_m), m_k);

	return falsePostive;
}