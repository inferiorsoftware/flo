#pragma once

#include "flo/dev/source.h"
#include "flo/dev/token.h"
#include "flo/dev/compile_error.h"

#include <vector>

namespace flo
{
std::vector<Token> tokenise(std::string code, CompileErrorListener* err);
std::vector<Token> tokenise(std::string code);
}
