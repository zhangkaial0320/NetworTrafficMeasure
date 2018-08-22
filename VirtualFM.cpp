#include "VirtualFM.h"
#include "Utility.h"
#include <math.h>
#include <string.h>
VirtualFM::VirtualFM(const size_t& sketchCount, const size_t& virtualSketchCount):
					m_sketchCount(sketchCount), m_virtualSketchCount(virtualSketchCount), m_sketchSize(sizeof(unsigned int) * 8)
{
	m_sketchs = new unsigned int[sketchCount];
	memset(m_sketchs, 0, sizeof(unsigned int) * sketchCount );

	srand(time(NULL));

	m_random = new unsigned int[virtualSketchCount];

	for(size_t i=0; i < virtualSketchCount; i++)
	{
		m_random[i] = rand();
	}
}


VirtualFM::~VirtualFM()
{
	if(m_sketchs != NULL)
		delete[] m_sketchs;
	if(m_random != NULL)
		delete[] m_random;
}

void VirtualFM::UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize)
{
	// e = dstId, f = srcId
	size_t uniHashVal = std::hash<unsigned int>{}(dstId) % m_virtualSketchCount;		// uniform hash: 0 ~ s-1
	size_t geoHash = GeoHash(dstId);													// geo hash: 0 ~ 31

	size_t index 	= std::hash<unsigned int>{}( srcId ^ m_random[ uniHashVal ] ) % m_sketchCount;
	size_t bitPos 	= geoHash; 

	m_sketchs[index] = m_sketchs[index] | (0x01 << bitPos);
	//printf("index = %d, bitPos = %d\n", index, bitPos);
	m_srcId.insert(srcId);
}

unsigned int VirtualFM::GeoHash(const unsigned int x)
{
	if(x == 0)
		return 0;

	// this code is quick version of finding successive 1-bits
	return m_sketchSize - (int)log2( 1.0 * (x & (-x)) ) - 1;
}

void VirtualFM::Measurement()
{
	unsigned int count = 0;
	set<unsigned int>::iterator it;

	for(size_t i=0; i<m_sketchCount; i++)
	{
		count += LeadingOnes(m_sketchs[i]);
	}
	float zm = count * 1.0 / m_sketchCount;

	//int time = 0;
	for( it = m_srcId.begin(); it != m_srcId.end(); it++ )
	{
		count = 0;
		unsigned int srcId = *it;

		// collect virtual sketches 
		for(size_t i = 0; i < m_virtualSketchCount; i++)
		{
			unsigned int r = m_random[i];
			unsigned int hashVal = std::hash<unsigned int>{}(srcId ^ r) % m_sketchCount;
			count += LeadingOnes(m_sketchs[hashVal]);	
		}
		float zs = count * 1.0 / m_virtualSketchCount;
		
		unsigned int spread = round( (m_virtualSketchCount / 0.78) * ( abs(pow(2.0, zm) - pow(2.0,zs)) ) );

		// write measurement into hashtable for benchmark use
		m_benchMark[srcId] = spread;
	}
	m_outputFile.close();
}

unsigned int VirtualFM::LeadingOnes(const unsigned int& x)
{
	if((x >> 31) == 0)
		return 0;

	for(size_t i=0; i<m_sketchSize; i++)
	{

		if( ( (x >> (m_sketchSize - i - 1) ) & 0x01 ) == 0 )
			return (i+1); 
	}
	return 0;
}
