#pragma once

#include <string>

namespace flo
{

struct Source
{
	const std::string id;
	const std::string code;

	Source(const std::string id,
		   const std::string code);

	void getDebugInfoAtIndex(const int index, int& lineNum, int& colNum, std::string& line);

	static Source load(const std::string path);
};

}
