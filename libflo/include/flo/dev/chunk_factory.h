#pragma once

#include "flo/bytecode/chunk.h"
#include "flo/bytecode/opcode.h"

namespace flo
{
namespace util
{

class ChunkFactory
{
	std::vector<uint8_t> bytes;
	std::vector<ValuePtr> constants;

public:
	ChunkFactory& op(const Opcode o);
	ChunkFactory& constant(ValuePtr value);
	Chunk end();
};

}
}
