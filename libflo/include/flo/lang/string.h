#pragma once

#include "flo/lang/value.h"

#include <string>

namespace flo
{

class String : public Value
{
	const std::string v;

	String(std::string v);

public:
	void toString(std::stringstream& ss) const override;
	static ValuePtr create(std::string str);
};

}
