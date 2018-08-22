#include "VirtualBmp.h"
#include <time.h>
#include <string.h>
#include <math.h>
#include "Utility.h"

VirtualBmp::VirtualBmp(const size_t& bitmapSize, const size_t& virtualBmpSize):
						m_bitmapSize(bitmapSize), m_virtualBmpSize(virtualBmpSize), m_elementSize(sizeof(unsigned int) * 8)
{
	m_bitmap = new unsigned int[ m_bitmapSize /m_elementSize + 1 ] ;
	memset(m_bitmap, 0, sizeof(unsigned int) * (m_bitmapSize /m_elementSize + 1) );

	srand(time(NULL));

	m_random = new unsigned int[m_virtualBmpSize];

	for(size_t i=0; i < m_virtualBmpSize; i++)
	{
		m_random[i] = rand();
	}

}

VirtualBmp::~VirtualBmp()
{
	if(m_bitmap != NULL)
		delete[] m_bitmap;
	if(m_random != NULL)
		delete[] m_random;
}

void VirtualBmp::UpdateTable(const unsigned int& srcId, const unsigned int& dstId, const unsigned int& flowSize)
{
	// e -> dstId, f->srcId;
	int i = std::hash<unsigned int>{}(dstId) % m_virtualBmpSize;		// i : [0, s)
	int r = m_random[i];

	size_t hashVal = std::hash<unsigned int>{}(srcId ^ r) % m_bitmapSize;	

	size_t index = hashVal / m_elementSize;
	size_t bitPos = hashVal % m_elementSize;

	m_bitmap[index] = m_bitmap[index] | (0x01 << bitPos);

	m_srcId.insert(srcId);	// save src id, for output purpose
}

void VirtualBmp::Measurement()
{
	unsigned int count = 0;
	set<unsigned int>::iterator it;
		
	for(size_t i = 0; i < (m_bitmapSize /m_elementSize + 1); i++)
	{
		count += Utility::numberOfSetBits(m_bitmap[i]);
	}

	if(count == m_bitmapSize )
	{
		printf("overflow, so set to max value\n");
		count--;
	}

	float vm = 1.0 - ( count * 1.0 / m_bitmapSize );

	for( it = m_srcId.begin(); it != m_srcId.end(); it++)
	{
		count = 0;
		unsigned int srcId = *it;
		// collect number of 1s in S array
		for(size_t i = 0; i < m_virtualBmpSize; i++)
		{
			unsigned int r = m_random[i];
			size_t hashVal = std::hash<unsigned int>{}(srcId ^ r) % m_bitmapSize;	
	
			size_t index = hashVal / m_elementSize;
			size_t bitPos = hashVal % m_elementSize;
	
			count += ( m_bitmap[index]  >> bitPos ) & 0x01;
		}

		if(count == m_virtualBmpSize )
		{
			printf("%s overflow, so set to max value\n", Utility::Ip2Str(srcId).c_str());
			count--;
		}

		float vs = 1.0 - ( count * 1.0 / m_virtualBmpSize );

		unsigned int spread = round( m_virtualBmpSize * ( abs(log(vm) - log(vs)) ) );

		m_benchMark[srcId] = spread;
		//m_outputFile << Utility::Ip2Str(srcId).c_str() << "\t" << spread << endl;
	}

	//m_outputFile.close();
}