#pragma once

#include "flo/bytecode/chunk.h"
#include "flo/dev/source.h"
#include <optional>
namespace flo
{

std::optional<Chunk> compile(const Source& src);

}
