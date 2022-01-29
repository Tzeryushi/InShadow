#include "IOHandler.h"

#include <fstream>

namespace YabeEngine
{
	/*************************************************************
	read to buff is an EXTRAORDINARILY simple binary file reader
	that seeks and gets the file size and passes the file information
	to the buffer. Based on a design by Benjamin Arnold (ChillstepCoder)
	*************************************************************/
	bool IOHandler::readToBuff(std::string filePath, std::vector<unsigned char>& buffer)
	{
		//reading in binary data!
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//getting file size
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		//reduce file size by any header bytes present
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);	//gives buffer[0] gives c_str
		file.close();

		return true;
	}

	//TODO: Implement file writing (for saves and the like)
	//this will be used for level editing as well as game saves
}