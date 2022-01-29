#pragma once

#include <vector>
namespace YabeEngine
{
	class IOHandler
	{
	public:
		static bool readToBuff(std::string filePath, std::vector<unsigned char>& buffer);
	};
}