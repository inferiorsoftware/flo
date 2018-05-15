#include "flo/lang/number.h"
#include <cmath>
#include <iomanip>

flo::Number::Number(const double v) : v(v) {}

double flo::Number::asNumber() const
{
	return v;
}

void flo::Number::toString(std::stringstream& ss) const
{
	double integ;
	if(modf(v, &integ) == 0)
	{
		ss << integ;
	}
	else
	{
		ss << v;
	}
}

flo::ValuePtr flo::Number::create(const double v)
{
	return ValuePtr(new Number(v));
}
