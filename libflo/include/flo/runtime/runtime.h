#pragma once
#include "flo/bytecode/chunk.h"
#include "flo/lang/value.h"
#include <vector>

namespace flo
{

class Runtime
{
	std::vector<ValuePtr> stack;

	void push(ValuePtr v);
	ValuePtr pop();
	std::pair<ValuePtr, ValuePtr> pop2();

public:
	int run(Chunk& chunk);
};

}
