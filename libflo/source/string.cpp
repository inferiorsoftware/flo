#include "flo/lang/string.h"

flo::String::String(std::string v) : v(v) {}

void flo::String::toString(std::stringstream& ss) const
{
	ss << v;
}

flo::ValuePtr flo::String::create(std::string str)
{
	return ValuePtr(new String(str));
}
