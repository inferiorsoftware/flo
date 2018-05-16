#include "flo/dev/chunk_factory.h"


flo::util::ChunkFactory& flo::util::ChunkFactory::op(const flo::Opcode o)
{
	if(o == Opcode::Const)
	{
		throw std::runtime_error("Do not add opcode 'Const' directly; use ChunkFactory::const().");
	}

	bytes.push_back((uint8_t) o);

	return *this;
}

flo::util::ChunkFactory&flo::util::ChunkFactory::constant(flo::ValuePtr value)
{
	const int index = constants.size();
	if(index > UINT8_MAX)
	{
		throw std::runtime_error("Too many constants!");
	}

	constants.push_back(value);

	bytes.push_back((uint8_t) Opcode::Const);
	bytes.push_back((uint8_t) index);

	return *this;
}


flo::Chunk flo::util::ChunkFactory::end()
{
	bytes.push_back((uint8_t) Opcode::End);

	return Chunk(bytes, constants);
}
