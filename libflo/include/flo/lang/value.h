#pragma once

#include "flo/lang/type.h"
#include <string>
#include <sstream>
#include <memory>

namespace flo
{

class Value
{
public:
	virtual double asNumber() const;
	virtual void toString(std::stringstream& ss) const = 0;
};
typedef std::shared_ptr<Value> ValuePtr;

}
