#include "flo/dev/keyword.h"

#include <unordered_map>

static std::unordered_map<std::string, flo::Keyword::Keyword> keywords
{
	{"out", flo::Keyword::Out }
};

bool flo::Keyword::isKeyword(std::string str)
{
	return keywords.find(str) != keywords.end();
};

flo::Keyword::Keyword flo::Keyword::asKeyword(std::string str)
{
	if(!isKeyword(str))
	{
		throw std::runtime_error(
					str + " is not a valid keyword. Check this with flo::Keyword::isKeyword().");
	}

	return keywords[str];
}
