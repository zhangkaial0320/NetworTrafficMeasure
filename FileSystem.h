#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <iostream>
#include <fstream>

class FileSystem
{
public:
	FileSystem(const string& file, )
private:
	ifsteam 		m_inputFile;
	ofstream 		m_outputFile;

	char* ReadRandomParameter(string fileName, size_t* size)
	{
	 	char * buffer;
	  	ifstream file (fileName, std::ios::in|std::ios::binary|std::ios::ate);
	  	*size = file.tellg();
	  	file.seekg (0, ios::beg);
	  	buffer = new char [*size];
	  	file.read (buffer, *size);
	  	file.close();
	  	return buffer;
	}
};

#endif