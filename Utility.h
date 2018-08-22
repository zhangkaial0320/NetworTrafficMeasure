#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;


class Utility
{
public:
	static unsigned int Str2Ip(const string& ipStr)
	{
   		std::string token;
   		std::istringstream tokenStream(ipStr);

   		unsigned int ip = 0;
   		unsigned int bits[] = {24, 16, 8, 0};
   		int i = 0 ;
   		while (std::getline(tokenStream, token, '.'))
   		{
   			unsigned char number = atoi(token.c_str());
   			ip = ip | (number << bits[i++]);
   		}
   		
   		if(token.size() == 0)
			return 0;
		else
			return ip;
	}

	static string Ip2Str(unsigned int ip)
	{
		char buf[40];
		sprintf(buf, "%u.%u.%u.%u", (ip&0xff000000)>>24, (ip&0x00ff0000)>>16, (ip&0x0000ff00)>>8, (ip&0x000000ff));
		return buf;
	}

	static bool Compare2Files(const string& file1, const string& file2)
	{
		ifstream f1(file1);
		ifstream f2(file2);
		unordered_map<string, int> res1;
		unordered_map<string, int> res2;
		string line;
		while(getline(f1, line))
		{
			istringstream iss(line);
			vector<string> tokens;
			copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

			res1[tokens[0]] = atoi(tokens[1].c_str());
		}

		while(getline(f2, line))
		{
			istringstream iss(line);
			vector<string> tokens;
			copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

			res2[tokens[0]] = atoi(tokens[1].c_str());
		}

		unordered_map<string, int>::iterator it;
		for(it = res1.begin(); it != res1.end(); it++)
		{
			string str = it->first;
			if(res2.find(str) == res2.end())
			{
				printf("can't find %s in table2\n", str.c_str());
				return false;
			}
			if(res2[str] != it->second)
			{
				printf("error, %s, %d, %s, %d\n", str.c_str(), it->second, str.c_str(), res2[str]);
				return false;
			}
		}
		return true;
	}

	static bool LineSeparate(const string& line, unsigned int* srcId, unsigned int* dstId, unsigned int* flowSize)
	{
		istringstream iss(line);

		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

		if(tokens.size() > 3)
			return false;
		
		*srcId = Utility::Str2Ip(tokens[0]);
		*dstId = Utility::Str2Ip(tokens[1]);
		*flowSize = atoi(tokens[2].c_str());
		
		return true;
	}
	static unsigned int numberOfSetBits(unsigned int i)
	{
     	// Java: use >>> instead of >>
     	// C or C++: use uint32_t
    	i = i - ((i >> 1) & 0x55555555);
     	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     	return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}

	static void Benchmark(const string& file1, const string& file2)
	{
		printf("Start Benchmark\n");
		ifstream f1(file1);
		ifstream f2(file2);
		unordered_map<string, unsigned int> res1;
		unordered_map<string, unsigned int> res2;
		string line;
		while(getline(f1, line))
		{
			istringstream iss(line);
			vector<string> tokens;
			copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

			res1[tokens[0]] = atoi(tokens[1].c_str());
		}

		while(getline(f2, line))
		{
			istringstream iss(line);
			vector<string> tokens;
			copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

			res2[tokens[0]] = atoi(tokens[1].c_str());
		}
		f1.close();
		f2.close();

		ofstream of("benchmark.dat", ios::out|ios::binary|ios::ate);

		unsigned int *data = new unsigned int[ 2 * res1.size()];

		unordered_map<string, unsigned int>::iterator it;
		
		int i =0;
		for(it = res1.begin(); it != res1.end(); it++)
		{
			string str = it->first;
			if(res2.find(str) == res2.end())
			{
				printf("can't find %s in table2\n", str.c_str());
				exit(0);
			}
			data[2*i]   = res2[str];
			data[2*i+1] = it->second;
			i++;
			//of.write(res2[str], sizeof(unsigned int));
			//of.write(it->second, sizeof(unsigned int));
		}
		of.write((const char*)data, sizeof(unsigned int) * 2 * res1.size());
	}
};

#endif