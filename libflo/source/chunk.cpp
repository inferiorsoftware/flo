#include "flo/bytecode/chunk.h"

flo::Chunk::Chunk(const std::vector<uint8_t> bytes,
				  const std::vector<flo::ValuePtr> constants)
	: bytes(bytes),
	  constants(constants)
{}

int flo::Chunk::size() const
{
	return bytes.size();
}

uint8_t flo::Chunk::at(const int index) const
{
	return bytes[index];
}

flo::ValuePtr flo::Chunk::constant(const uint8_t index) const
{
	return constants[(int) index];
}

std::vector<uint8_t>::const_iterator flo::Chunk::begin()
{
	return bytes.begin();
}

