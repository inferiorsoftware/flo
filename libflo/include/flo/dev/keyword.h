#pragma once

#include <string>

namespace flo
{

namespace Keyword
{
	enum Keyword
	{
		Out
	};

	bool isKeyword(std::string str);
	Keyword asKeyword(std::string str);
};

}
