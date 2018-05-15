#pragma once

#include "flo/lang/value.h"
#include <vector>
#include <cstdint>

namespace flo
{

/** A 'chunk' of bytecode. */
class Chunk
{
	const std::vector<uint8_t> bytes;
	const std::vector<flo::ValuePtr> constants;

public:
	Chunk(const std::vector<uint8_t> bytes,
		  const std::vector<flo::ValuePtr> constants);

	int size() const;
	uint8_t at(const int index) const;
	ValuePtr constant(const uint8_t index) const;

	std::vector<uint8_t>::const_iterator begin();
};

}
