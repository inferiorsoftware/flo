#pragma once

#include "flo/dev/ast.h"
#include "flo/bytecode/chunk.h"

#include <vector>

namespace flo
{

Chunk* generate(std::vector<StmtPtr> statements);

}
