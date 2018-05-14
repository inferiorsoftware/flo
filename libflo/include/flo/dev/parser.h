#pragma once

#include "flo/dev/ast.h"
#include "flo/dev/compile_error.h"
#include <vector>

namespace flo
{

std::vector<StmtPtr> parse(std::vector<Token> tokens, CompileErrorListener* err);
std::vector<StmtPtr> parse(std::vector<Token> tokens);

}
