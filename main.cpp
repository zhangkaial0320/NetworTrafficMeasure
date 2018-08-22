#include "NaiveHashTable.h"
#include "Utility.h"
#include "ProbCount.h"
#include "VirtualBmp.h"
#include "VirtualFM.h"
#include "CountMin.h"
#include "BloomFilter.h"
#include <iostream>
using namespace std;

void TestHashTable()
{
	NaiveHashTable hash;

	if(!hash.LoadTrafficFile("traffic.txt"))
	{
		exit(0);
	}
	hash.StartProcess();

	//hash.MeasureSpreadPerSrc("my_output.txt");
	hash.MeasureFlowSizePerSrc();
	//Utility::Compare2Files("my_output.txt", "traffic_spread.txt");
}

void TestProbCount()
{
	ProbCount hash(1000);

	if(!hash.LoadTrafficFile("traffic.txt"))
	{
		exit(0);
	}
	hash.StartProcess();

	hash.Measurement();
	//Utility::Benchmark("my_output.txt", "traffic_spread.txt");

}

void TestVirtualBmp()
{


	VirtualBmp hash(2000000, 1000);

	if(!hash.LoadTrafficFile("traffic.txt"))
	{
		exit(0);
	}
	hash.StartProcess();

	hash.Measurement();
	Utility::Benchmark("my_output.txt", "traffic_spread.txt");
}

void TestVirtualFM()
{
	NaiveHashTable hash;

	if(!hash.LoadTrafficFile("traffic.txt"))
	{
		exit(0);
	}
	hash.StartProcess();

	//hash.MeasureSpreadPerSrc("my_output.txt");
	hash.Measurement();

	VirtualFM vhash(4000000, 4096);

	if(!vhash.LoadTrafficFile("traffic.txt"))
	{
		exit(0);
	}
	vhash.StartProcess();
	vhash.Measurement();
	vhash.BenchMark(hash.GetGroundtruthSpread(), "VirtualFM.txt");
	vhash.MakePlot("VirtualFM.txt", "VirtualFM.png");
	//Utility::Benchmark("my_output.txt", "traffic_spread.txt");
}
void TestCountMin()
{
	CountMin count(60000, 6);

	if(!count.LoadTrafficFile("traffic.txt"))
	{
		exit(0);
	}
	count.StartProcess();

	count.Measurement();
	Utility::Benchmark("my_output.txt", "my_outputFlowSize.txt");
}

void TestBloomFilter()
{
	BloomFilter bloomFilter(100000, 100);
	if(!bloomFilter.LoadTrafficFile("traffic.txt"))
	{
		exit(0);
	}
	bloomFilter.StartProcess();
	cout<<"theoretical false positive = "<<bloomFilter.GetFalsePositive()<<endl;

	const int n = 200000;

	unsigned int falseCount = 0;
	srand(time(NULL));

	for(size_t i = 0; i < n; i++)
	{
		if(bloomFilter.Measurement(rand()))
			falseCount++;
	}
	cout<< "actual false positive = "<< 1.0 * falseCount / n << endl;
}


void TestAll()
{
	TrafficMeasureBase* measure[6] = {
										new NaiveHashTable(),
									  	new ProbCount(1000),
									  	new VirtualBmp(2000000, 1000),
									  	new VirtualFM(4000000, 4096),
									  	new CountMin(60000, 6),
									  	new BloomFilter(200000, 100)
									 };

	string log[] = {"NaiveHashTable", "ProbCount", "VirtualBmp", "VirtualFM", "CountMin", "BloomFilter" };								 
	for(int i = 0; i < 6; i++)
	{
		if(!measure[i]->LoadTrafficFile("traffic.txt"))
		{
			cout<<"Load traffice file fails!"<<endl;
			exit(0);
		}
	
		printf("Start Processing : %s\n", log[i].c_str());
		measure[i]->StartProcess();
		printf("Finish recording\n\n\n");
	}
	for(int i = 0; i < 5; i++)
	{
		printf("Start Measurement : %s\n", log[i].c_str());
		measure[i]->Measurement();
		printf("Finish Measurement : %s\n\n\n", log[i].c_str());
	}

	vector<string> benchmarkFile = {"NaiveHashTable.txt", "ProbCount.txt", "VirtualBmp.txt", "VirtualFM.txt",
							         "CountMin.txt"};
	vector<string> plotImage = {"NaiveHashTable.png", "ProbCount.png", "VirtualBmp.png", "VirtualFM.png",
							         "CountMin.png"};
	((NaiveHashTable*)measure[0])->MeasureFlowSizePerSrc();
	for(int i = 0; i < 4; i++)
	{
		printf("Start generate benchmarkFile : %s\n", log[i].c_str());
		measure[i]->BenchMark(((NaiveHashTable*)measure[0])->GetGroundtruthSpread(), benchmarkFile[i]);
		printf("BenchmarkFile : %s is generated\n\n\n", benchmarkFile[i].c_str());
	}
	printf("Start generate benchmarkFile : %s\n", log[4].c_str());
	measure[4]->BenchMark(((NaiveHashTable*)measure[0])->GetGroundtruthFlowSize(), benchmarkFile[4]);
	printf("BenchmarkFile : %s is generated\n\n", benchmarkFile[4].c_str());


	printf("Start Measurement : %s\n", log[5].c_str());
	const int n = 2000000;
	unsigned int falseCount = 0;
	srand(time(NULL));


	for(size_t i = 0; i < n; i++)
	{
		if(((BloomFilter*)measure[5])->Measurement(rand()))
			falseCount++;
	}
	cout<<"BloomFilter: Random membership test ("<< n<<" members)"<<endl;
	cout<<"Theoretical false positive = "<<((BloomFilter*)measure[5])->GetFalsePositive()<<endl;
	cout<< "Actual false positive = "<< 1.0 * falseCount / n << "\n\n"<<endl;

	for(size_t i = 0; i < 5; i++)
	{
		printf("Start generate plot: %s\n", plotImage[i].c_str());
		measure[5]->MakePlot(benchmarkFile[i], plotImage[i]);
	}

	for(int i = 0; i < 6; i++)
		delete measure[i];
}


void TestPlot()
{
	vector<string> benchmarkFile = {"NaiveHashTable.txt"};
	vector<TrafficMeasureBase*> measure = {new ProbCount(1000)};
	measure[0]->MakePlot(benchmarkFile[0], "ProbCount.png");

}
int main(int argc, char** argv)
{
	//TestHashTable();
	//TestProbCount();
	//TestVirtualBmp();
	//TestVirtualFM();
	//TestCountMin();
	//TestBloomFilter();
	TestAll();
	//TestPlot();
	return 1;
}
