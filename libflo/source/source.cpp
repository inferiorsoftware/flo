#include "flo/dev/source.h"

#include <fstream>
#include <sstream>

flo::Source::Source(const std::string id, const std::string code)
	:id(id), code(code) {}

void flo::Source::getDebugInfoAtIndex(const int index, int& lineNum, int& colNum, std::string& line)
{
	lineNum = 0;
	colNum = 0;

	std::stringstream ss;

	bool found = false;


	for(int i=0; i<code.size(); ++i)
	{
		const char ch = code.at(i);

		if(i < index)
		{
			if(ch == '\n')
			{
				lineNum++;
				colNum = 0;

				ss.str("");
				continue;
			}

			colNum++;
		}
		else if(ch == '\n') break;
		ss << ch;
	}

	line = ss.str();
}

flo::Source flo::Source::load(const std::string path)
{
	std::ifstream ifs(path);

	if(!ifs)
	{
		throw std::runtime_error("Failed to open file " + path);
	}

	std::string code((std::istreambuf_iterator<char>(ifs)),
			std::istreambuf_iterator<char>());

	return Source(path, code);
}
