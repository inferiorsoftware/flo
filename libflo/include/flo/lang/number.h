#pragma once

#include "flo/lang/value.h"
#include <memory>

namespace flo
{

class Number : public Value
{
	const double v;

	Number(const double v);

public:
	double asNumber() const final;
	void toString(std::stringstream& ss) const override;

	static ValuePtr create(const double v);
};

}
